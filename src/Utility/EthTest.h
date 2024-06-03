/* Brief класс теста Ethernet интерфейса*/
#ifndef _ETH_TEST_H
#define _ETH_TEST_H

#include <stdint.h>

    enum class TYPE_t : uint16_t
    {
	    REQ_FRAME = 0xF000,           //!< тип кадра - тестовый кадр запроса.
        REQ_FRAME = 0xF001,           //!< тип кадра - тестовый кадр подтверждения.
	    TETS_FRAME = 0xE000           //!< тип кадра - тестовый кадр с данными.
	};

    enum class BODY_TYPE_t : uint16_t
    {
	    INC_VAL = 0x01,                 //!< Заполнять тело ариф. прогрессией с шагом 1 начзначение 1.
        RAND_VAL = 0x02                 //!< Заполнять тело случайными значениями.
	};

    enum class REQ_TYPE_t : uint8_t
    {
	    SAME_LEN = 0x01,                //!< запрос на N кадров длинны L.
        INC_LEN = 0x02,                 //!< запрос на N кадров длинны [L...L+(N-1)*inc_val].
	    DEC_LEN = 0x03,                 //!< запрос на N кадров длинны [L...L-(N-1)*inc_val].
        SET_NEW_BODY_PADING = 0x80,     //!< запрос в поле amount будет указан тип заполения тела тестового кадра
    };

namespace EthTest{
    struct _type
    {
        uint16_t direction      :1;
        uint16_t wait           :1;
        uint16_t reserv         :6;  
        REQ_TYPE_t type         :8;  
    };

    struct req_body
    {
        _type type;         
        uint16_t amount;        // количество тестовых кадров
        uint16_t len;           // длинна тестовых кадров
        uint16_t inc_val;       // шаг длинны тестовых кадров
        uint32_t waint_us;      // ожидание между отправкой тестовых кадров (в мкс)
    };
    


}

#endif /* _ETH_TEST_H */