//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/bufferdatasource.h>           // class definition
//--- system -----------------------------------------------------------------
#include <cstring>
//----------------------------------------------------------------------------

using namespace soma;
using namespace std;

BufferDataSource::BufferDataSource( char* ptr, unsigned long size, int mode )
  : _buffer( ptr ), _size( size ), _pos( 0 )
{
  _mode = mode;
}


BufferDataSource::~BufferDataSource()
{
}


DataSource* BufferDataSource::clone() const
{
  return new BufferDataSource( _buffer, _size, _mode );
}


int BufferDataSource::iterateMode() const
{
  return DirectAccess | SequentialAccess;
}


bool BufferDataSource::isOpen() const
{
  return _buffer != 0;
}


bool BufferDataSource::open( int mode )
{
  _mode = mode;
  _pos = 0;
  return _buffer != 0;
}


void BufferDataSource::setBuffer( char* ptr, unsigned long size, int mode )
{
  _buffer = ptr;
  _size = size;
  _mode = mode;
  _pos = 0;
}


void BufferDataSource::close()
{
  _pos = 0;
}


soma::offset_t BufferDataSource::size() const
{
  return _size;
}


soma::offset_t BufferDataSource::at() const
{
  return _pos;
}


bool BufferDataSource::at( offset_t pos )
{
  if( pos < _size )
    {
      _pos = pos;
      return true;
    }
  return false;
}


long BufferDataSource::readBlock( char * data, unsigned long maxlen )
{
  if( !(mode() & Read) || _pos >= _size )
    return -1;
  if( _size - _pos > maxlen )
    maxlen = _size - _pos;
  memcpy( data, _buffer + _pos, maxlen );
  _pos += maxlen;
  return maxlen;
}


long BufferDataSource::writeBlock( const char * data, unsigned long len )
{
  if( !(mode() & Write) || _pos >= _size )
    return -1;
  if( _size - _pos > len )
    len = _size - _pos;
  memcpy( _buffer + _pos, data, len );
  _pos += len;
  return len;
}


int BufferDataSource::getch()
{
  if( !(mode() & Read) || _pos >= _size )
    return -1;
  char	c = *(_buffer + _pos);
  ++_pos;
  return c;
}


int BufferDataSource::putch( int ch )
{
  if( !(mode() & Write) || _pos >= _size )
    return -1;
  char	c = static_cast<char>( ch );
  *(_buffer + _pos) = c;
  ++_pos;
  return ch;
}


bool BufferDataSource::ungetch( int ch )
{
  if( !(mode() & Read) || _pos == 0 )
    return false;
  --_pos;
  _buffer[ _pos ] = ch;
  return true;
}


bool BufferDataSource::allowsMemoryMapping() const
{
  return false;
}


const char* BufferDataSource::buffer() const
{
  return _buffer;
}


char* BufferDataSource::buffer()
{
  return _buffer;
}

