#include <soma-io/Container/DicomProxy.h>

#include <cstring>
#include <cassert>


soma::DicomProxy::DicomProxy( soma::DataInfo* info )
                : m_buffer( 0 ),
                  m_dataOwner( true )
{

  if ( info )
  {

    m_info = *info;

  }

}


soma::DicomProxy::DicomProxy( uint8_t* ptr, soma::DataInfo* info  )
                : m_buffer( ptr ),
                  m_dataOwner( false )
{

  if ( info )
  {

    m_info = *info;

  }

}


soma::DicomProxy::~DicomProxy()
{

  destroy();

}


bool soma::DicomProxy::allocate( soma::DataInfo* info )
{

  if ( m_buffer )
  {

    destroy();

  }

  if ( info )
  {

    m_info = *info;

  }

  m_info.initialize();
  m_datasetHeader.clear();
  m_datasetHeader.resize( m_info.m_fileCount );
  m_datasetHeader.setFormat( "dicom" );

  if ( m_dataOwner )
  {

    m_buffer = new uint8_t[ m_info.m_datasetSize * m_info.m_bpp ];

  }

  if ( !m_buffer )
  {

    return false;

  }

  std::memset( m_buffer, 
               0, 
               m_info.m_datasetSize * m_info.m_bpp * sizeof( uint8_t ) );

  int32_t y, z, t;
  int32_t dimY = m_info.m_height;
  int32_t dimZ = m_info.m_slices;
  int32_t dimT = m_info.m_frames;
  int32_t scanline = m_info.m_width * m_info.m_bpp;
  uint8_t* p = m_buffer;

  m_lineAccess.resize( dimT );

  for ( t = 0; t < dimT; t++ )
  {

    m_lineAccess[ t ].resize( dimZ );

    for ( z = 0; z < dimZ; z++ )
    {

      m_lineAccess[ t ][ z ].resize( dimY );

      for ( y = 0; y < dimY; y++, p += scanline )
      {

        m_lineAccess[ t ][ z ][ y ] = p;

      }

    }

  }

  return true;

}


void soma::DicomProxy::destroy()
{

  if ( m_dataOwner )
  {

    delete[] m_buffer;

    m_buffer = 0;

  }

  m_lineAccess.clear();

}


bool soma::DicomProxy::isNull()
{

  return m_buffer ? false : true;

}


bool soma::DicomProxy::isMemoryMapped() const
{

  return false;

}


soma::DataInfo& soma::DicomProxy::getDataInfo()
{

  return m_info;

}


soma::BinaryHeader& soma::DicomProxy::getBinaryHeader()
{

  return m_datasetHeader;

}


uint8_t* soma::DicomProxy::operator()( int32_t x, 
                                       int32_t y, 
                                       int32_t z, 
                                       int32_t t )
{

  assert( ( x >= 0 ) && ( x < m_info.m_width ) );
  assert( ( y >= 0 ) && ( y < m_info.m_height ) );
  assert( ( z >= 0 ) && ( z < m_info.m_slices ) );
  assert( ( t >= 0 ) && ( t < m_info.m_frames ) );

  return m_lineAccess[ t ][ z ][ y ] + x * m_info.m_bpp;

}

