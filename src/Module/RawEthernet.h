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
#include <string>

#define PRINT_DEBUG_INFO_RAW_ETHERNET

class RawEthernet
{
private:
    int rawsocket;                  //! сокет 
    std::string interface_name;     //! название интерфейса

    int constructing_ethernet_header(struct ethhdr *const ethernet_header, const uint64_t dest_addr, const uint16_t data_len) const; 

    inline uint64_t htonMAC(uint64_t x) const { return (((uint64_t)htonl(x)) << 16) + htons((x) >> 32); } __attribute__ ((__const__)) //!  flips the bytes end to end. If you apply it twice, it restores the value to its original state.
    inline uint64_t ntohMAC(uint64_t x) const { return (((uint64_t)htonl(x)) << 16) + htons((x) >> 32); } __attribute__ ((__const__)) //!  So the same function can be used for ntohll

    RawEthernet(const RawEthernet &) = 0; // запрещаем конструктор копирования
    RawEthernet& operator=(const RawEthernet &) = delete; // запрещаем оператор присваевания.

public:
    explicit RawEthernet(const char *__restrict interface_name);
    ~RawEthernet();

    int send_ethernet_frame(const uint64_t dest_addr, const uint8_t * const body_data_ptr, const uint16_t data_len ,  const uint16_t data_type = 0) const; //! выслать данные с интерфейса

    int receive_ethernet_frame(uint8_t * const receive_buff, const size_t buff_len) const; //! Запросить очередной кадр с интерфейса.
    
};


#endif /* _RAW_ETHERNET_H */