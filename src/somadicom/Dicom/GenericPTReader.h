#ifndef _GenericPTReader_h_
#define _GenericPTReader_h_


#include <Dicom/PTImageStorageReader.h>
#include <Pattern/Singleton.h>


namespace soma
{


class GenericPTReader : public PTImageStorageReader,
                        public Singleton< GenericPTReader >
{

  protected:

    friend class Singleton< GenericPTReader >;

    GenericPTReader();

};


}


#endif
