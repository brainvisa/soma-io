#ifndef _GenericCTReader_h_
#define _GenericCTReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/CTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


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
