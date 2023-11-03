#ifndef DEV_SON_T
#define DEV_SON_T

#include "iBase.h"

class dev_son_t:public iBase
{
private:
    dev_two data;
public:
    void get_some_data(void* ptr_on_data, devise_type *dev_type) override;
    dev_son_t(double voltage_1,double voltage_2);
    ~dev_son_t();
};

#endif /* DEV_SON_T */
