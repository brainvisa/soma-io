//--- plugin -----------------------------------------------------------------
#include <soma-io/plugin/niftiplugin.h>
#include <soma-io/checker/niftiformatchecker.h>
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
    bool initnifti()
    {
      new NiftiPlugin;
      return true;
    }
    bool niftiinit __attribute__((unused)) = initnifti();
  }
}

NiftiPlugin::NiftiPlugin() : Plugin()
{
    vector<string>  exts(5);
    exts[0] = "nii";
    exts[1] = "nii.gz";
    exts[2] = "img";
    exts[3] = "img.gz";
    exts[4] = "hdr";

    //////////////////////////////////////////////////////////////////////////
    ////                         C H E C K E R                            ////
    //////////////////////////////////////////////////////////////////////////

    DataSourceInfoLoader::registerFormat( "NIFTI-1",
                                          new Nifti1FormatChecker, exts );
    DataSourceInfoLoader::registerFormat( "NIFTI-2",
                                          new Nifti2FormatChecker, exts );
}


NiftiPlugin::~NiftiPlugin()
{
}


string NiftiPlugin::name() const
{
  return string("NIFTI SOMA-IO");
}


bool NiftiPlugin::noop()
{
  return true;
}

