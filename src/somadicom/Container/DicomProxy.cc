#ifdef SOMA_IO_DICOM
#include <soma-io/Container/DicomProxy.h>
#else
#include <Container/DicomProxy.h>
#endif

#include <cstring>

#if defined( __SSE2__ ) && !defined( SOMA_IO_DICOM )
#include <immintrin.h>
#endif


#ifdef SOMA_IO_DICOM
dcm::DicomProxy::DicomProxy( dcm::DataInfo* info )
               : _buffer( 0 ),
                 _dataOwner( true )
#else
dcm::DicomProxy::DicomProxy( dcm::DataInfo* info )
               : dcm::Model(),
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
dcm::DicomProxy::DicomProxy( uint8_t* ptr, dcm::DataInfo* info  )
               : _buffer( ptr ),
                _dataOwner( false )
#else
dcm::DicomProxy::DicomProxy( uint8_t* ptr, dcm::DataInfo* info  )
               : dcm::Model(),
                 _buffer( ptr ),
                 _dataOwner( false )
#endif
{

  if ( info )
  {

    _info = *info;

  }

}


dcm::DicomProxy::~DicomProxy()
{

  destroy();

}


bool dcm::DicomProxy::allocate( dcm::DataInfo* info )
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

  dcm::BoundingBox< int32_t > outBoundingBox;

  if ( _info._noFlip )
  {

    outBoundingBox = _info._boundingBox;

  }
  else
  {

    outBoundingBox =
           _info._patientOrientation.getDirectBoundingBox( _info._boundingBox );

  }

  int32_t dimX = outBoundingBox.getUpperX() - 
                 outBoundingBox.getLowerX() + 1;
  int32_t dimY = outBoundingBox.getUpperY() - 
                 outBoundingBox.getLowerY() + 1;
  int32_t dimZ = outBoundingBox.getUpperZ() - 
                 outBoundingBox.getLowerZ() + 1;
  int32_t dimT = outBoundingBox.getUpperT() - 
                 outBoundingBox.getLowerT() + 1;
  int32_t datasetSize = dimX * dimY * dimZ * dimT;

  if ( _dataOwner )
  {

#if defined( __SSE2__ ) && !defined( SOMA_IO_DICOM )
    _buffer = (uint8_t*)_mm_malloc( datasetSize * _info._bpp, 16 );
#else
    _buffer = new uint8_t[ datasetSize * _info._bpp ];
#endif

    std::memset( _buffer, 0, datasetSize * _info._bpp * sizeof( uint8_t ) );

  }

  if ( !_buffer )
  {

    return false;

  }

  int32_t y, z, t;
  int32_t scanline = dimX * _info._bpp;
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


void dcm::DicomProxy::destroy()
{

  if ( _dataOwner )
  {

#if defined( __SSE2__ ) && !defined( SOMA_IO_DICOM )
    _mm_free( _buffer );
#else
    delete [] _buffer;
#endif

    _buffer = 0;

  }

  _lineAccess.clear();

}


bool dcm::DicomProxy::isNull() const
{

  if ( _buffer )
  {

    return false;

  }

  return true;

}


bool dcm::DicomProxy::isMemoryMapped() const
{

  return false;

}


dcm::DataInfo& dcm::DicomProxy::getDataInfo()
{

  return _info;

}


dcm::BinaryHeader& dcm::DicomProxy::getBinaryHeader()
{

  return _info._datasetHeader;

}


uint8_t* dcm::DicomProxy::operator()( int32_t x, 
                                      int32_t y, 
                                      int32_t z, 
                                      int32_t t )
{

  return _lineAccess[ t ][ z ][ y ] + x * _info._bpp;

}
