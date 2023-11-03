#ifndef NET_SIM
#define NET_SIM
#include "data_type.h"
#include <assert.h>
#include <iostream>

#define VOLTAGE_TYPE 1
#define CURRENT_TYPE 2
#define MAX_DEV_NUMB 32 

class iBase;

typedef void (iBase::*get_some_data)(void* ptr_on_data, devise_type *dev_type);

struct devise_on_net
{
    devise_type dev_arr[MAX_DEV_NUMB];
    get_some_data get_data[MAX_DEV_NUMB];
    iBase* devise_ptr[MAX_DEV_NUMB];

    devise_on_net() : dev_arr{devise_type::NOPE},
                      get_data{nullptr},
                      devise_ptr{nullptr}
    {
    }
};


class net_sim
{
private:
    static devise_on_net net_devise_table;
    net_sim(){};                       //!< запрещаем конструктор
    net_sim(const net_sim &) = delete; //!< запрещаем конструктор копирования
    static void print_devise_value(const void* ptr_on_data,const devise_type dev_type,const int number_on_net);

public:
    static net_sim &get() //!< Доступ к синглтону
    {
        static net_sim inst;
        return inst;
    }

    static uint32_t add_devise_to_net(devise_type type_to_add, iBase *devise_ptr, get_some_data get_data_callback);
    static void print_all_devise_value(void);

    ~net_sim();
};

#endif /* NET_SIM */

