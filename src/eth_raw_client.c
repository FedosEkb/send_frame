// cSpell:disable

#include "eth_raw_client.h"

#define DATA_SIZE 0x065 





// private  static declaration 


/// @brief send some data from raw ethernet
/// @param  
/// @return error status. 0 == NOT_ERROR
int ether_send(int *rawsocket_in)
{
	// set up the socket, which is simply an integer
	int rawsocket = *rawsocket_in;

	
	//create a packet

	

#ifdef PRINT_DEBUG_INFO
	printf("\n IP address from " INTERFACE_NAME ":\t%s\n", // NOTE we have nothing in first 2 bait!!
		   inet_ntoa(((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr));
#endif /* PRINT_DEBUG_INFO */

	const size_t sum_of_all_headers = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
	const size_t data_size = DATA_SIZE + sum_of_all_headers;

	unsigned char * send_buff = malloc(data_size);
	memset(send_buff,0,data_size);

	/* constructing a Ethernet header  */

	struct ethhdr *ethernet_header = (struct ethhdr *) send_buff; // struct for ether header
	//bzero(&ethernet_header, sizeof(ethernet_header));
	constructing_ethernet_header(ethernet_header, rawsocket_in);

	/* constructing a IP header  */

	struct iphdr *iph = (struct iphdr *)(send_buff + sizeof(struct ethhdr)); 

	constructing_ip_header(iph, rawsocket_in);

	/* constructing a UDP header  */

	struct udphdr *uh = (struct udphdr *)(send_buff + sizeof(struct iphdr) + sizeof(struct ethhdr));

	constructing_udp_header(uh);

	/* filling len of UDP and IP  */

	uh->len = htons((data_size - sizeof(struct iphdr) - sizeof(struct ethhdr))); // UDP length field
	iph->tot_len = htons(data_size - sizeof(struct ethhdr));					 // IP length field

	/* constructing a data load of frame  */

	for (int i = sum_of_all_headers; i < data_size; i++)
	{
		send_buff[i] = i - sum_of_all_headers;
	}

	/* calculating CRC for IP header */

	iph->check = checksum((unsigned short*)(send_buff + sizeof(struct ethhdr)), (sizeof(struct iphdr)/2));

#ifdef PRINT_DEBUG_INFO
	for (int i = 0; i < data_size; i++)
	{
		// sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr)
		switch (i)
		{
		case 0:
			printf("\nEthernet header:\n");
			break;
		case sizeof(struct ethhdr):
			printf("\nIP header:\n");
			break;
		case sizeof(struct ethhdr) + sizeof(struct iphdr):
			printf("\nUDP header:\n");
			break;
		case sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr):
			printf("\nPacket body:\n");
			break;
		default:
			break;
		}
		printf("%02X ", send_buff[i]);
	}
#endif /* PRINT_DEBUG_INFO */
	//end of creating a packet

	/* print all header here */
#ifdef PRINT_SENDING_HEADERS
	print_ethernet_header(ethernet_header);
	print_ip_header(iph);
	print_udp_header(uh);
#endif /* PRINT_DEBUG_INFO */
	//

	//write packet to socket
	if(write(rawsocket, (send_buff), data_size) == -1)
	//if(write(rawsocket, (unsigned char *)(&send_buff), data_size) == -1)
	{
		perror("Error writing bytes to the socket! \n");
		exit(-1);
	}

	// close_raw_socket_at_interface(&rawsocket);

	return 0;
}

/// @brief create a raw socket io interface
/// @param interface_name to detect name print "ip addr" in shell
/// @param rawsocket pointer to integer what will be used as socket
/// @return return status
int open_raw_socket_at_interface(const char *__restrict interface_name, int *rawsocket)
{
	printf("\nOpen socket on interface %s\n", interface_name);
	// format socket(int socket_family, int socket_type, int protocol);
	#ifdef SEND_IP_IN_FRAME
	// if ((*rawsocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) 
	#endif /* SEND_IP_IN_FRAME */
	if ((*rawsocket = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1)
	{
		perror("Error creating raw socket: ");
		exit(-1);
	}
	// end of setting up socket

	// bind interface eth0 to the socket
	struct sockaddr_ll sll;
	struct ifreq ifr;

	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));

	/* First Get the Interface Index  */
	strncpy((char *)ifr.ifr_name, interface_name, IFNAMSIZ-1); 
	if ((ioctl(*rawsocket, SIOCGIFINDEX, &ifr)) == -1)	// NOTE в ifreg ifr возвращается номер (индекс) интерфейса 
	{
		perror("Error getting Interface index !\n");
		exit(-1);
	}

	/* Bind our raw socket to this interface */
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL); // NOTE будем пылесосить все пакеты на этом интерфейс  

	if ((bind(*rawsocket, (struct sockaddr *)&sll, sizeof(sll))) == -1)
	{
		perror("Error binding raw socket to interface\n");
		exit(-1);
	}

	// end of binding interface to socket
	return 0;
}

/// @brief
/// @param rawsocket pointer to socket
/// @return execution status
int close_raw_socket_at_interface(int *rawsocket)
{
	printf("\nClose socket %d\n", *rawsocket);
	// close the socket
	if (close(*rawsocket) == -1)
	{
		perror("Error closing the socket! \n");
		exit(-1);
	}
	return 0;
}

/// @brief 
/// @param  
/// @return 
int ether_receive(int *rawsocket_in){

	// set up the socket, which is simply an integer
	int rawsocket = *rawsocket_in;

	// open_raw_socket_at_interface(INTERFACE_NAME,&rawsocket);

	unsigned char *buffer = (unsigned char *)malloc(65536); // to receive data
	memset(buffer, 0, 65536);
	struct sockaddr saddr;
	int saddr_len = sizeof(saddr);

	// Receive a network packet and copy in to buffer
	size_t buflen = recvfrom(rawsocket, buffer, 65536, 0, &saddr, (socklen_t *)&saddr_len);
	if (buflen < 0)
	{
		printf("error in reading recvfrom function\n");
		return -1;
	}

	printf("\nResivet from socket (%zu bait in frame):\n", buflen);
	// extract ethernet header

	struct ethhdr *eth = (struct ethhdr *)(buffer);

	print_ethernet_header(eth);

	// extract ip header
#ifdef EXTRACT_IP_HEADER  // NOTE it defined in header 

	unsigned short iphdrlen;
	struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));

	print_ip_header(ip);

#ifdef EXTRACT_UDP_HEADER // NOTE it defined in header
	iphdrlen = ip->ihl * 4;
	/* getting pointer to udp header*/
	struct udphdr *udp = (struct udphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));

	print_udp_header(udp);

#endif /* EXTRACT_TCP_HEADER */
#endif /* EXTRACT_IP_HEADER */

	/* extract data from frame*/

	unsigned char *data = (buffer + sizeof(struct ethhdr)
#ifdef EXTRACT_IP_HEADER
						   + iphdrlen

#ifdef EXTRACT_UDP_HEADER
						   + sizeof(struct udphdr)
#endif /* EXTRACT_UDP_HEADER */
#endif /* EXTRACT_IP_HEADER */
	);

	int remaining_data = buflen - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));

	/* print all remaining data*/

	printf("\nRemaining data:\n");

	for (int i = 0; i < remaining_data; i++)
	{
		if (i != 0 && i % 16 == 0)
			printf("\n");
		printf(" %.2X ", data[i]);
	}

	// close_raw_socket_at_interface(&rawsocket);

	return -1; 
}

/// @brief 
/// @param rawsocket_in 
/// @param buffer_addr 
/// @param buffer_len 
/// @param current_buffer_offset 
/// @return 
uint32_t ether_receive_and_save(int *rawsocket_in, uint8_t *buffer_addr, uint32_t buffer_len, uint32_t current_buffer_offset)
{
	// open_raw_socket_at_interface(INTERFACE_NAME,&rawsocket);

	unsigned char *buffer = (unsigned char *)malloc(65536); // to receive data
	if (!buffer)
	{
		printf("error in ether_receive_and_save allocate memory for receive buffer\n");
		return -1;
	}
	memset(buffer, 0, 65536);
	struct sockaddr saddr;
	int saddr_len = sizeof(saddr);

	// Receive a network packet and copy in to buffer
	size_t buflen = recvfrom(*rawsocket_in, buffer, 65536, 0, &saddr, (socklen_t *)&saddr_len);
	if (buflen < 0)
	{
		printf("error in ether_receive_and_save function\n");
		return -1;
	}
	else if (buflen > buffer_len)
	{
		printf("interface have received too many data, more than buffer length (%d)\n", buffer_len);
		return -1;
	}
	if ((buffer_len - 1)  - current_buffer_offset >= buflen) // NOTE данные умещаются в "верхнюю" часть кольцевого буфера  
	{
		memcpy(buffer_addr + current_buffer_offset,buffer,buflen);
		return current_buffer_offset + buflen;  //NOTE return last writing position
	} else
	{
		size_t data_copy_in_first_memcpy = (buffer_len - 1) - current_buffer_offset;
		memcpy(buffer_addr + current_buffer_offset, buffer, data_copy_in_first_memcpy);
		memcpy(buffer_addr, buffer + data_copy_in_first_memcpy, buflen - data_copy_in_first_memcpy);
		return buflen - data_copy_in_first_memcpy; //NOTE return last writing position
	}
	return 0;
}

/// @brief Constructing ethernet header on address of first param
/// @param ether_header address for data formatting
/// @param rawsocket socket buinding  
/// @return error status
int constructing_ethernet_header(struct ethhdr *ethernet_header, int *rawsocket)
{
		/* get MAC address from interface */

	struct ifreq ifreq_mac;  // here will be MAC ad 
	memset(&ifreq_mac, 0, sizeof(ifreq_mac));
	strncpy(ifreq_mac.ifr_name,INTERFACE_NAME, IFNAMSIZ - 1); // giving name of Interface

	if ((ioctl(*rawsocket, SIOCGIFHWADDR, &ifreq_mac)) < 0)
	{ // getting MAC Address
		printf("error in SIOCGIFHWADDR ioctl reading");
	}

#ifdef PRINT_DEBUG_INFO
	printf("\n MAC address from " INTERFACE_NAME ":\t%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
		   ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[0] & 0xff, ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[1] & 0xff,
		   ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[2] & 0xff, ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[3] & 0xff,
		   ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[4] & 0xff, ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data[5] & 0xff);
#endif /* PRINT_DEBUG_INFO */

	/* constructing a Ethernet header  */

	

#ifdef SEND_TO_LOOPBACK
	memcpy(ethernet_header->h_dest, (void *)(ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data), ETH_ALEN);
#else
	ethernet_header->h_dest[0] = DESTMAC0;
	ethernet_header->h_dest[1] = DESTMAC1;
	ethernet_header->h_dest[2] = DESTMAC2;
	ethernet_header->h_dest[3] = DESTMAC3;
	ethernet_header->h_dest[4] = DESTMAC4;
	ethernet_header->h_dest[5] = DESTMAC5;
#endif /* SEND_TO_LOOPBACK */
	memcpy(ethernet_header->h_source, (void *)(ifreq_mac.ifr_ifru.ifru_hwaddr.sa_data), ETH_ALEN);
	ethernet_header->h_proto = htons(ETH_P_IP); // means next header will be IP header
    return 0;
}

/// @brief creating Ethernet header on address iph.
/// @param iph pointer to header address
/// @param rawsocket_in Sockt from which will be taken a source address 
/// @return status
int constructing_ip_header(struct iphdr *iph, int *rawsocket_in)
{
	struct ifreq ifreq_ip;
	memset(&ifreq_ip, 0, sizeof(ifreq_ip));
	strncpy(ifreq_ip.ifr_name, INTERFACE_NAME, IFNAMSIZ - 1); // giving name of Interface
	if (ioctl(*rawsocket_in, SIOCGIFADDR, &ifreq_ip) < 0)		  // getting IP Address
	{
		printf("error in SIOCGIFADDR \n");
	}
	
	iph->ihl = 5;  				// NOTE  internet header lenght
	iph->version = 4;			// NOTE  version of protocol
	iph->tos = 16;				// NOTE  DSCP − Differentiated Services Code Point; this is Type of Service.
									//	+ ECN − Explicit Congestion Notification; 
									//It carries information about the congestion seen in the route.

	iph->id = htons(10201); 	// NOTE Identification − If IP packet is fragmented during the transmission,
								// all the fragments contain same identification number.
								// to identify original IP packet they belong to.
	// iph->frag_off = ? this fild for fragmentations purpose. it's don't need.  	
	iph->ttl = 64;				// NOTE Time to live
	iph->protocol = 17; 		// NOTE Protocol − Tells the Network layer at the destination host,
								// to which Protocol this packet belongs to, i.e. the next level Protocol.
								// For example protocol number of ICMP is 1, TCP is 6 and UDP is 17.

	iph->saddr = inet_addr(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr))); //NOTE sourse addres
#ifdef SEND_TO_LOOPBACK
	iph->daddr = inet_addr(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr))); 
#else
	iph->daddr = inet_addr("192.168.0.2"); // NOTE put destination IP address
#endif /* SEND_TO_LOOPBACK */
    return 0;
}

int constructing_udp_header(struct udphdr *uh)
{
	uh->source = htons(23451);
	uh->dest = htons(23452);
	uh->check = 0;
	return 0;
}
