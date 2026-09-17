//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/filedatasource.h>              // class definition
//--- cartobase --------------------------------------------------------------
#include <cartobase/stream/fileutil.h>                           // fileStat()
//--- sys --------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
//--- system -----------------------------------------------------------------
#include <fcntl.h>
#include <unistd.h>
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

FileDataSource::FileDataSource( const string & filename, offset_t offset, 
                                int mode )
  :
#ifdef CARTO_FILEDS_AS_FD
  FDDataSource( -1, mode ), 
#else
  CStreamDataSource( 0, mode ), 
#endif
  _url( filename ), _offset( offset )
{
  if( mode & Write )
    {
      string st = FileUtil::fileStat( filename );
      if( st.find( '+' ) != string::npos && st.find( 'w' ) == string::npos )
        {
          // file is not writeable
          mode &= ~Write;
          _mode = mode;
        }
    }
}


FileDataSource::~FileDataSource()
{
}


DataSource* FileDataSource::clone() const
{
  return new FileDataSource( _url, _offset, _mode );
}


string FileDataSource::url() const
{
  return _url;
}


soma::offset_t FileDataSource::initialOffset() const
{
  return _offset;
}


bool FileDataSource::open( int mode )
{
  if( isOpen() )
    return true;

#ifdef CARTO_FILEDS_AS_FD

  _mode = mode;
  int	flags;
  switch( mode )
    {
    case Read:
      flags = O_RDONLY;
      break;
    case Write:
      flags = O_WRONLY | O_CREAT | O_TRUNC;
      break;
    case ReadWrite:
      flags = O_RDWR;
      break;
    default:
      flags = O_RDONLY;
    }
#if !defined( __APPLE__ ) && !defined( _WIN32 )
  // MacOS 10 doesn't have this flag
  flags |= O_LARGEFILE;
#endif

#ifdef _WIN32
  int	fd = ::open( url().c_str(), flags );
#else
  int	fd = ::open( url().c_str(), flags, S_IRUSR | S_IWUSR | S_IRGRP 
                     | S_IWGRP | S_IROTH | S_IWOTH );
#endif

  setDescriptor( fd );
  if( _fd < 0 && ( mode & Write ) )
    {
      // try to reopen it read-only
      return open( mode & ~Write );
    }

  if( descriptor() >= 0 && _offset != 0 )
    lseek( _fd, _offset, SEEK_SET );
  return _fd >= 0;

#else // CARTO_FILEDS_AS_FD

  return CStreamDataSource::open( mode );

#endif
}


bool FileDataSource::allowsMemoryMapping() const
{
  return true;
}


soma::offset_t FileDataSource::size() const
{
#ifdef CARTO_FILEDS_AS_FD
  return FDDataSource::size() - _offset;
#else
  return CStreamDataSource::size() - _offset;
#endif
}

soma::offset_t FileDataSource::at() const
{
#ifdef CARTO_FILEDS_AS_FD
  return FDDataSource::at() - _offset;
#else
  return CStreamDataSource::at() - _offset;
#endif
}


bool FileDataSource::at( offset_t pos )
{
#ifdef CARTO_FILEDS_AS_FD
  return FDDataSource::at( pos + _offset );
#else
  return CStreamDataSource::at( pos + _offset );
#endif
}

