#ifndef _Callback_h_
#define _callback_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>


namespace soma
{


class Callback
{

  public:

    Callback();
    virtual ~Callback();

    virtual void execute( int32_t );

};


}


#endif
