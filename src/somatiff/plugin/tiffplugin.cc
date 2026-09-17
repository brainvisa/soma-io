//--- plugin -----------------------------------------------------------------
#include <soma-io/plugin/tiffplugin.h>
#include <soma-io/checker/tiffformatchecker.h>
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
    bool inittiff()
    {
      new TiffPlugin;
      return true;
    }
    bool tiffinit __attribute__((unused)) = inittiff();
  }
}

TiffPlugin::TiffPlugin() : Plugin()
{
    vector<string>  exts(2);
    exts[0] = "tif";
    exts[1] = "tiff";

    //////////////////////////////////////////////////////////////////////////
    ////                         C H E C K E R                            ////
    //////////////////////////////////////////////////////////////////////////
    DataSourceInfoLoader::registerFormat( "TIFF", new TiffFormatChecker, exts );
}


TiffPlugin::~TiffPlugin()
{
}


string TiffPlugin::name() const
{
  return string("TIFF SOMA-IO");
}


bool TiffPlugin::noop()
{
  return true;
}

