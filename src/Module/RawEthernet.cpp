#include "RawEthernet.h"


/// @brief create a raw socket io interface
/// @param interface_name to detect name print "ip addr" in shell
RawEthernet::RawEthernet(const char *__restrict interface_name)
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
