#ifndef _GenericEnhancedMRReader_h_
#define _GenericEnhancedMRReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/EnhancedMRImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace dcm
{


class GenericEnhancedMRReader : public EnhancedMRImageStorageReader,
                                public Singleton< GenericEnhancedMRReader >
{

  protected:

    friend class Singleton< GenericEnhancedMRReader >;

    GenericEnhancedMRReader();

};


}


#endif
