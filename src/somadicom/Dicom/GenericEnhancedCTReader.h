#ifndef _GenericEnhancedCTReader_h_
#define _GenericEnhancedCTReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/EnhancedCTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/EnhancedCTImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace dcm
{


class GenericEnhancedCTReader : public EnhancedCTImageStorageReader,
                                public Singleton< GenericEnhancedCTReader >
{

  protected:

    friend class Singleton< GenericEnhancedCTReader >;

    GenericEnhancedCTReader();

};


}


#endif
