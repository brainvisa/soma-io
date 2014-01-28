#ifndef _GenericUSMultiframeReader_h_
#define _GenericUSMultiframeReader_h_


#include <soma-io/Dicom/USMultiframeImageStorageReader.h>
#include <soma-io/Pattern/Singleton.h>


namespace soma
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
