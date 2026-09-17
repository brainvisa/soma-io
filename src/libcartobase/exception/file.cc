//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/exception/file.h>
#include <errno.h>
#include <string.h>


using namespace std;
using namespace carto;


//=============================================================================
//	M E T H O D S
//=============================================================================

file_error::file_error( const string& filename_arg )
	   : errno_error( init( filename_arg ) ),
             _filename( filename_arg )
{
}


file_error::file_error( const string&, const string& filename_arg )
	   : errno_error( init( filename_arg ) ),
             _filename( filename_arg )
{
}


file_error::~file_error() throw ()
{
}


const char* file_error::filename() const
{

  return _filename.c_str();

}


string file_error::init( const string& filename_arg )
{

  string s(filename_arg);
  s += ": ";
  s += strerror(errno);
  return s;

}
