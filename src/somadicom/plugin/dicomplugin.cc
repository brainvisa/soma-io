//--- plugin -------------------------------------------------------------------
#include <soma-io/plugin/dicomplugin.h>
#include <soma-io/checker/dicomformatchecker.h>
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
    bool initDicom()
    {
      new DicomPlugin;
      return true;
    }
    bool dicominit __attribute__((unused)) = initDicom();
  }
}

DicomPlugin::DicomPlugin() : Plugin()
{
    vector<string> exts( 3 );
    exts[0]="dcm";
    exts[1]="dic";
    exts[2]="";
    
    ////////////////////////////////////////////////////////////////////////////
    ////                          C H E C K E R                             ////
    ////////////////////////////////////////////////////////////////////////////
    
    DataSourceInfoLoader::registerFormat( "DICOM", new DicomFormatChecker, exts );
    
}


DicomPlugin::~DicomPlugin()
{
}


string DicomPlugin::name() const
{
  return string("DICOM SOMA-IO");
}


bool DicomPlugin::noop()
{
  return true;
}

