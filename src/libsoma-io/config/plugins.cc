//--- moved to soma-io -------------------------------------------------------
#include <cartobase/plugin/plugin.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/config/version.h>
#include <cartobase/config/paths.h>
//--- system -----------------------------------------------------------------
#include <list>
#include <string>
//----------------------------------------------------------------------------

using namespace carto;
using namespace std;

namespace
{

  bool addSomaPlugins()
  {
    list<PluginLoader::PluginFile> & plugins = PluginLoader::pluginFiles();
    string somaplugins = Paths::findResourceFile( "somaio.plugins",
                                                  "soma-io" );
    if( !somaplugins.empty() ) {
      plugins.push_back( PluginLoader::PluginFile(
                            somaplugins, cartobaseVersionString() )
                       );
      return true;
    } else
      return false;
  }

  bool dummy __attribute__((unused)) = addSomaPlugins();

}
