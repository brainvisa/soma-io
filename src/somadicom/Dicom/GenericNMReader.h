#ifndef _GenericNMReader_h_
#define _GenericNMReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/NMImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/NMImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace dcm
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
