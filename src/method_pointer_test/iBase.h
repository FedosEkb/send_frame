#ifndef I_BASE
#define I_BASE

#include "data_type.h"
#include "net_sim.h"

class iBase
{
protected:
    net_sim& net;
public:
    virtual void get_some_data(void *ptr_on_data, devise_type *dev_type) = 0;
    iBase(/* args */);
    ~iBase();
};



#endif /* I_BASE */
