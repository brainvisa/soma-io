#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ColorLut.h>
#else
#include <Dicom/ColorLut.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcxfer.h>


dcm::ColorLut::ColorLut()
             : _count( 0 ),
               _first( 0 ),
               _bits( 16 ),
               _lut( 0 )
{
}


dcm::ColorLut::~ColorLut()
{
}


void dcm::ColorLut::initialize( DcmItem* item, 
                                const DcmTagKey& descriptor, 
                                const DcmTagKey& data )
{

  if ( item )
  {

    Uint16 tmpUShort;
    Sint16 tmpShort;

    if ( item->findAndGetUint16( descriptor, tmpUShort, 0 ).good() )
    {

      _count = uint32_t( tmpUShort );

    }
    else
    {

      if ( item->findAndGetSint16( descriptor, tmpShort, 0 ).good() )
      {

        _count = uint32_t( OFstatic_cast( Uint16, tmpShort ) );

      }

    }

    if ( item->findAndGetUint16( descriptor, tmpUShort, 1 ).good() )
    {

      _first = int32_t( tmpUShort );

    }
    else
    {

      if ( item->findAndGetSint16( descriptor, tmpShort, 1 ).good() )
      {

        _first = int32_t( OFstatic_cast( Uint16, tmpShort ) );

      }

    }

    if ( item->findAndGetUint16( descriptor, tmpUShort, 2 ).good() )
    {

      _bits = int32_t( tmpUShort );

    }
    else
    {

      if ( item->findAndGetSint16( descriptor, tmpShort, 2 ).good() )
      {

        _bits = int32_t( OFstatic_cast( Uint16, tmpShort ) );

      }

    }

    unsigned long lutCount = 0;

    if ( item->findAndGetUint16Array( data, 
                                      (const Uint16*&)_lut, 
                                      &lutCount ).good() )
    {

      if ( _lut && ( lutCount > 0 ) )
      {

        if ( lutCount == ( ( _count + 1 ) >> 1 ) )
        {

          uint16_t* buffer = new uint16_t[ _count ];

          if ( buffer )
          {

            uint8_t* p = (uint8_t*)_lut;
            uint16_t* q = buffer;

            if ( gLocalByteOrder == EBO_BigEndian )
            {

              while ( lutCount-- )
              {

                *q++ = uint16_t( *( p + 1 ) );
                *q++ = uint16_t( *p );
                p += 2;

              }

            }
            else
            {

              int32_t n = _count;

              while ( n-- )
              {

                *q++ = uint16_t( *p++ );

              }

            }

            delete [] _lut;

            _lut = buffer;

          }

        }

      }

    }

  }

}


int32_t dcm::ColorLut::getBits() const
{

  return _bits;

}


uint16_t dcm::ColorLut::operator()( int32_t index )
{

  if ( _lut )
  {

    if ( index < _first )
    {

      return _lut[ 0 ];

    }
    else if ( index > int32_t( _count ) )
    {

      return _lut[ _count - 1 ];

    }

    return _lut[ index - _first ];

  }

  return 0;

}
