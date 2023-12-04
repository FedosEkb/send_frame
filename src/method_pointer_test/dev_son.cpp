#include "dev_son.h"

void dev_son::get_some_data(void *ptr_on_data, devise_type *dev_type)
{
    *(reinterpret_cast<dev_one*>(ptr_on_data)) = data;
    *dev_type = devise_type::AMPERMETER;
}

dev_son::dev_son(double current)
{
    data.current_curse = static_cast<uint32_t>(current);
    data.current_fine = current;
    net.add_devise_to_net(devise_type::AMPERMETER,this,&iBase::get_some_data);
}

dev_son::~dev_son()
{
}