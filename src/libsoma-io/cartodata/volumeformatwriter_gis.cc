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

#include <soma-io/cartodata/volumeformatwriter_d.h>
//#include <soma-io/writer/formatwriter_d.h>
#include <soma-io/plugingis/gisimagewriter_d.h>
#include <soma-io/io/formatdictionary_d.h>
#include <cartobase/type/types.h>

using namespace moncartodata;
using namespace soma;
using namespace carto;
using namespace std;

namespace
{
  bool initMe()
  {
    vector<string>  exts(2);
    exts[0] = "ima";
    exts[1] = "dim";
    
    //==========================================================================
    //   V O L U M E
    //==========================================================================
    
    //--- INT ------------------------------------------------------------------
    
    VolumeFormatWriter<int8_t> *vfr8 = new VolumeFormatWriter<int8_t>;
    vfr8->attach( rc_ptr<ImageWriter<int8_t> >( new GisImageWriter<int8_t> ) );
    FormatDictionary<Volume<int8_t> >::registerFormat( "GIS", vfr8, exts );
    
    VolumeFormatWriter<int16_t> *vfr16 = new VolumeFormatWriter<int16_t>;
    vfr16->attach( rc_ptr<ImageWriter<int16_t> >( new GisImageWriter<int16_t> ) );
    FormatDictionary<Volume<int16_t> >::registerFormat( "GIS", vfr16, exts );
    
    VolumeFormatWriter<int32_t> *vfr32 = new VolumeFormatWriter<int32_t>;
    vfr32->attach( rc_ptr<ImageWriter<int32_t> >( new GisImageWriter<int32_t> ) );
    FormatDictionary<Volume<int32_t> >::registerFormat( "GIS", vfr32, exts );
    
    //--- UINT -----------------------------------------------------------------
    
    VolumeFormatWriter<uint8_t> *vfru8 = new VolumeFormatWriter<uint8_t>;
    vfru8->attach( rc_ptr<ImageWriter<uint8_t> >( new GisImageWriter<uint8_t> ) );
    FormatDictionary<Volume<uint8_t> >::registerFormat( "GIS", vfru8, exts );
    
    VolumeFormatWriter<uint16_t> *vfru16 = new VolumeFormatWriter<uint16_t>;
    vfru16->attach( rc_ptr<ImageWriter<uint16_t> >( new GisImageWriter<uint16_t> ) );
    FormatDictionary<Volume<uint16_t> >::registerFormat( "GIS", vfru16, exts );
    
    VolumeFormatWriter<uint32_t> *vfru32 = new VolumeFormatWriter<uint32_t>;
    vfru32->attach( rc_ptr<ImageWriter<uint32_t> >( new GisImageWriter<uint32_t> ) );
    FormatDictionary<Volume<uint32_t> >::registerFormat( "GIS", vfru32, exts );
    
    //--- FLOAT ----------------------------------------------------------------
    
    VolumeFormatWriter<float> *vfrf = new VolumeFormatWriter<float>;
    vfrf->attach( rc_ptr<ImageWriter<float> >( new GisImageWriter<float> ) );
    FormatDictionary<Volume<float> >::registerFormat( "GIS", vfrf, exts );
    /*
    VolumeFormatWriter<cfloat> *vfrcf = new VolumeFormatWriter<cfloat>;
    vfrcf->attach( rc_ptr<ImageWriter<cfloat> >( new GisImageWriter<cfloat> ) );
    FormatDictionary<Volume<cfloat> >::registerFormat( "GIS", vfrcf, exts );
    */
    VolumeFormatWriter<double> *vfrd = new VolumeFormatWriter<double>;
    vfrd->attach( rc_ptr<ImageWriter<double> >( new GisImageWriter<double> ) );
    FormatDictionary<Volume<double> >::registerFormat( "GIS", vfrd, exts );
    /*
    VolumeFormatWriter<cdouble> *vfrcd = new VolumeFormatWriter<cdouble>;
    vfrcd->attach( rc_ptr<ImageWriter<cdouble> >( new GisImageWriter<cdouble> ) );
    FormatDictionary<Volume<cdouble> >::registerFormat( "GIS", vfrcd, exts );
    */
    //==========================================================================
    //   V O L U M E   R E F
    //==========================================================================
    
    //--- INT ------------------------------------------------------------------
    
    VolumeRefFormatWriter<int8_t> *rfr8 = new VolumeRefFormatWriter<int8_t>;
    rfr8->attach( rc_ptr<ImageWriter<int8_t> >( new GisImageWriter<int8_t> ) );
    FormatDictionary<VolumeRef<int8_t> >::registerFormat( "GIS", rfr8, exts );
    
    VolumeRefFormatWriter<int16_t> *rfr16 = new VolumeRefFormatWriter<int16_t>;
    rfr16->attach( rc_ptr<ImageWriter<int16_t> >( new GisImageWriter<int16_t> ) );
    FormatDictionary<VolumeRef<int16_t> >::registerFormat( "GIS", rfr16, exts );
    
    VolumeRefFormatWriter<int32_t> *rfr32 = new VolumeRefFormatWriter<int32_t>;
    rfr32->attach( rc_ptr<ImageWriter<int32_t> >( new GisImageWriter<int32_t> ) );
    FormatDictionary<VolumeRef<int32_t> >::registerFormat( "GIS", rfr32, exts );
    
    //--- UINT -----------------------------------------------------------------
    
    VolumeRefFormatWriter<uint8_t> *rfru8 = new VolumeRefFormatWriter<uint8_t>;
    rfru8->attach( rc_ptr<ImageWriter<uint8_t> >( new GisImageWriter<uint8_t> ) );
    FormatDictionary<VolumeRef<uint8_t> >::registerFormat( "GIS", rfru8, exts );
    
    VolumeRefFormatWriter<uint16_t> *rfru16 = new VolumeRefFormatWriter<uint16_t>;
    rfru16->attach( rc_ptr<ImageWriter<uint16_t> >( new GisImageWriter<uint16_t> ) );
    FormatDictionary<VolumeRef<uint16_t> >::registerFormat( "GIS", rfru16, exts );
    
    VolumeRefFormatWriter<uint32_t> *rfru32 = new VolumeRefFormatWriter<uint32_t>;
    rfru32->attach( rc_ptr<ImageWriter<uint32_t> >( new GisImageWriter<uint32_t> ) );
    FormatDictionary<VolumeRef<uint32_t> >::registerFormat( "GIS", rfru32, exts );
    
    //--- FLOAT ----------------------------------------------------------------
    
    VolumeRefFormatWriter<float> *rfrf = new VolumeRefFormatWriter<float>;
    rfrf->attach( rc_ptr<ImageWriter<float> >( new GisImageWriter<float> ) );
    FormatDictionary<VolumeRef<float> >::registerFormat( "GIS", rfrf, exts );
    /*
    VolumeRefFormatWriter<cfloat> *rfrcf = new VolumeRefFormatWriter<cfloat>;
    rfrcf->attach( rc_ptr<ImageWriter<cfloat> >( new GisImageWriter<cfloat> ) );
    FormatDictionary<VolumeRef<cfloat> >::registerFormat( "GIS", rfrcf, exts );
    */
    VolumeRefFormatWriter<double> *rfrd = new VolumeRefFormatWriter<double>;
    rfrd->attach( rc_ptr<ImageWriter<double> >( new GisImageWriter<double> ) );
    FormatDictionary<VolumeRef<double> >::registerFormat( "GIS", rfrd, exts );
    /*
    VolumeRefFormatWriter<cdouble> *rfrcd = new VolumeRefFormatWriter<cdouble>;
    rfrcd->attach( rc_ptr<ImageWriter<cdouble> >( new GisImageWriter<cdouble> ) );
    FormatDictionary<VolumeRef<cdouble> >::registerFormat( "GIS", rfrcd, exts );
    */
    return true;
  }

  bool  _dummy = initMe();
}
template class VolumeFormatWriter<int8_t>;
template class VolumeFormatWriter<int16_t>;
template class VolumeFormatWriter<int32_t>;
template class VolumeFormatWriter<uint8_t>;
template class VolumeFormatWriter<uint16_t>;
template class VolumeFormatWriter<uint32_t>;
template class VolumeFormatWriter<float>;
template class VolumeFormatWriter<double>;

template class VolumeRefFormatWriter<int8_t>;
template class VolumeRefFormatWriter<int16_t>;
template class VolumeRefFormatWriter<int32_t>;
template class VolumeRefFormatWriter<uint8_t>;
template class VolumeRefFormatWriter<uint16_t>;
template class VolumeRefFormatWriter<uint32_t>;
template class VolumeRefFormatWriter<float>;
template class VolumeRefFormatWriter<double>;