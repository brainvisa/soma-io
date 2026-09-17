#include <cartobase/stream/directory.h>
#include <cartobase/stream/fileutil.h>
#include <cartobase/exception/file.h>
#include <iostream>
#include <list>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

using namespace carto;
using namespace std;


namespace carto
{
  struct Directory::Private
  {
    Private();
    void scan() const;

    string			dirname;
    mutable bool		scanned;
    mutable set<string>	files;
    mutable set<string>	directories;
    mutable bool		state;
  };
}


Directory::Private::Private() : scanned( false ), state( false )
{
}


void Directory::Private::scan() const
{
  files.clear();
  directories.clear();
  scanned = true;
  state = false;

  // non-portable things
  DIR	*dir = opendir( dirname.c_str() );
  if( !dir ) {
    //cerr << "cannot read directory " << dirname.c_str() << endl;
    return;
  }
  state = true;

  struct dirent	*dent = readdir( dir );
#ifndef _DIRENT_HAVE_D_TYPE
  struct stat	s;
  char	sep = FileUtil::separator();
#endif

  while( dent )
    {
#ifdef _DIRENT_HAVE_D_TYPE	// valid under linux
      /* cout << "dir entry " << dent->d_name << ", type: " 
         << (int) dent->d_type << endl; */
      // filter out . and ..
      if( strcmp( dent->d_name, "." ) && strcmp( dent->d_name, ".." ) )
	switch( dent->d_type )
	  {
	  case DT_DIR:
	    directories.insert( dent->d_name );
	    //cout << "(dir)\n";
	    break;
	  case DT_FIFO:	// pipe
	  case DT_CHR:	// char device
	  case DT_BLK:	// block device
	  case DT_REG:	// regular file
	  case DT_LNK:	// link / symlink
	  case DT_SOCK:	// socket
#ifdef DT_WHT
	  case DT_WHT:	// (?)
#endif
	  default:
	    files.insert( dent->d_name );
	    //cout << "(file)\n";
	  }
#else	// solaris, ...
      // filter out . and ..
      // here we must stat files to get their type
      if( strcmp( dent->d_name, "." ) && strcmp( dent->d_name, ".." ) 
	  && !stat(  (dirname + sep + dent->d_name).c_str(), &s ) )
	switch( s.st_mode )
	  {
	  case S_IFDIR:
	    if( !strcmp( dent->d_name, "." ) || !strcmp( dent->d_name, ".." ) )
	      break;
	    directories.insert( dent->d_name );
	    break;
	  case S_IFIFO:	// pipe
	  case S_IFCHR:		// char device
	  case S_IFBLK:		// block device
	  case S_IFREG:		// regular file
#ifndef _WIN32
	  case S_IFLNK:		// symlink (not returned by stat() but lstat())
#endif
	  default:
	    files.insert( dent->d_name );
	  }
#endif
      dent = readdir( dir );
    }
  closedir( dir );
}


Directory::Directory( const string & dname )
  : d( new Directory::Private )
{
  d->dirname = dname;
}


Directory::~Directory()
{
  delete d;
}


string Directory::dirname() const
{
  return( d->dirname );
}


void Directory::chdir( const string & dname )
{
  d->dirname = dname;
  d->scanned = false;
  d->files.clear();
  d->directories.clear();
  d->state = false;
}


set<string> Directory::files() const
{
  if( !d->scanned )
    d->scan();
  return( d->files );
}


set<string> Directory::directories() const
{
  if( !d->scanned )
    d->scan();
  return( d->directories );
}


bool Directory::isValid() const
{
  // this is not optimal, we should not entirely scan the directory
  if( !d->scanned )
    d->scan();
  return( d->state );
}


void Directory::mkdir() const
{
  DIR	*dir = opendir( dirname().c_str() );
  if( !dir )
    {
      d->state = true;
      d->scanned = false;
#if defined( _WIN32 )
      if( ::mkdir( d->dirname.c_str() ) )
#else
      if( ::mkdir( d->dirname.c_str(),
            S_IRWXU | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH
                | S_IXOTH ) )
#endif
        throw file_error( d->dirname );
    }
  else
    closedir( dir );
}


void Directory::makedirs() const
{
  string	dn = dirname();
  list<string>	dirs;
  Directory	dir( FileUtil::dirname( dirname() ) );

  while( !dir.isValid() && dir.dirname() != dn )
    {
      dn = dir.dirname();
      dirs.push_back( dn );
      dir.chdir( FileUtil::dirname( dn ) );
    }
  dirs.push_back( dirname() );

  list<string>::iterator	i, e = dirs.end();
  for( i=dirs.begin(); i!=e; ++i )
    {
      dir.chdir( *i );
      dir.mkdir();
    }
}


