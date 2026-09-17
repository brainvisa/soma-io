//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/cstreamdatasource.h>           // base definition
//--- sys --------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
//--- system -----------------------------------------------------------------
#include <unistd.h>
#include <cstdio>
//----------------------------------------------------------------------------

using namespace soma;
using namespace std;

CStreamDataSource::CStreamDataSource( FILE *f, int mode ) 
  : DataSource(), _f( f )
{
  _mode = mode;
}


CStreamDataSource::~CStreamDataSource()
{
  close();
}


DataSource* CStreamDataSource::clone() const
{
  return new CStreamDataSource( _f, _mode );
}


FILE *CStreamDataSource::filePtr() const
{
  return _f;
}


void CStreamDataSource::setFilePtr( FILE *f )
{
  _f = f;
}


int CStreamDataSource::iterateMode() const
{
  return DirectAccess | SequentialAccess;
}


bool CStreamDataSource::isOpen() const
{
  return _f != 0;
}


bool CStreamDataSource::open ( int mode )
{
  _mode = mode;
  if( _f != 0 )
    return _f;
  const char	*smode = "rb";
  switch( mode )
    {
    case Read:
      smode = "rb";
      break;
    case Write:
      smode = "wb";
      break;
    case ReadWrite:
      smode = "rb+";
      break;
    }
  _f = fopen( url().c_str(), smode );
  return _f != 0;
}


void CStreamDataSource::close()
{
  if( _f )
    {
      fclose( _f );
      _f = 0;
    }
}


soma::offset_t CStreamDataSource::size() const
{
  struct stat	buf;
  if( fstat( fileno( _f ), &buf ) != 0 )
    return 0;
  return buf.st_size;
}


soma::offset_t CStreamDataSource::at() const
{
  if( _f )
    return ftell( _f );
  return 0;
}


bool CStreamDataSource::at( offset_t pos )
{
  if( _f )
    return fseek( _f, pos, SEEK_SET ) != -1;
  return false;
}


bool CStreamDataSource::allowsMemoryMapping() const
{
  return true;
}


long CStreamDataSource::readBlock( char* data, unsigned long maxlen )
{
  return fread( data, 1, maxlen, _f );
}


long CStreamDataSource::writeBlock( const char* data, unsigned long len )
{
  return fwrite( data, 1, len, _f );
}


int CStreamDataSource::getch()
{
  char	c;
  if( readBlock( &c, 1 ) != 1 )
    return -1;
  return c;
}


int CStreamDataSource::putch( int ch )
{
  char	c = static_cast<char>( ch );
  if( writeBlock( &c, 1 ) != 1 )
    return -1;
  return c;
}


bool CStreamDataSource::ungetch( int )
{
  return fseek( _f, -1, SEEK_CUR ) != -1;
}


