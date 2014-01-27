#ifndef _Callback_h_
#define _callback_h_


#include <Utils/StdInt.h>


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
