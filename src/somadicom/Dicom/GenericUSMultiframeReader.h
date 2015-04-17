#ifndef _GenericUSMultiframeReader_h_
#define _GenericUSMultiframeReader_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/USMultiframeImageStorageReader.h>
#include <Pattern/Singleton.h>
#endif


namespace dcm
{


class GenericUSMultiframeReader : public USMultiframeImageStorageReader,
                                  public Singleton< GenericUSMultiframeReader >
{

  protected:

    friend class Singleton< GenericUSMultiframeReader >;

    GenericUSMultiframeReader();

};


}


#endif
