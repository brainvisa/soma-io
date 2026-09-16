#include <cartobase/config/version.h>
#include <cartobase/stream/sstream.h>

using namespace std;

string carto::cartobaseVersionString()
{
  static string	ver;
  if( ver.empty() )
    {
      ostringstream	s;
      s << CARTOBASE_VERSION_MAJOR << '.' << CARTOBASE_VERSION_MINOR << '.' 
        << CARTOBASE_VERSION_TINY;
      ver = s.str();
    }
  return ver;
}


string carto::cartobaseShortVersion()
{
  static string	ver;
  if( ver.empty() )
    {
      ostringstream	s;
      s << CARTOBASE_VERSION_MAJOR << '.' << CARTOBASE_VERSION_MINOR;
      ver = s.str();
    }
  return ver;
}


