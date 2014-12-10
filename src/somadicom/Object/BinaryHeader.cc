#include <soma-io/Object/BinaryHeader.h>

#include <cstring>
#include <cassert>


soma::BinaryHeader::BinaryHeader()
                  : m_format( "" )
{
}


soma::BinaryHeader::BinaryHeader( const soma::BinaryHeader& other )
{

  this->operator = ( other );

}


soma::BinaryHeader::~BinaryHeader()
{

  clear();

}


soma::BinaryHeader& soma::BinaryHeader::operator = ( 
                                               const soma::BinaryHeader& other )
{

  resize( other.getCount() );

  uint32_t i = 0;
  std::vector< soma::BinaryHeader::Buffer >::const_iterator
    o = other.begin(),
    oe = other.end();
  std::vector< soma::BinaryHeader::Buffer >::iterator
    b = m_buffers.begin();

  while ( o != oe )
  {

    if ( allocate( i, o->second ) )
    {

      std::memcpy( b->first, o->first, o->second );
      b->second = o->second;

    }

    ++o;
    i++;

  }

  return *this;

}


soma::BinaryHeader::Buffer& soma::BinaryHeader::operator [] ( 
                                                          const uint32_t index )
{

  assert( index < uint32_t( m_buffers.size() ) );

  return m_buffers[ index ];

}


soma::BinaryHeader::iterator soma::BinaryHeader::begin()
{

  return m_buffers.begin();

}


soma::BinaryHeader::iterator soma::BinaryHeader::end()
{

  return m_buffers.end();

}


soma::BinaryHeader::const_iterator soma::BinaryHeader::begin() const
{

  return m_buffers.begin();

}


soma::BinaryHeader::const_iterator soma::BinaryHeader::end() const
{

  return m_buffers.end();

}


int32_t soma::BinaryHeader::getCount() const
{

  return (int32_t)m_buffers.size();

}


void soma::BinaryHeader::resize( uint32_t n, soma::BinaryHeader::Buffer buffer )
{

  if ( n )
  {

    m_buffers.resize( n, buffer );

  }

}


bool soma::BinaryHeader::allocate( uint32_t index, uint32_t length )
{

  if ( length && ( index < uint32_t( m_buffers.size() ) ) )
  {

    length += length % 2;

    soma::BinaryHeader::Buffer& buffer = m_buffers[ index ];

    if ( buffer.first )
    {

      delete [] buffer.first;

    }

    buffer.first = new uint8_t[ length ];

    if ( buffer.first )
    {

      std::memset( buffer.first, 0, length );
      buffer.second = length;

      return true;

    }

  }

  return false;

}


void soma::BinaryHeader::clear()
{

  std::vector< soma::BinaryHeader::Buffer >::iterator
    b = m_buffers.begin(),
    be = m_buffers.end();

  while ( b != be )
  {

    delete [] b->first;
    ++b;

  }

  m_buffers.clear();

}


void soma::BinaryHeader::setFormat( const std::string& format )
{

  m_format = format;

}


const std::string& soma::BinaryHeader::getFormat() const
{

  return m_format;

}

