#ifndef _GenericUSMultiframeReader_h_
#define _GenericUSMultiframeReader_h_


#include <Dicom/USMultiframeImageStorageReader.h>
#include <Pattern/Singleton.h>


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
