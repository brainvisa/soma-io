#ifndef _GenericMRReader_h_
#define _GenericMRReader_h_


#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


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
