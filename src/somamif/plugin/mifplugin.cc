//--- plugin -----------------------------------------------------------------
#include <soma-io/plugin/mifplugin.h>
#include <soma-io/checker/mifformatchecker.h>
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

namespace soma
{
  namespace
  {
    bool initmif()
    {
      new MifPlugin;
      return true;
    }
    bool mifinit __attribute__((unused)) = initmif();
  }
}

MifPlugin::MifPlugin() : Plugin()
{
    vector<string>  exts{"mif", "mif.gz", "mih"};

    //////////////////////////////////////////////////////////////////////////
    ////                         C H E C K E R                            ////
    //////////////////////////////////////////////////////////////////////////

    DataSourceInfoLoader::registerFormat( "MIF",
                                          new MifFormatChecker, exts );
}


MifPlugin::~MifPlugin()
{
}


string MifPlugin::name() const
{
  return string("MIF SOMA-IO");
}


bool MifPlugin::noop()
{
  return true;
}
