#include <soma-io/config/osversion.h>
#include <cartobase/stream/sstream.h>

using namespace std;

string soma::openslideVersionString()
{
  static string	ver;
  if( ver.empty() )
    {
      ver = OPENSLIDE_VERSION;
    }
  return ver;
}


string soma::openslideShortVersion()
{
  static string	ver;
  if( ver.empty() )
    {
      string longver = OPENSLIDE_VERSION;
      size_t found = longver.rfind('.');
      if (found != std::string::npos)
        ver = longver.substr(0, found);
    }
  return ver;
}
