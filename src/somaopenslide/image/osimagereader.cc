//--- plugin -----------------------------------------------------------------
#include <soma-io/image/osimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgba.h>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;

namespace carto
{
  template class OSImageReader<VoxelRGBA>;
}

