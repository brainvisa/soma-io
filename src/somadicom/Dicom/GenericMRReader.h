#ifndef _GenericMRReader_h_
#define _GenericMRReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/MRImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace soma
{


class GenericMRReader : public MRImageStorageReader,
                        public Singleton< GenericMRReader >
{

  protected:

    friend class Singleton< GenericMRReader >;

    GenericMRReader();

};


}


#endif
