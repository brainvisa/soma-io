//--- plugin -----------------------------------------------------------------
#include <soma-io/image/dicomimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/type/voxelrgb.h>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;

namespace carto
{
  template class DicomImageReader<int8_t>;
  template class DicomImageReader<int16_t>;
  template class DicomImageReader<uint8_t>;
  template class DicomImageReader<uint16_t>;
  template class DicomImageReader<float>;
  template class DicomImageReader<VoxelRGB>;
}
