#ifdef SOMA_IO_DICOM
#include <soma-io/Object/BinaryHeader.h>
#else
#include <Object/BinaryHeader.h>
#endif

#include <algorithm>
#include <functional>
#include <cstring>
#include <cassert>


soma::BinaryHeader::BinaryHeader()
                  : _format( "" )
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


soma::BinaryHeader::Buffer& soma::BinaryHeader::operator [] ( 
                                                          const uint32_t index )
{

  assert( index < uint32_t( _buffers.size() ) );

  return _buffers[ _lut[ index ] ];

}


soma::BinaryHeader::iterator soma::BinaryHeader::begin()
{

  return _buffers.begin();

}


soma::BinaryHeader::iterator soma::BinaryHeader::end()
{

  return _buffers.end();

}


soma::BinaryHeader::const_iterator soma::BinaryHeader::begin() const
{

  return _buffers.begin();

}


soma::BinaryHeader::const_iterator soma::BinaryHeader::end() const
{

  return _buffers.end();

}


int32_t soma::BinaryHeader::getCount() const
{

  return (int32_t)_buffers.size();

}


std::vector< int32_t >& soma::BinaryHeader::getLut()
{

  return _lut;

}


void soma::BinaryHeader::resize( uint32_t n, soma::BinaryHeader::Buffer buffer )
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


bool soma::BinaryHeader::allocate( uint32_t index, uint32_t length )
{

  if ( index < uint32_t( _buffers.size() ) )
  {

    soma::BinaryHeader::Buffer& buffer = _buffers[ index ];

    return allocate( buffer, length );

  }

  return false;

}


bool soma::BinaryHeader::allocate( soma::BinaryHeader::Buffer& buffer, 
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


void soma::BinaryHeader::add( soma::BinaryHeader::Buffer& buffer )
{

  _buffers.push_back( buffer );
  _lut.push_back( int32_t( _lut.size() ) );

}


void soma::BinaryHeader::trim()
{

  std::vector< soma::BinaryHeader::Buffer >::iterator
    last = std::remove_if( _buffers.begin(),
                           _buffers.end(),
                           std::bind2nd( 
                                  std::equal_to< soma::BinaryHeader::Buffer >(),
                                  soma::BinaryHeader::Buffer( 0, 0 ) ) );

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


void soma::BinaryHeader::clear()
{

  std::vector< soma::BinaryHeader::Buffer >::iterator
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


void soma::BinaryHeader::setFormat( const std::string& format )
{

  _format = format;

}


const std::string& soma::BinaryHeader::getFormat() const
{

  return _format;

}

