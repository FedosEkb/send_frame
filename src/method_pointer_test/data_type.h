#ifndef DATA_TYPE
#define DATA_TYPE

#include <stdint.h>

enum class devise_type
{
    NOPE = 0,
    AMPERMETER = 1,
    VOLTMETER = 2
};

struct dev_one
{
    uint32_t current_curse;
    double current_fine;
};

struct dev_two
{
    uint32_t voltage1_curse;
    double voltage1_fine;
    uint32_t voltage2_curse;
    double voltage2_fine;
};

#endif /* DATA_TYPE */