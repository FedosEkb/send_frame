/* cSpell:disable */
#include "test.h"

typedef unsigned int super_uint32_t;

inline uint64_t htonMAC(uint64_t x) { return (((uint64_t)htonl(x)) << 16) + htons((x) >> 32); } __attribute__ ((__const__)) //!  flips the bytes end to end. If you apply it twice, it restores the value to its original state.
inline uint64_t ntohMAC(uint64_t x) { return (((uint64_t)htonl(x)) << 16) + htons((x) >> 32); } __attribute__ ((__const__)) //!  So the same function can be used for ntohll


int main()
{

#ifdef ETH_TEST
    // int rawsocket;
    // open_raw_socket_at_interface(INTERFACE_NAME,&rawsocket);

    
    // for (;;)
    // {
    //     ether_send(&rawsocket);
    //     sleep(1);
    // }

    // ether_send(&rawsocket);

    // sleep (1);
    // ether_receive(&rawsocket);
    // close_raw_socket_at_interface(&rawsocket);

    RawEthernet ethenet("lo");
    uint8_t buff[0x600];
    for (size_t i = 0; i < 255; i++)
    {
        buff[i] = i;
    }
    
    ethenet.send_ethernet_frame(0x0,buff,255,0xFFF0);
#endif /* ETH_TEST */
    size_t taked_len = ethenet.receive_ethernet_frame(buff,0x600);
    print_ethernet_header(reinterpret_cast<ethhdr *>(buff));

    return 0;
}


