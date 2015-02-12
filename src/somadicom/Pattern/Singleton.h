#ifndef _Singleton_h_
#define _Singleton_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif


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
