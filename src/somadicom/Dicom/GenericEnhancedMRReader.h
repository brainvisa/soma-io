#ifndef _GenericEnhancedMRReader_h_
#define _GenericEnhancedMRReader_h_


#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


namespace soma
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
