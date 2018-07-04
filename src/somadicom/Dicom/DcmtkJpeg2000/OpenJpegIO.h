#ifndef _DcmtkJpeg2000_OpenJpegIO_h_
#define _DcmtkJpeg2000_OpenJpegIO_h_


#include <soma-io/Utils/StdInt.h>

#include <openjpeg.h>


namespace dcm
{


class OpenJpegIO
{

  public:

    OpenJpegIO();
    virtual ~OpenJpegIO();

    void* getImage( uint8_t* buffer, int32_t length );
    bool decode( uint8_t* buffer, void* imageIn );

  private:

    OPJ_CODEC_FORMAT _format;

};


}


#endif
