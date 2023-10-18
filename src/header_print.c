#include "header_print.h"
#include "eth_raw_client.h"


/// @brief 
/// @param  
void print_ethernet_header(struct ethhdr *eth)
{
	printf("\nEthernet Header:\n\n");
	printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0],
		   eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
		   eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	printf("\t|-Protocol : %d\n", eth->h_proto);
}

/// @brief 
/// @param ip
void print_ip_header(struct iphdr *ip)
{
	printf("\nIP Header:\n\n");
	printf("\t|-Version : %d\n", (unsigned int)ip->version);

	printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n", (unsigned int)ip->ihl,
		   ((unsigned int)(ip->ihl)) * 4);

	printf("\t|-Type Of Service : %d\n", (unsigned int)ip->tos);

	printf("\t|-Total Length : %d Bytes\n", ntohs(ip->tot_len));

	printf("\t|-Identification : %d\n", ntohs(ip->id));

	printf("\t|-Time To Live : %d\n", (unsigned int)ip->ttl);

	printf("\t|-Protocol : %d\n", (unsigned int)ip->protocol);

	printf("\t|-Header Checksum : %d\n", ntohs(ip->check));
	struct sockaddr_in source;
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = ip->saddr;
	printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));

	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = ip->daddr;
	printf("\t|-Destination IP : %s\n", inet_ntoa(source.sin_addr));
}

/// @brief 
/// @param udp 
void print_udp_header(struct udphdr *udp)
{
	printf("\nUDP Header:\n\n");
	printf("\t|-Source Port : %d\n", ntohs(udp->source));
	printf("\t|-Destination Port : %d\n", ntohs(udp->dest));
	printf("\t|-UDP Length : %d\n", ntohs(udp->len));
	printf("\t|-UDP Checksum : %d\n", ntohs(udp->check));
}

/// @brief calculate CRC16 for IP
/// @param buff pointer to buffer for which need to calculate CRC 
/// @param _16bitword buffer len
/// @return calculated CRC 16
unsigned short checksum(unsigned short *buff, int _16bitword)
{
	unsigned long sum;
	for (sum = 0; _16bitword > 0; _16bitword--)
		sum += htons(*(buff)++);
	sum = ((sum >> 16) + (sum & 0xFFFF));
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}

