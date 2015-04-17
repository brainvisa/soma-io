#ifdef SOMA_IO_DICOM
#include <soma-io/Object/BinaryHeader.h>
#else
#include <Object/BinaryHeader.h>
#endif

#include <algorithm>
#include <functional>
#include <cstring>
#include <cassert>


dcm::BinaryHeader::BinaryHeader()
                 : _format( "" )
{
}


dcm::BinaryHeader::BinaryHeader( const dcm::BinaryHeader& other )
{

  this->operator = ( other );

}


dcm::BinaryHeader::~BinaryHeader()
{

  clear();

}


dcm::BinaryHeader& dcm::BinaryHeader::operator = ( 
                                                const dcm::BinaryHeader& other )
{

  resize( other.getCount() );

  uint32_t i = 0;
  std::vector< dcm::BinaryHeader::Buffer >::const_iterator
    o = other.begin(),
    oe = other.end();
  std::vector< dcm::BinaryHeader::Buffer >::iterator
    b = _buffers.begin();

  while ( o != oe )
  {

    if ( allocate( i, o->second ) )
    {

      std::memcpy( b->first, o->first, o->second );
      b->second = o->second;

    }

    ++o;
    ++b;
    i++;

  }

  _lut = other._lut;

  return *this;

}


dcm::BinaryHeader::Buffer& dcm::BinaryHeader::operator [] ( 
                                                          const uint32_t index )
{

  assert( index < uint32_t( _buffers.size() ) );

  return _buffers[ _lut[ index ] ];

}


dcm::BinaryHeader::iterator dcm::BinaryHeader::begin()
{

  return _buffers.begin();

}


dcm::BinaryHeader::iterator dcm::BinaryHeader::end()
{

  return _buffers.end();

}


dcm::BinaryHeader::const_iterator dcm::BinaryHeader::begin() const
{

  return _buffers.begin();

}


dcm::BinaryHeader::const_iterator dcm::BinaryHeader::end() const
{

  return _buffers.end();

}


int32_t dcm::BinaryHeader::getCount() const
{

  return (int32_t)_buffers.size();

}


std::vector< int32_t >& dcm::BinaryHeader::getLut()
{

  return _lut;

}


void dcm::BinaryHeader::resize( uint32_t n, dcm::BinaryHeader::Buffer buffer )
{

  if ( n )
  {

    _buffers.resize( n, buffer );
    _lut.resize( n, 0 );

    int32_t i = 0;
    std::vector< int32_t >::iterator
      l = _lut.begin(),
      le = _lut.end();

    while ( l != le )
    {

      *l = i++;
      ++l;

    }

  }

}


bool dcm::BinaryHeader::allocate( uint32_t index, uint32_t length )
{

  if ( index < uint32_t( _buffers.size() ) )
  {

    dcm::BinaryHeader::Buffer& buffer = _buffers[ index ];

    return allocate( buffer, length );

  }

  return false;

}


bool dcm::BinaryHeader::allocate( dcm::BinaryHeader::Buffer& buffer, 
                                  uint32_t length )
{

  if ( length )
  {

    length += length % 2;

    if ( buffer.first )
    {

      delete [] buffer.first;

    }

    buffer.first = new uint8_t[ length ];

    if ( buffer.first )
    {

      buffer.second = length;

      return true;

    }

  }

  return false;

}


void dcm::BinaryHeader::add( dcm::BinaryHeader::Buffer& buffer )
{

  _buffers.push_back( buffer );
  _lut.push_back( int32_t( _lut.size() ) );

}


void dcm::BinaryHeader::trim()
{

  std::vector< dcm::BinaryHeader::Buffer >::iterator
    last = std::remove_if( _buffers.begin(),
                           _buffers.end(),
                           std::bind2nd( 
                                   std::equal_to< dcm::BinaryHeader::Buffer >(),
                                   dcm::BinaryHeader::Buffer( 0, 0 ) ) );

  _buffers.erase( last, _buffers.end() );
  _lut.resize( _buffers.size() );

  int32_t i = 0;
  std::vector< int32_t >::iterator
    l = _lut.begin(),
    le = _lut.end();

  while ( l != le )
  {

    *l = i++;
    ++l;

  }

}


void dcm::BinaryHeader::clear()
{

  std::vector< dcm::BinaryHeader::Buffer >::iterator
    b = _buffers.begin(),
    be = _buffers.end();

  while ( b != be )
  {

    delete [] b->first;
    ++b;

  }

  _buffers.clear();
  _lut.clear();

}


void dcm::BinaryHeader::setFormat( const std::string& format )
{

  _format = format;

}


const std::string& dcm::BinaryHeader::getFormat() const
{

  return _format;

}
