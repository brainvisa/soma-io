#include <soma-io/Container/DatasetHeader.h>

#include <cstring>


soma::DatasetHeader::DatasetHeader( uint32_t length )
                   : m_buffer( 0 ),
                     m_length( length )
{

  allocate( length );

}


soma::DatasetHeader::~DatasetHeader()
{

  delete [] m_buffer;

}


bool soma::DatasetHeader::allocate( uint32_t length )
{

  if ( length )
  {

    length += length % 2;

    if ( m_buffer )
    {

      delete [] m_buffer;

    }

    m_buffer = new uint8_t[ length ];

    if ( m_buffer )
    {

      std::memset( m_buffer, 0, length );
      m_length = length;

      return true;

    }

  }

  return false;

}


uint8_t* soma::DatasetHeader::getBuffer()
{

  return m_buffer;

}


uint32_t soma::DatasetHeader::getLength()
{

  return m_length;

}
