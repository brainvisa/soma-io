#include <soma-io/config/version.h>
#include <cartobase/stream/sstream.h>

using namespace std;

string soma::somaioVersionString()
{
  static string	ver;
  if( ver.empty() )
    {
      ostringstream	s;
      s << SOMAIO_VERSION_MAJOR << '.' << SOMAIO_VERSION_MINOR << '.' 
        << SOMAIO_VERSION_TINY;
      ver = s.str();
    }
  return ver;
}


string soma::somaioShortVersion()
{
  static string	ver;
  if( ver.empty() )
    {
      ostringstream	s;
      s << SOMAIO_VERSION_MAJOR << '.' << SOMAIO_VERSION_MINOR;
      ver = s.str();
    }
  return ver;
}


