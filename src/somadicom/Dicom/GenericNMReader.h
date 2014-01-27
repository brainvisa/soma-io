#ifndef _GenericNMReader_h_
#define _GenericNMReader_h_


#include <Dicom/NMImageStorageReader.h>
#include <Pattern/Singleton.h>


namespace soma
{


class GenericNMReader : public NMImageStorageReader,
                        public Singleton< GenericNMReader >
{

  protected:

    friend class Singleton< GenericNMReader >;

    GenericNMReader();

};


}


#endif
