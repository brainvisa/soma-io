#ifndef _GenericMRReader_h_
#define _GenericMRReader_h_


#include <Dicom/MRImageStorageReader.h>
#include <Pattern/Singleton.h>


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
