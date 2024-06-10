//"cSpell.language": "en,ru",

/*
 * MsEth.h
 *
 *  Created on: 3 апр. 2023 г.
 *      Author: ChirkovaNS
 */

#ifndef MS_ETH_MODULE
#define MS_ETH_MODULE

/* --------------------- Include section --------------------- */
#include <stdint.h>
#include <iostream>
#include "../eth_raw_client.h"
/* --------------------- Include section end --------------------- */
#define MAX_ETHER_FRAME_TYPE 20 //!< максимальное количество типов кадров . 
// type_length_pairs20

 
//! \brief ID для модулей канала.  NOTE its here for backward compatibility !!  (crutch)
	enum modules : uint8_t
	{
		muo  	= 0,	//!< ID МУО.
		msv  	= 1,	//!< ID МСВ.
		mpzu 	= 2,	//!< ID МПЗУ.
		mtv  	= 3,	//!< ID МТВ.
		msGost	= 4,	//!< ID МС ГОСТ.
		msEth   = 5,    //!< ID МС Ethernet.
	};


class MsEthModule /* : public MsModule */ // TODO check its need for some function/
{
public:
    MsEthModule(modules eth_module, uint8_t eth_priority, uint32_t eth_buffer_addr, uint32_t eth_buffer_len, uint64_t mac_address);
    ~MsEthModule()
    {
        close_raw_socket_at_interface(rawsocket);
        delete[] rxBufferAddr;
        delete[] rawsocket;
    }

    //! \brief Поля регистра MS_Eth_Cfg.
    union config_mode_t {
      uint16_t full_word;
      struct
      {
          uint16_t rate         : 1 ; //!< Cкорость обмена (0/1 - 100Мбит/10Мбит)
          uint16_t autonegation : 1 ; //!< Автоподстройка скорости (0/1 - выкл/вкл)
          uint16_t mode_exch    : 1 ; //!< Режим обмена (0/1 - полнодуплексный/полудуплекс)
          uint16_t pream_enable : 1 ; //!< Передача преамбулы (0/1 - вкл/выкл)
          uint16_t ind_adr      : 1 ; //!< Разрешение приёма кадров с индивид. MAC-адресами (0/1 - разрешен/запрещен)
          uint16_t group_adr    : 1 ; //!< Разрешение приёма кадров с групп. MAC-адресами (0/1 - разрешен/запрещен)
          uint16_t broad_adr    : 1 ; //!< Разрешение приёма кадров с широковещ. MAC-адресами (0/1 - разрешен/запрещен)
          uint16_t tx_enable    : 1 ; //!< Разрешение работы приёмника (0/1 - разрешен/запрещен)
          uint16_t rx_enable    : 1 ; //!< Разрешение работы передатчика (0/1 - разрешен/запрещен)
          uint16_t pad_enable   : 1 ; //!< Включение автозаполнения (PAD) (0/1 - выкл/вкл)
          uint16_t idle_enable  : 1 ; //!< Включение циклической посылки Idle (0/1 - вкл/выкл)
          uint16_t rx_sa_enable : 1 ; //!< Разрешение приёма от всех абонентов (0/1 - разрешен/только от MAC-адр в MS_ExternAddr)
          uint16_t phy_rst      : 1 ; //!< Сигнал обнуления на схему физического уровня (0/1 - выставлен/снят)
          uint16_t reserve      : 2 ; //!< Резерв, заполняется "0"
          uint16_t mode_mii_spi : 1 ; //!< Выбор интерфейса для передачи данных (0/1 - MII/SPI)
      } param;
    };

    //! \brief Поля регистра MS_Eth_LT.
    union length_type_t {
      uint32_t full_word;
      struct
      {
          uint32_t mode         : 8;    //!< Режим заполнения поля (0 - факт.объем данных, 1 - значение из "value")
          uint32_t value        : 16;   //!< Значение поля при "mode" = 0x01
          uint32_t reserve      : 8;    //!< Резерв, заполняется "0"
      } param;
    };

    //! \brief Частные регистры МС Ethernet доступные на запись и чтение.
    struct eth_registers_t
    {
        config_mode_t MS_Eth_Cfg;   //!< 0h W/R Режим работы МС-Е
        uint64_t MS_Eth_SA;         //!< 8h W/R Собственный MAC-адрес МСE
        uint16_t MS_Eth_IPG;        //!< 10h W/R Регистр технологической паузы между кадрами в дискретах IPG
        uint32_t MS_Eth_IdleCycle;  //!< 14h W/R Цикл посылки Idle, в тактах СнК, д.б. не менее IPG
        uint32_t MS_Eth_IdleFault;  //!< 18h W/R Интервал, по истечению ктр выставляется сбой по отсутствию Idle
        uint16_t MS_Eth_Dbg;        //!< 1Ch W/R Регистр отладки
        length_type_t MS_Eth_LT;    //!< 20h W/R Заполнение поля Length/Type кадра Ethernet
        uint16_t MS_Eth_spi_mode;   //!< 24h W/R Регистр управления МСЕ в режиме SPI

        eth_registers_t() : MS_Eth_SA(0),
                            MS_Eth_IPG(0),
                            MS_Eth_IdleCycle(0),
                            MS_Eth_IdleFault(0),
                            MS_Eth_Dbg(0),
                            MS_Eth_spi_mode(0)
                            {
                             MS_Eth_Cfg.full_word = 0;
                             MS_Eth_LT.full_word = 0;
                            }
    };

    //! \brief Частные регистры МС Ethernet доступные только на чтение.
    struct eth_state_registers_t
    {
        uint32_t MS_Eth_SumTxByte;  //!< 0h R Счётчик байт, выданных во внешний интерфейс от подачи питания
        uint32_t MS_Eth_SumTxFrame; //!< 4h R Счётчик кадров, выданных во внешний интерфейс от подачи питания
        uint32_t MS_Eth_SumRxByte;  //!< 8h R Счётчик байт, принятых по КЛС от подачи питания
        uint32_t MS_Eth_SumRxFrame; //!< Ch R Счётчик кадров, принятых по КЛС от подачи питания
        uint16_t MS_Eth_State;      //!< 10h R Регистр состояния МСE в режиме Ethernet
        uint16_t MS_Eth_Fault;      //!< 12h R Регистр сбоев
        uint8_t MS_Eth_spi_status;  //!< 14h R Регистр статуса МСЕ в режиме SPI
    };


    // //! \brief Адреса частных регистров МС Ethernet. 
    // enum
    // {
    //     base_regs_addr          = 0x75002080,
    //     ADDR_MS_Eth_SumTxByte   = base_regs_addr,
    //     ADDR_MS_Eth_SumTxFrame  = base_regs_addr + 0x4,
    //     ADDR_MS_Eth_SumRxByte   = base_regs_addr + 0x8,
    //     ADDR_MS_Eth_SumRxFrame  = base_regs_addr + 0xC,
    //     ADDR_MS_Eth_State       = base_regs_addr + 0x10,
    //     ADDR_MS_Eth_Fault       = base_regs_addr + 0x12,
    //     ADDR_MS_Eth_spi_status  = base_regs_addr + 0x14,
    //     ADDR_MS_Eth_SAgroup     = base_regs_addr + 0x18,
    //     ADDR_MS_Eth_Cfg         = base_regs_addr + 0x180,
    //     ADDR_MS_Eth_SA          = base_regs_addr + 0x188,
    //     ADDR_MS_Eth_IPG         = base_regs_addr + 0x190,
    //     ADDR_MS_Eth_IdleCycle   = base_regs_addr + 0x194,
    //     ADDR_MS_Eth_IdleFault   = base_regs_addr + 0x198,
    //     ADDR_MS_Eth_Dbg         = base_regs_addr + 0x19C,
    //     ADDR_MS_Eth_LT          = base_regs_addr + 0x1A0,
    //     ADDR_MS_Eth_spi_mode    = base_regs_addr + 0x1A4,
    // };

    //! \brief Кодировки для заполнения полей регистра MS_Eth_Dbg.
    enum
    {
        LOOPBACK_ENABLE = 0b001,        //!< Режим замыкания передатчика на приёмник включен
        LOOPBACK_DISABLE = 0b000,       //!< Режим замыкания передатчика на приёмник выключен
        STANDART_MODE = 0b000,          //!< Штатный режим работы (разрешение приёма/выдачи)
        STOP_TX_RX_IMMEDIATELY = 0b010, //!< Останов приёмника и передатчика сразу по получению данной кодировки
        STOP_TX_RX_AFTER_EXCH = 0b100,  //!< Останов приёмника и передатчика по завершению приёма/передачи кадра
    };

    //! \brief Кодировки для заполнения поля регистра MS_Eth_LT.
    enum : uint8_t
    {
        SET_ETH_LT_WITH_FRAME_TYPE = 0x01,        //!< Заполнение поля Length/Type типом кадра.
        SET_ETH_LT_WITH_FRAME_SIZE = 0x00,        //!< Заполнение поля Length/Type размером кадра.

    };

    //! \brief Соответствие типа пакета Ethernet (поле Length/Type) его длине.
    struct type_length_pairs_t
    {
        uint16_t type;      //!< Тип пакета.
        uint16_t length;    //!< Длина пакета.
    };

    void DefaultConfigEthernetRegisters();
    void SetLengthTypeFields(uint8_t mode_field, uint16_t length_type_field = 0);

    void SetPacketLengthCorrespondingType(uint16_t packet_type, uint16_t packet_length);
    uint16_t GetPacketLengthCorrespondingType(uint16_t packet_type);
    uint32_t GetReceivedDataAddress(uint64_t mac_addr_extern, uint32_t current_buffer_position);

    bool CompareMemory(uint64_t first_addr, uint64_t second_addr, uint32_t length_in_byte);

    uint64_t GetSelfMacAddress()    {return self_mac_addr;}

private:
    uint64_t self_mac_addr;
    type_length_pairs_t type_length_pairs[MAX_ETHER_FRAME_TYPE];  //!< Соответствия типа пакета его длине.
    uint32_t type_length_pairs_counter;
    uint8_t *rxBufferAddr;    
    uint32_t rxBufferLength;
    int *rawsocket;  
    uint32_t received_buffer_position;
};


#endif /* MS_ETH_MODULE */
