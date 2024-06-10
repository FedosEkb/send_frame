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
/// @param dest_addr MAC addr in host order
/// @param data_len value of field type/len in frame header
/// @return error status
int RawEthernet::constructing_ethernet_header(struct ethhdr *const ethernet_header, const uint64_t dest_addr, const uint16_t data_len) const
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

/// @brief Send ethernate frame on intnerface
/// @param dest_addr destination MAC in host order
/// @param body_data_ptr pointer to body data 
/// @param data_len pointer fo body data  
/// @param data_type value of field type/len in frame header
/// @return error status
int RawEthernet::send_ethernet_frame(const uint64_t dest_addr, const uint8_t * const body_data_ptr, const uint16_t data_len , uint16_t data_type/*  = 0 */) const {

	const size_t sum_of_all_headers = sizeof(struct ethhdr); 
	const size_t data_size = data_len + sum_of_all_headers;

	uint8_t * send_buff = static_cast<uint8_t *>(malloc(data_size)); // TODO заменить на new
	memset(send_buff,0,data_size);

	data_type = data_type ? data_type : data_len; // если поле type не определлено, устанавлеваем в него длинну тела кадра.

	constructing_ethernet_header(reinterpret_cast<ethhdr * const>(send_buff),dest_addr,data_type);

	memcpy(send_buff + sum_of_all_headers,body_data_ptr,data_len); // TODO проверить что сложение с void* дает корректный результат


#ifdef PRINT_DEBUG_INFO_RAW_ETHERNET
	for (int i = 0; i < data_size; i++)
	{
		switch (i)
		{
		case 0:
			printf("\nEthernet header:\n");
			break;
		case sizeof(struct ethhdr) :
			printf("\nPacket body:\n");
			break;
		default:
			break;
		}
		printf("%02X ", send_buff[i]);
	}
#endif /* PRINT_DEBUG_INFO_RAW_ETHERNET */

	//write frame to socket
	if(write(rawsocket, send_buff, data_size) == -1)
	{
		perror("Error writing bytes to the socket! \n");
		exit(-1);
	}
	free(send_buff); // TODO заменить на delete
	return 0;
}

/// @brief Resive frame from interface
/// @param receive_buff buffer for data for imput dtat
/// @param buff_len data len
/// @return error status or amount of received byte
int RawEthernet::receive_ethernet_frame(uint8_t * const receive_buff, const size_t buff_len) const {
	struct sockaddr saddr;
	int saddr_len = sizeof(saddr);

	// Receive a network packet and copy in to buffer
	size_t buflen = recvfrom(rawsocket, receive_buff, buff_len, 0, &saddr, (socklen_t *)&saddr_len);
	if (buflen < 0)
	{
		printf("error in reading recvfrom function\n");
		return -1;
	}
	return buflen;
}

