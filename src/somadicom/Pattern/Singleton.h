#ifndef _Singleton_h_
#define _Singleton_h_


#include <soma-io/config/soma_config.h>


namespace soma
{


template < class T >
class Singleton
{

  public:

    static T& getInstance()
    { 

      static T instance;
      return instance;

    }

};


}


#endif
