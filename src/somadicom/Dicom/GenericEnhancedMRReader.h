#ifndef _GenericEnhancedMRReader_h_
#define _GenericEnhancedMRReader_h_


#include <Dicom/EnhancedMRImageStorageReader.h>
#include <Pattern/Singleton.h>


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
