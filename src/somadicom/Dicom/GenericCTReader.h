#ifndef _GenericCTReader_h_
#define _GenericCTReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/CTImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/CTImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif

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
