/* cSpell:disable */
#include "test.h"

typedef unsigned int super_uint32_t;

inline uint64_t htonMAC(uint64_t x) { return (((uint64_t)htonl(x)) << 16) + htons((x) >> 32); } __attribute__ ((__const__)) //!  flips the bytes end to end. If you apply it twice, it restores the value to its original state.
inline uint64_t ntohMAC(uint64_t x) { return (((uint64_t)htonl(x)) << 16) + htons((x) >> 32); } __attribute__ ((__const__)) //!  So the same function can be used for ntohll

int main()
{

    std::cout << "test hello" << std::endl;

    uint64_t testMAC = 0x123456789ABC;

    std::cout << "before swap " << std::hex << testMAC << std::endl;
    std::cout << "after swap " << std::hex << htonMAC(testMAC) << std::endl;
    
#ifdef ETH_TEST
    int rawsocket;
    open_raw_socket_at_interface(INTERFACE_NAME,&rawsocket);
    // for (;;)
    // {
    //     ether_send(&rawsocket);
    //     sleep(1);
    // }

    ether_send(&rawsocket);

    // sleep (1);
    // ether_receive(&rawsocket);
    close_raw_socket_at_interface(&rawsocket);
#endif /* ETH_TEST */

    return 0;
}


