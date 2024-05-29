/* cSpell:disable */
#include "test.h"

typedef unsigned int super_uint32_t;

int main()
{

    std::cout << "test hello" << std::endl;
    
#ifdef ETH_TEST
    int rawsocket;
    open_raw_socket_at_interface(INTERFACE_NAME,&rawsocket);
    ether_send(&rawsocket);
    // sleep (1);
    // ether_receive(&rawsocket);
    close_raw_socket_at_interface(&rawsocket);
#endif /* ETH_TEST */

    return 0;
}


