#ifndef _GenericUSReader_h_
#define _GenericUSReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/USImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/USImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif

namespace dcm
{


class GenericUSReader : public USImageStorageReader,
                        public Singleton< GenericUSReader >
{

  protected:

    friend class Singleton< GenericUSReader >;

    GenericUSReader();

};


}


#endif
