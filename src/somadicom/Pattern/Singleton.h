#ifndef _Singleton_h_
#define _Singleton_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif


namespace dcm
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


#define ForceSingletonInstanciation( NAME ) \
static dcm::NAME& forced_##NAME##_instanciation = dcm::NAME::getInstance();


#endif
