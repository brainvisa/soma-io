//--- plugin -----------------------------------------------------------------
#include <soma-io/image/gisimagewriter_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class GisImageWriter<int8_t>;
template class GisImageWriter<int16_t>;
template class GisImageWriter<int32_t>;
template class GisImageWriter<int64_t>;
template class GisImageWriter<uint8_t>;
template class GisImageWriter<uint16_t>;
template class GisImageWriter<uint32_t>;
template class GisImageWriter<uint64_t>;
template class GisImageWriter<float>;
template class GisImageWriter<double>;
template class GisImageWriter<VoxelRGB>;
template class GisImageWriter<VoxelRGBA>;
template class GisImageWriter<VoxelHSV>;
template class GisImageWriter<std::complex<float> >;
template class GisImageWriter<std::complex<double> >;

} // namespace soma
