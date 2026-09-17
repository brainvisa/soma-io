//--- plugin -----------------------------------------------------------------
#include <soma-io/image/tiffimagewriter_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class TiffImageWriter<int8_t>;
template class TiffImageWriter<int16_t>;
template class TiffImageWriter<int32_t>;
template class TiffImageWriter<uint8_t>;
template class TiffImageWriter<uint16_t>;
template class TiffImageWriter<uint32_t>;
template class TiffImageWriter<float>;
template class TiffImageWriter<double>;
template class TiffImageWriter<VoxelRGB>;
template class TiffImageWriter<VoxelRGBA>;
template class TiffImageWriter<VoxelHSV>;

} // namespace soma
