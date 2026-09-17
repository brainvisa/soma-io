//--- plugin -----------------------------------------------------------------
#include <soma-io/image/mifimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class MifImageReader<int8_t>;
template class MifImageReader<int16_t>;
template class MifImageReader<int32_t>;
template class MifImageReader<uint8_t>;
template class MifImageReader<uint16_t>;
template class MifImageReader<uint32_t>;
template class MifImageReader<float>;
template class MifImageReader<double>;
template class MifImageReader<std::complex<float> >;
template class MifImageReader<std::complex<double> >;

} // namespace soma
