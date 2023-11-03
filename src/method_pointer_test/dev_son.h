#ifndef DEV_SON
#define DEV_SON

#include "iBase.h"

class dev_son:public iBase
{
private:
    dev_one data;
public:
    void get_some_data(void* ptr_on_data, devise_type *dev_type) override;
    dev_son(double current);
    ~dev_son();
};

#endif /* DEV_SON */
