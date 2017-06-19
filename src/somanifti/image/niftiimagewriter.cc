/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

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
