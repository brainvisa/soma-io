//--- plugin -----------------------------------------------------------------
#include <soma-io/image/dicomimagewriter_d.h>
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

  template class DicomImageWriter<int8_t>;
  template class DicomImageWriter<uint8_t>;
  template class DicomImageWriter<int16_t>;
  template class DicomImageWriter<uint16_t>;
  template class DicomImageWriter<float>;
  template class DicomImageWriter<VoxelRGB>;

}
