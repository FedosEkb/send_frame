/* cSpell:disable */
#include "test.h"

int main()
{
#ifdef ETH_TEST


    RawEthernet ethenet("lo");
    uint8_t buff[0x600];
    for (size_t i = 0; i < 0x600; i++)
    {
        buff[i] = i;
    }

    std::cout << "\nSend frame:\n";

    

    for (size_t i = 0; (i < 64) && (48 + i*48) < 1500 ; i++)
    {
        ethenet.send_ethernet_frame(0x120000000000, buff, 48 + i*48, 0xF000 + 48 + i*48);
    }
    ethenet.send_ethernet_frame_other_MAC(0x120000000000, 0x120000000100, buff, 46, 0xF000 + 46);
    ethenet.send_ethernet_frame_other_MAC(0x470000000000, 0x120000000000, buff, 46, 0xF000 + 46);
    ethenet.send_ethernet_frame_other_MAC(0x470000000000, 0x120000000000, buff, 46);
    ethenet.send_ethernet_frame_other_MAC(0x470000000000, 0x120000000000, buff, 1000);
    ethenet.send_ethernet_frame_other_MAC(0x470000000000, 0x120000000000, buff, 1500);

    // std::cout << "\nReceive frame:\n";
    // size_t taked_len = ethenet.receive_ethernet_frame(buff,0x600);
    // print_ethernet_header(reinterpret_cast<ethhdr *>(buff));

#endif /* ETH_TEST */

    return 0;
}


