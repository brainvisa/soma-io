#ifdef SOMA_IO_DICOM
#include <soma-io/Container/DicomProxy.h>
#else
#include <Container/DicomProxy.h>
#endif

#include <cstring>
#include <cassert>

#ifdef __SSE2__
#include <immintrin.h>
#endif


#ifdef SOMA_IO_DICOM
soma::DicomProxy::DicomProxy( soma::DataInfo* info )
                : _buffer( 0 ),
                  _dataOwner( true )
#else
soma::DicomProxy::DicomProxy( soma::DataInfo* info )
                : soma::Model(),
                  _buffer( 0 ),
                  _dataOwner( true )
#endif
{

  if ( info )
  {

    _info = *info;

  }

}


#ifdef SOMA_IO_DICOM
soma::DicomProxy::DicomProxy( uint8_t* ptr, soma::DataInfo* info  )
                : _buffer( ptr ),
                  _dataOwner( false )
#else
soma::DicomProxy::DicomProxy( uint8_t* ptr, soma::DataInfo* info  )
                : soma::Model(),
                  _buffer( ptr ),
                  _dataOwner( false )
#endif
{

  if ( info )
  {

    _info = *info;

  }

}


soma::DicomProxy::~DicomProxy()
{

  destroy();

}


bool soma::DicomProxy::allocate( soma::DataInfo* info )
{

  if ( _buffer )
  {

    destroy();

  }

  if ( info )
  {

    _info = *info;

  }

  _info.initialize();

  if ( _dataOwner )
  {

#ifdef __SSE2__
    _buffer = (uint8_t*)_mm_malloc( _info._datasetSize * _info._bpp, 16 );
#else
    _buffer = new uint8_t[ _info._datasetSize * _info._bpp ];
#endif

  }

  if ( !_buffer )
  {

    return false;

  }

  std::memset( _buffer, 
               0, 
               _info._datasetSize * _info._bpp * sizeof( uint8_t ) );

  int32_t y, z, t;
  int32_t dimY = _info._height;
  int32_t dimZ = _info._slices;
  int32_t dimT = _info._frames;
  int32_t scanline = _info._width * _info._bpp;
  uint8_t* p = _buffer;

  _lineAccess.resize( dimT );

  for ( t = 0; t < dimT; t++ )
  {

    _lineAccess[ t ].resize( dimZ );

    for ( z = 0; z < dimZ; z++ )
    {

      _lineAccess[ t ][ z ].resize( dimY );

      for ( y = 0; y < dimY; y++, p += scanline )
      {

        _lineAccess[ t ][ z ][ y ] = p;

      }

    }

  }

  return true;

}


void soma::DicomProxy::destroy()
{

  if ( _dataOwner )
  {

#ifdef __SSE2__
    _mm_free( _buffer );
#else
    delete [] _buffer;
#endif

    _buffer = 0;

  }

  _lineAccess.clear();

}


bool soma::DicomProxy::isNull() const
{

  if ( _buffer )
  {

    return false;

  }

  return true;

}


bool soma::DicomProxy::isMemoryMapped() const
{

  return false;

}


soma::DataInfo& soma::DicomProxy::getDataInfo()
{

  return _info;

}


soma::BinaryHeader& soma::DicomProxy::getBinaryHeader()
{

  return _info._datasetHeader;

}


uint8_t* soma::DicomProxy::operator()( int32_t x, 
                                       int32_t y, 
                                       int32_t z, 
                                       int32_t t )
{

  assert( ( x >= 0 ) && ( x < _info._width ) );
  assert( ( y >= 0 ) && ( y < _info._height ) );
  assert( ( z >= 0 ) && ( z < _info._slices ) );
  assert( ( t >= 0 ) && ( t < _info._frames ) );

  return _lineAccess[ t ][ z ][ y ] + x * _info._bpp;

}

