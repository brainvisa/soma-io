//--- plugin -------------------------------------------------------------------
#include <soma-io/plugin/osplugin.h>
#include <soma-io/checker/osformatchecker.h>
//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
//--- system -------------------------------------------------------------------
#include <vector>
#include <string>
//------------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

namespace soma {
  namespace {
    bool initos()
    {
      new OSPlugin;
      return true;
    }
    bool osinit __attribute__((unused)) = initos();
  }
}

OSPlugin::OSPlugin() : Plugin()
{
    vector<string>  exts(9);
    exts[0] = "tif";    // Aperio, Trestle, Generic TIFF
    exts[1] = "tiff";   // Aperio, Trestle, Generic TIFF
    exts[2] = "svs";    // Aperio
    exts[3] = "vms";    // Hamamatsu
    exts[4] = "vmu";    // Hamamatsu
    exts[5] = "ndpi";   // Hamamatsu
    exts[6] = "scn";    // Leica
    exts[7] = "mrxs";   // MIRAX
    exts[8] = "czi";    // Zeiss

    ////////////////////////////////////////////////////////////////////////////
    ////                          C H E C K E R                             ////
    ////////////////////////////////////////////////////////////////////////////

    DataSourceInfoLoader::registerFormat( "OpenSlide", new OSFormatChecker, exts );

}


OSPlugin::~OSPlugin()
{
}


string OSPlugin::name() const
{
  return string("OpenSlide SOMA-IO");
}


bool OSPlugin::noop()
{
  return true;
}

