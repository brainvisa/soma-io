#ifndef _GenericPTReader_h_
#define _GenericPTReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/PTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/PTImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace dcm
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
