#ifndef _GenericCTReader_h_
#define _GenericCTReader_h_


#include <Dicom/CTImageStorageReader.h>
#include <Pattern/Singleton.h>


namespace soma
{


class GenericCTReader : public CTImageStorageReader,
                        public Singleton< GenericCTReader >
{

  protected:

    friend class Singleton< GenericCTReader >;

    GenericCTReader();

};


}


#endif
