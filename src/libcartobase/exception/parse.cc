//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/config/cartobase-config.h>
#include <cartobase/exception/parse.h>
#include <stdio.h>
#include <errno.h>


using namespace std;
using namespace carto;


//=============================================================================
//	M E T H O D S
//=============================================================================

parse_error::parse_error( const string& what_arg,
                          const string& token_arg,
                          const string& filename_arg,
                          int line_arg )
            : io_error( init( what_arg,
			      token_arg,
			      filename_arg,
			      line_arg ), "" ),
              _filename( filename_arg ),
              _line( line_arg )
{
}


parse_error::parse_error( const string& what_arg,
                          const string& token_arg,
                          int line_arg )
            : io_error( init( what_arg, token_arg, line_arg ), "" ),
              _line( line_arg )
{
}


parse_error::parse_error( const string& what_arg, int line_arg,
                          const string& filename_arg )
            : io_error( what_arg, "" ),
              _filename( filename_arg ),
              _line( line_arg )
{
}


parse_error::~parse_error() throw ()
{
}


const char* parse_error::filename() const
{

  return _filename.c_str();

}


int parse_error::line() const
{

  return _line;

}


string parse_error::init( const string& what_arg,
                          const string& token_arg,
                          const string& filename_arg,
                          int line_arg )
{

  string s( filename_arg );
  s += ": found \"";
  s += token_arg;
  s += "\" while waiting for \"";
  s += what_arg;
  s += "\" (line ";
  char buffer[ 10 ];
  sprintf(buffer, "%d", line_arg);
  s += buffer;
  s += ")";
  return s;

}


string parse_error::init( const string& what_arg,
                          const string& token_arg,
                          int line_arg )
{

  string s("found \"");
  s += token_arg;
  s += "\" while waiting for \"";
  s += what_arg;
  s += "\" (line ";
  char buffer[ 10 ];
  sprintf(buffer, "%d", line_arg);
  s += buffer;
  s += ")";
  return s;

}
