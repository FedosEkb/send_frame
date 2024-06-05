#include "RawEthernet.h"


/// @brief create a raw socket io interface
/// @param interface_name to define name print "ip addr" in shell
RawEthernet::RawEthernet(const char *__restrict interface_name):
	interface_name(interface_name)
{   
    printf("\nOpen socket on interface %s\n", interface_name);
	
	if ((rawsocket = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1)
	{
		perror("Error creating raw socket: ");
		exit(-1);
	}
	// end of setting up socket

	// bind interface eth0 to the socket
	struct sockaddr_ll sll;
	struct ifreq ifr;

    memset(&sll, 0, sizeof(sll));
    memset(&ifr, 0, sizeof(ifr));
    
	/* First Get the Interface Index  */
	strncpy((char *)ifr.ifr_name, interface_name, IFNAMSIZ-1); 
	if ((ioctl(rawsocket, SIOCGIFINDEX, &ifr)) == -1)	// NOTE в ifreg ifr возвращается номер (индекс) интерфейса 
	{
		perror("Error getting Interface index !\n");
		exit(-1);
	}

	/* Bind our raw socket to this interface */
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL); // NOTE будем пылесосить все пакеты на этом интерфейс  

	if ((bind(rawsocket, (struct sockaddr *)&sll, sizeof(sll))) == -1)
	{
		perror("Error binding raw socket to interface\n");
		exit(-1);
	}
    // end of binding interface to socket
}

RawEthernet::~RawEthernet()
{
	printf("\nClose socket %d\n", rawsocket);
	// close the socket
	if (close(rawsocket) == -1)
	{
		perror("Error closing the socket! \n");
		exit(-1);
	}
}

/// @brief Constructing ethernet header on address of first param
/// @param ether_header address for data formatting
/// @return error status
int RawEthernet::constructing_ethernet_header(struct ethhdr *const ethernet_header, const uint64_t dest_addr, const uint16_t data_len)
{
		/* get MAC address from interface */
	struct ifreq ifreq_mac;  // here will be MAC 
	memset(&ifreq_mac, 0, sizeof(ifreq_mac));
	strncpy(ifreq_mac.ifr_name, interface_name.c_str(), IFNAMSIZ - 1); // giving name of Interface

	if ((ioctl(rawsocket, SIOCGIFHWADDR, &ifreq_mac)) < 0)
	{ // getting MAC Address
		printf("error in SIOCGIFHWADDR ioctl reading");
	}

#ifdef PRINT_DEBUG_INFO_RAW_ETHERNET
	printf("\n MAC address from %s :\t%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
		   interface_name.c_str(),
		   ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[0] & 0xff, ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[1] & 0xff,
		   ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[2] & 0xff, ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[3] & 0xff,
		   ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[4] & 0xff, ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[5] & 0xff);
#endif /* PRINT_DEBUG_INFO */

	/* constructing a Ethernet header  */

	

#ifdef SEND_TO_LOOPBACK
	memcpy(ethernet_header->h_dest, (void *)(ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data), ETH_ALEN);
#else
	uint64_t dest_addr_MAC_net_order = htonMAC(dest_addr);
	memcpy(ethernet_header->h_dest, &dest_addr_MAC_net_order, ETH_ALEN); // TODO проверить корректность работы!
	// ethernet_header->h_dest[0] = 12; 
	// ethernet_header->h_dest[1] = 0;
	// ethernet_header->h_dest[2] = 0;
	// ethernet_header->h_dest[3] = 0;
	// ethernet_header->h_dest[4] = 0;
	// ethernet_header->h_dest[5] = 0;
#endif /* SEND_TO_LOOPBACK */
	memcpy(ethernet_header->h_source, (void *)(ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data), ETH_ALEN);

	// #define DATA_SIZE (0x600 - 14 - 4 - 100) 
	ethernet_header->h_proto = htons(data_len); // amount of data in body
    return 0;
}
