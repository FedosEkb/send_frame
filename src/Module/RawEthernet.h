/* Brief класс для работы с чистым Ethernet*/
#ifndef _RAW_ETHERNET_H
#define _RAW_ETHERNET_H

#include <unistd.h> /* write and close functions */
#include <stdlib.h>
#include <sys/socket.h> /* contains socket(), bind(), includes SOCK_RAW macro from bits/socket.h */
#include <features.h>
#include <linux/if_packet.h> /* contains sockaddr_ll */
#include <linux/if_ether.h>  /* contain 802.3 protocol numbers and constants, contains definition of ethernet frame header */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h> /* contains ifreq, includes SIOCGIFINDEX */
#include <net/if.h>
#include <netinet/ether.h>
// #include<net/ethernet.h> /* contains definition of ethernet address */
#include <netinet/in.h>

class RawEthernet
{
private:
    int rawsocket;
public:
    explicit RawEthernet(const char *__restrict interface_name);
    ~RawEthernet();
};


#endif /* _RAW_ETHERNET_H */