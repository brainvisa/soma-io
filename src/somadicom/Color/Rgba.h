#ifndef _Rgba_h_
#define _Rgba_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif


namespace soma
{


class Rgba
{

  public:

    Rgba( uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255 );
    Rgba( const Rgba& other );
    virtual ~Rgba();

    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
    uint8_t _alpha;

};


}


#endif
