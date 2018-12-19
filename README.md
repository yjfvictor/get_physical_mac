Get the Real MAC Address from the NIC Hardware
===

This is a program to get the real MAC address from the NIC hardware.

The core code is 

    epaddr->cmd = ETHTOOL_GPERMADDR;
    epaddr->size = MAX_ADDR_LEN;

and

    ioctl(sockfd, SIOCETHTOOL, &ifr);


---


<p xmlns:dct="http://purl.org/dc/terms/">
<a rel="license"
href="https://creativecommons.org/publicdomain/zero/1.0/">
<img src="https://i.creativecommons.org/p/zero/1.0/88x31.png" style="border-style: none;" alt="CC0" />
</a>
<br />
To the extent possible under law,
<span resource="[_:publisher]" rel="dct:publisher">
<span property="dct:title">yjf_victor</span></span>
has waived all copyright and related or neighboring rights to
<span property="dct:title">get_physical_mac</span>.
</p>
