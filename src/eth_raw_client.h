/* cSpell:disable */

#ifndef ETH_RAW_CLIENT
#define ETH_RAW_CLIENT

/* --------------------- Include section--------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> /* write and close functions */

#include <sys/socket.h> /* contains socket(), bind(), includes SOCK_RAW macro from bits/socket.h */
#include <features.h>
#include <linux/if_packet.h> /* contains sockaddr_ll */
#include <linux/if_ether.h>  /* contain 802.3 protocol numbers and constants, contains definition of ethernet frame header */
#include <errno.h>
#include <sys/ioctl.h> /* contains ifreq, includes SIOCGIFINDEX */
#include <net/if.h>
#include <netinet/ether.h>
// #include<net/ethernet.h> /* contains definition of ethernet address */
#include <netinet/in.h>


#define EXTRACT_IP_HEADER	 // NOTE comment if you want to receive raw ethernet frame 
#ifdef EXTRACT_IP_HEADER
#include <netinet/ip.h>
#include <arpa/inet.h>
#endif /* EXTRACT_IP_HEADER */

#define EXTRACT_UDP_HEADER
#ifdef EXTRACT_UDP_HEADER
#include <linux/udp.h>
#endif /* EXTRACT_TCP_HEADER */

#include "header_print.h"

/* --------------------- Define section--------------------- */

// #define SEND_TO_LOOPBACK // NOTE comment it for translate to NIC // TODO create turn  off/on this macros from script 

#define PRINT_DEBUG_INFO		// TODO create turn  off/on this macros from script 
#define PRINT_SENDING_HEADERS	// TODO create turn  off/on this macros from script 

#ifdef SEND_TO_LOOPBACK
#define INTERFACE_NAME "lo"
#else
// NOTE send broadcast header
#define INTERFACE_NAME "eth0"
#define DESTMAC0 0x00
#define DESTMAC1 0x00
#define DESTMAC2 0x00
#define DESTMAC3 0x00
#define DESTMAC4 0x00
#define DESTMAC5 0x12


#endif /* SEND_TO_LOOPBACK */

/* --------------------- Define section end--------------------- */

/*
  #include<net/ethernet.h> and #include<linux/if_ether.h> have different structures for ethernet, net/ethernet.h contain the address structure

 */
//http://sig9.com/articles/gcc-packed-structures

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int ether_send(int *rawsocket_in);

int ether_receive(int *rawsocket_in);

uint32_t ether_receive_and_save(int *rawsocket_in, uint8_t *buffer_addr,
                                uint32_t buffer_len, uint32_t current_buffer_offset);

int open_raw_socket_at_interface(const char *__restrict interface_name, int *rawsocket);

int close_raw_socket_at_interface(int *rawsocket);

void print_pack(unsigned char *packet, int length);

unsigned short checksum(unsigned short *buff, int _16bitword);

int constructing_ethernet_header(struct ethhdr * ether_header, int *rawsocket);

int constructing_ip_header(struct iphdr *ip_header, int *rawsocket_in);

int constructing_udp_header(struct udphdr *uh);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ETH_RAW_CLIENT */
