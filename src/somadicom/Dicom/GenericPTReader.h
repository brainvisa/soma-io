#ifndef _GenericPTReader_h_
#define _GenericPTReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


namespace soma
{


class GenericPTReader : public PTImageStorageReader,
                        public Singleton< GenericPTReader >
{

  protected:

    friend class Singleton< GenericPTReader >;

    GenericPTReader();

};


}


#endif
