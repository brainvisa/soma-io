#ifndef _GenericUSReader_h_
#define _GenericUSReader_h_


#include <soma-io/Dicom/USImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


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
