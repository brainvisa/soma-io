//--- plugin -----------------------------------------------------------------
#include <soma-io/image/tiffimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class TiffImageReader<int8_t>;
template class TiffImageReader<int16_t>;
template class TiffImageReader<int32_t>;
template class TiffImageReader<int64_t>;
template class TiffImageReader<uint8_t>;
template class TiffImageReader<uint16_t>;
template class TiffImageReader<uint32_t>;
template class TiffImageReader<uint64_t>;
template class TiffImageReader<float>;
template class TiffImageReader<double>;
template class TiffImageReader<VoxelRGB>;
template class TiffImageReader<VoxelRGBA>;
template class TiffImageReader<VoxelHSV>;
// template class TiffImageReader<std::complex<float> >;
// template class TiffImageReader<std::complex<double> >;

} // namespace soma
