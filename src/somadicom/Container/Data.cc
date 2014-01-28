#include <soma-io/Container/Data.h>

#include <cstring>
#include <cassert>


soma::Data::Data()
          : m_data( 0 )
{
}


soma::Data::~Data()
{

  Destroy();

}


bool soma::Data::Create( const soma::DataInfo& info )
{

  m_info = info;
  m_info.initialize();

  m_data = new uint8_t[ m_info.m_datasetSize * m_info.m_bpp ];

  if ( !m_data )
  {

    return false;

  }

  std::memset( m_data, 
               0, 
               m_info.m_datasetSize * m_info.m_bpp * sizeof( uint8_t ) );

  int32_t y, z, t;
  int32_t dimY = m_info.m_height;
  int32_t dimZ = m_info.m_slices;
  int32_t dimT = m_info.m_frames;
  int32_t scanline = m_info.m_width * m_info.m_bpp;
  uint8_t* p = m_data;

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


void soma::Data::Destroy()
{

  delete[] m_data;

  m_info.clear();
  m_lineAccess.clear();

  m_data = 0;

}


bool soma::Data::IsNull()
{

  return m_data ? false : true;

}


uint8_t* soma::Data::getLine( int32_t y, int32_t z, int32_t t )
{

  assert( ( y >= 0 ) && ( y < m_info.m_height ) );
  assert( ( z >= 0 ) && ( z < m_info.m_slices ) );
  assert( ( t >= 0 ) && ( t < m_info.m_frames ) );

  return m_lineAccess[ t ][ z ][ y ];

}


uint8_t* soma::Data::getSlice( int32_t z, int32_t t )
{

  assert( ( z >= 0 ) && ( z < m_info.m_slices ) );
  assert( ( t >= 0 ) && ( t < m_info.m_frames ) );

  return m_lineAccess[ t ][ z ][ 0 ];

}


uint8_t* soma::Data::getFrame( int32_t t )
{

  assert( ( t >= 0 ) && ( t < m_info.m_frames ) );

  return m_lineAccess[ t ][ 0 ][ 0 ];

}
