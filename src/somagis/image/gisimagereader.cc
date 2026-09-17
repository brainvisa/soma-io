//--- plugin -----------------------------------------------------------------
#include <soma-io/image/gisimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class GisImageReader<int8_t>;
template class GisImageReader<int16_t>;
template class GisImageReader<int32_t>;
template class GisImageReader<int64_t>;
template class GisImageReader<uint8_t>;
template class GisImageReader<uint16_t>;
template class GisImageReader<uint32_t>;
template class GisImageReader<uint64_t>;
template class GisImageReader<float>;
template class GisImageReader<double>;
template class GisImageReader<VoxelRGB>;
template class GisImageReader<VoxelRGBA>;
template class GisImageReader<VoxelHSV>;
template class GisImageReader<std::complex<float> >;
template class GisImageReader<std::complex<double> >;

} // namespace soma
