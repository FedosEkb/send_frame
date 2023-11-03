#include "dev_son_t.h"

void dev_son_t::get_some_data(void *ptr_on_data, devise_type *dev_type)
{
    ptr_on_data = &data;
    *dev_type = devise_type::VOLTMETER;
}

dev_son_t::dev_son_t(double voltage_1, double voltage_2)
{
    data.voltage1_curse = static_cast<uint32_t>(voltage_1);
    data.voltage1_fine = voltage_1;
    data.voltage2_curse = static_cast<uint32_t>(voltage_2);
    data.voltage2_fine = voltage_2;
    net.add_devise_to_net(devise_type::VOLTMETER,this,&iBase::get_some_data);

}

dev_son_t::~dev_son_t()
{
}