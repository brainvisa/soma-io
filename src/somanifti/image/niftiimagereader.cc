//--- plugin -----------------------------------------------------------------
#include <soma-io/image/niftiimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class NiftiImageReader<int8_t>;
template class NiftiImageReader<int16_t>;
template class NiftiImageReader<int32_t>;
template class NiftiImageReader<int64_t>;
template class NiftiImageReader<uint8_t>;
template class NiftiImageReader<uint16_t>;
template class NiftiImageReader<uint32_t>;
template class NiftiImageReader<uint64_t>;
template class NiftiImageReader<float>;
template class NiftiImageReader<double>;
template class NiftiImageReader<VoxelRGB>;
template class NiftiImageReader<VoxelRGBA>;
//template class NiftiImageReader<VoxelHSV>;
template class NiftiImageReader<std::complex<float> >;
template class NiftiImageReader<std::complex<double> >;

} // namespace soma
