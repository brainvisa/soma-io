//--- plugin -----------------------------------------------------------------
#include <soma-io/plugin/gisplugin.h>
#include <soma-io/checker/gisformatchecker.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
//--- system -----------------------------------------------------------------
#include <vector>
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

namespace soma {
  namespace {
    bool initgis()
    {
      new GisPlugin;
      return true;
    }
    bool gisinit __attribute__((unused)) = initgis();
  }
}

GisPlugin::GisPlugin() : Plugin()
{
    vector<string>  exts(2);
    exts[0] = "ima";
    exts[1] = "dim";

    //////////////////////////////////////////////////////////////////////////
    ////                         C H E C K E R                            ////
    //////////////////////////////////////////////////////////////////////////

    DataSourceInfoLoader::registerFormat( "GIS", new GisFormatChecker, exts );
}


GisPlugin::~GisPlugin()
{
}


string GisPlugin::name() const
{
  return string("GIS SOMA-IO");
}


bool GisPlugin::noop()
{
  return true;
}

