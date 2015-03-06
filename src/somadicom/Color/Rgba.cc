#ifdef SOMA_IO_DICOM
#include <soma-io/Color/Rgba.h>
#else
#include <Color/Rgba.h>
#endif


soma::Rgba::Rgba( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
          : _red( r ),
            _green( g ),
            _blue( b ),
            _alpha( a )
{
}


soma::Rgba::Rgba( const Rgba& other )
          : _red( other._red ),
            _green( other._green ),
            _blue( other._blue ),
            _alpha( other._alpha )
{
}


soma::Rgba::~Rgba()
{
}
