#ifndef _GenericSCReader_h_
#define _GenericSCReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/SCImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/SCImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif

namespace soma
{


class GenericSCReader : public SCImageStorageReader,
                        public Singleton< GenericSCReader >
{

  protected:

    friend class Singleton< GenericSCReader >;

    GenericSCReader();

};


}


#endif
