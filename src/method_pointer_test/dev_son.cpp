#include "dev_son.h"

void dev_son::get_some_data(void *ptr_on_data, devise_type *dev_type)
{
    ptr_on_data = &data;
    *dev_type = devise_type::VOLTMETER;
}

dev_son::dev_son(double current)
{
    data.current_curse = static_cast<uint32_t>(current);
    data.current_fine = current;
}


dev_son::~dev_son()
{
}