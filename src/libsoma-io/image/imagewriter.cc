//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter_d.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma
{

template class ImageWriter<int8_t>;
template class ImageWriter<int16_t>;
template class ImageWriter<int32_t>;
template class ImageWriter<int64_t>;
template class ImageWriter<uint8_t>;
template class ImageWriter<uint16_t>;
template class ImageWriter<uint32_t>;
template class ImageWriter<uint64_t>;
template class ImageWriter<float>;
template class ImageWriter<double>;
template class ImageWriter<VoxelRGB>;
template class ImageWriter<VoxelRGBA>;
template class ImageWriter<VoxelHSV>;
template class ImageWriter<cfloat>;
template class ImageWriter<cdouble>;
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
// char may be a different type from int8_t and uint8_t
template class ImageWriter<char>;
#endif

} // namespace soma
