Get the Real MAC Address from the NIC Hardware
===

This is a program to get the real MAC address from the NIC hardware.

The core code is 

    epaddr->cmd = ETHTOOL_GPERMADDR;
    epaddr->size = MAX_ADDR_LEN;

and

    ioctl(sockfd, SIOCETHTOOL, &ifr);
