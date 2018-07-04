#ifndef _DcmtkJpeg2000_JasperIO_h_
#define _DcmtkJpeg2000_JasperIO_h_


#include <soma-io/Utils/StdInt.h>

#include <jasper/jasper.h>


namespace dcm
{


class JasperIO
{

  public:

    JasperIO();
    virtual ~JasperIO();

    void* getImage( uint8_t* buffer, int32_t length );
    bool decode( uint8_t* buffer, void* imageIn );

  private:

    bool _jasInitialized;

};


}


#endif
