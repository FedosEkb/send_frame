#include "net_sim.h"

devise_on_net net_sim::net_devise_table;


uint32_t net_sim::add_devise_to_net(devise_type type_to_add, iBase* devise_ptr, get_some_data get_data_callback)
{
    uint32_t dev_on_net = MAX_DEV_NUMB;
    for (int iter = 0; iter < MAX_DEV_NUMB; ++iter)
    {
        if (net_devise_table.dev_arr[iter] == devise_type::NOPE)
        {
            net_devise_table.dev_arr[iter] = type_to_add;
            net_devise_table.devise_ptr[iter] = devise_ptr;
            net_devise_table.get_data[iter] = get_data_callback;
            dev_on_net = iter;
            break;
        }
    }
    return ++dev_on_net; 
}

void net_sim::print_all_devise_value(void)
{
    devise_type dev_type = devise_type::NOPE;
    uint8_t buffer_for_data[256];  // TODO память ВЫДЕЛЕНА КОРЯВО!! просто для теста!! не использовать так как есть 
    void * ptr_to_dev_data = buffer_for_data;
    for (int iter = 0; iter < MAX_DEV_NUMB; iter++)
    {
        if (net_devise_table.dev_arr[iter] == devise_type::NOPE ) break;
        
        iBase* current_dev = net_devise_table.devise_ptr[iter];   
        get_some_data current_getter = net_devise_table.get_data[iter]; 
        // TODO можно попробовать запихнуть две предыдущих строки в следующую, но читаемо это будет? 
        (current_dev->*current_getter)(ptr_to_dev_data, &dev_type); //TODO убрать указатель на тип здесь? мы и без этого значем что за тип девайса мы сейчас опрашиваем
        // NOTE да тут такой плохой синтаксис, подразумевается наверно следующее: сначала определяем что это метод нужного нам объекта а затем передаём ему параметры
        print_devise_value(ptr_to_dev_data,dev_type,iter + 1);
    }
    
}
void net_sim::print_devise_value(const void *ptr_on_data,const devise_type dev_type,const int number_on_net)
{
    std::cout << "Devise number: " << number_on_net << std::endl;
    switch (dev_type)
    {
    case devise_type::AMPERMETER:
    {
        const dev_one *ptr_on_ampermeter_data = reinterpret_cast<const dev_one *>(ptr_on_data);
        std::cout << "Devise type: ampermeter\n";
        std::cout << "Course value: " << ptr_on_ampermeter_data->current_curse << std::endl;
        std::cout << "Fine value: " << ptr_on_ampermeter_data->current_curse << std::endl;
        std::cout << std::endl;
    }
    break;
    case devise_type::VOLTMETER:
    {
        const dev_two *ptr_on_voltmeter_data = reinterpret_cast<const dev_two *>(ptr_on_data);
        std::cout << "Devise type: voltmeter\n";
        std::cout << "Course voltage_1: " << ptr_on_voltmeter_data->voltage1_curse << std::endl;
        std::cout << "Fine voltage_1: " << ptr_on_voltmeter_data->voltage1_fine << std::endl;
        std::cout << "Course voltage_2: " << ptr_on_voltmeter_data->voltage2_curse << std::endl;
        std::cout << "Fine voltage_2: " << ptr_on_voltmeter_data->voltage2_fine << std::endl;
        std::cout << std::endl;
    }
    break;
    default:
        assert(false && "Попытка распечатать незарегестрированный прибор!");
        break;
    }
}

// net_sim::net_sim(/* args */)
// {
// }

net_sim::~net_sim()
{
}