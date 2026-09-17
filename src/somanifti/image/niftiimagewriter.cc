//--- plugin -----------------------------------------------------------------
#include <soma-io/image/niftiimagewriter_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma {

template class NiftiImageWriter<int8_t>;
template class Nifti1ImageWriter<int8_t>;
template class Nifti2ImageWriter<int8_t>;

template class NiftiImageWriter<int16_t>;
template class Nifti1ImageWriter<int16_t>;
template class Nifti2ImageWriter<int16_t>;

template class NiftiImageWriter<int32_t>;
template class Nifti1ImageWriter<int32_t>;
template class Nifti2ImageWriter<int32_t>;


template class NiftiImageWriter<int64_t>;
template class Nifti1ImageWriter<int64_t>;
template class Nifti2ImageWriter<int64_t>;

template class NiftiImageWriter<uint8_t>;
template class Nifti1ImageWriter<uint8_t>;
template class Nifti2ImageWriter<uint8_t>;

template class NiftiImageWriter<uint16_t>;
template class Nifti1ImageWriter<uint16_t>;
template class Nifti2ImageWriter<uint16_t>;

template class NiftiImageWriter<uint32_t>;
template class Nifti1ImageWriter<uint32_t>;
template class Nifti2ImageWriter<uint32_t>;

template class NiftiImageWriter<uint64_t>;
template class Nifti1ImageWriter<uint64_t>;
template class Nifti2ImageWriter<uint64_t>;

template class NiftiImageWriter<float>;
template class Nifti1ImageWriter<float>;
template class Nifti2ImageWriter<float>;

template class NiftiImageWriter<double>;
template class Nifti1ImageWriter<double>;
template class Nifti2ImageWriter<double>;

template class NiftiImageWriter<VoxelRGB>;
template class Nifti1ImageWriter<VoxelRGB>;
template class Nifti2ImageWriter<VoxelRGB>;

template class NiftiImageWriter<VoxelRGBA>;
template class Nifti1ImageWriter<VoxelRGBA>;
template class Nifti2ImageWriter<VoxelRGBA>;

//template class NiftiImageWriter<VoxelHSV>;
//template class Nifti1ImageWriter<VoxelHSV>;
//template class Nifti2ImageWriter<VoxelHSV>;

template class NiftiImageWriter<cfloat>;
template class Nifti1ImageWriter<cfloat>;
template class Nifti2ImageWriter<cfloat>;

template class NiftiImageWriter<cdouble>;
template class Nifti1ImageWriter<cdouble>;
template class Nifti2ImageWriter<cdouble>;

} // namespace soma
