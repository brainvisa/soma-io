//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader_d.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma
{

template class ImageReader<int8_t>;
template class ImageReader<int16_t>;
template class ImageReader<int32_t>;
template class ImageReader<int64_t>;
template class ImageReader<uint8_t>;
template class ImageReader<uint16_t>;
template class ImageReader<uint32_t>;
template class ImageReader<uint64_t>;
template class ImageReader<float>;
template class ImageReader<double>;
template class ImageReader<VoxelRGB>;
template class ImageReader<VoxelRGBA>;
template class ImageReader<VoxelHSV>;
template class ImageReader<cfloat>;
template class ImageReader<cdouble>;
#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
// char may be a different type from int8_t and uint8_t
template class ImageReader<char>;
#endif

} // namespace soma
