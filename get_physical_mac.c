/**
 * @file get_physical_mac.c
 * @brief Get the real MAC address from the NIC hardware
 * @author yjf_victor
 * @version 1.0
 * @date 2016-12-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#include <net/if.h>

#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <linux/if_ether.h>

#ifndef MAX_ADDR_LEN
#define MAX_ADDR_LEN	32
#endif

#ifndef MAC_FMT
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifndef MAC_ARG
#define MAC_ARG(x) (((unsigned char *)(x))[0]), (((unsigned char *)(x))[1]), (((unsigned char *)(x))[2]), (((unsigned char *)(x))[3]), (((unsigned char *)(x))[4]), (((unsigned char *)(x))[5])
#endif

/**
 * @brief print the usage to `stderr`
 * @param[in] prog_name the name of the program, i.e. `argv[0]`
 */
static void usage(const char *prog_name)
{
	assert (prog_name != NULL);
	fprintf (stderr, "%s <interface>\n", prog_name);
}

/**
 * @brief allocate the memory for `struct ethtool_perm_addr`, and initialize its member variable
 * @return the pointer to the allocated memory, or `NULL` on failure
 */
static struct ethtool_perm_addr* new_ethtool_perm_addr(void)
{
	size_t epaddr_length = sizeof(struct ethtool_perm_addr) + MAX_ADDR_LEN;
	struct ethtool_perm_addr* epaddr = (struct ethtool_perm_addr *)malloc(epaddr_length);
	if (epaddr == NULL)
		return NULL;

	memset(epaddr, 0, epaddr_length);
	epaddr->cmd = ETHTOOL_GPERMADDR;
	epaddr->size = MAX_ADDR_LEN;

	return epaddr;
}

/**
 * @brief the `main` entry
 * @param[in] argc  argument count
 * @param[in] argv  argument values
 * @return  `EXIT_SUCCESS` or `EXIT_FAILURE`
 */
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		usage(argv[0]);
		goto failure;
	}

	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, argv[1], sizeof(ifr.ifr_name));
	struct ethtool_perm_addr* epaddr = new_ethtool_perm_addr();
	if (epaddr == NULL)
	{
		fputs("malloc() failed\n", stderr);
		goto failure;
	}
	ifr.ifr_data = (char *)epaddr;

	int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sockfd < 0)
	{
		perror("socket() failed");
		goto free_and_failure;
	}
	if (ioctl(sockfd, SIOCETHTOOL, &ifr) < 0)
	{
		perror("ioctl(SIOCETHTOOL) failed");
		goto close_and_failure;
	}

	close(sockfd);
	sockfd = -1;

	if (epaddr->size != ETH_ALEN)
	{
		fputs("MAC size error\n", stderr);
		goto free_and_failure;
	}

	printf(MAC_FMT "\n", MAC_ARG(epaddr->data));

	free(epaddr);
	epaddr = NULL;
	ifr.ifr_data = (char *)(epaddr = NULL);
	return EXIT_SUCCESS;

close_and_failure:
	close(sockfd);
	sockfd = -1;
free_and_failure:
	free(epaddr);
	epaddr = NULL;
failure:
	return EXIT_FAILURE;
}
