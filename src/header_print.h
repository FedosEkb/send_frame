#ifndef HEADER_PRINT
#define HEADER_PRINT


#include <linux/if_ether.h>  /* contain 802.3 protocol numbers and constants, contains definition of ethernet frame header */
#include <netinet/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void print_ethernet_header(struct ethhdr *eth);

void print_ip_header(struct iphdr *ip);

void print_udp_header(struct udphdr *udp);

#endif /* HEADER_PRINT */