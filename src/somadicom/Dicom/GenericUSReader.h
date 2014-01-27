#ifndef _GenericUSReader_h_
#define _GenericUSReader_h_


#include <Dicom/USImageStorageReader.h>
#include <Pattern/Singleton.h>


namespace soma
{


class GenericUSReader : public USImageStorageReader,
                        public Singleton< GenericUSReader >
{

  protected:

    friend class Singleton< GenericUSReader >;

    GenericUSReader();

};


}


#endif
