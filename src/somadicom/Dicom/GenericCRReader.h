#ifndef _GenericCRReader_h_
#define _GenericCRReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/CRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/CRImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace soma
{


class GenericCRReader : public CRImageStorageReader,
                        public Singleton< GenericCRReader >
{

  protected:

    friend class Singleton< GenericCRReader >;

    GenericCRReader();

};


}


#endif
