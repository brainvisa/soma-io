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

#include <soma-io/cartodata/volumeformatreader_d.h>
#include <soma-io/reader/formatreader_d.h>
#include <soma-io/plugingis/gisimagereader_d.h>
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
    
    VolumeFormatReader<int8_t> *vfr8 = new VolumeFormatReader<int8_t>;
    vfr8->attach( rc_ptr<ImageReader<int8_t> >( new GisImageReader<int8_t> ) );
    FormatDictionary<Volume<int8_t> >::registerFormat( "GIS", vfr8, exts );
    
    VolumeFormatReader<int16_t> *vfr16 = new VolumeFormatReader<int16_t>;
    vfr16->attach( rc_ptr<ImageReader<int16_t> >( new GisImageReader<int16_t> ) );
    FormatDictionary<Volume<int16_t> >::registerFormat( "GIS", vfr16, exts );
    
    VolumeFormatReader<int32_t> *vfr32 = new VolumeFormatReader<int32_t>;
    vfr32->attach( rc_ptr<ImageReader<int32_t> >( new GisImageReader<int32_t> ) );
    FormatDictionary<Volume<int32_t> >::registerFormat( "GIS", vfr32, exts );
    
    //--- UINT -----------------------------------------------------------------
    
    VolumeFormatReader<uint8_t> *vfru8 = new VolumeFormatReader<uint8_t>;
    vfru8->attach( rc_ptr<ImageReader<uint8_t> >( new GisImageReader<uint8_t> ) );
    FormatDictionary<Volume<uint8_t> >::registerFormat( "GIS", vfru8, exts );
    
    VolumeFormatReader<uint16_t> *vfru16 = new VolumeFormatReader<uint16_t>;
    vfru16->attach( rc_ptr<ImageReader<uint16_t> >( new GisImageReader<uint16_t> ) );
    FormatDictionary<Volume<uint16_t> >::registerFormat( "GIS", vfru16, exts );
    
    VolumeFormatReader<uint32_t> *vfru32 = new VolumeFormatReader<uint32_t>;
    vfru32->attach( rc_ptr<ImageReader<uint32_t> >( new GisImageReader<uint32_t> ) );
    FormatDictionary<Volume<uint32_t> >::registerFormat( "GIS", vfru32, exts );
    
    //--- FLOAT ----------------------------------------------------------------
    
    VolumeFormatReader<float> *vfrf = new VolumeFormatReader<float>;
    vfrf->attach( rc_ptr<ImageReader<float> >( new GisImageReader<float> ) );
    FormatDictionary<Volume<float> >::registerFormat( "GIS", vfrf, exts );
    /*
    VolumeFormatReader<cfloat> *vfrcf = new VolumeFormatReader<cfloat>;
    vfrcf->attach( rc_ptr<ImageReader<cfloat> >( new GisImageReader<cfloat> ) );
    FormatDictionary<Volume<cfloat> >::registerFormat( "GIS", vfrcf, exts );
    */
    VolumeFormatReader<double> *vfrd = new VolumeFormatReader<double>;
    vfrd->attach( rc_ptr<ImageReader<double> >( new GisImageReader<double> ) );
    FormatDictionary<Volume<double> >::registerFormat( "GIS", vfrd, exts );
    /*
    VolumeFormatReader<cdouble> *vfrcd = new VolumeFormatReader<cdouble>;
    vfrcd->attach( rc_ptr<ImageReader<cdouble> >( new GisImageReader<cdouble> ) );
    FormatDictionary<Volume<cdouble> >::registerFormat( "GIS", vfrcd, exts );
    */
    //==========================================================================
    //   V O L U M E   R E F
    //==========================================================================
    
    //--- INT ------------------------------------------------------------------
    
    VolumeRefFormatReader<int8_t> *rfr8 = new VolumeRefFormatReader<int8_t>;
    rfr8->attach( rc_ptr<ImageReader<int8_t> >( new GisImageReader<int8_t> ) );
    FormatDictionary<rc_ptr<Volume<int8_t> > >::registerFormat( "GIS", rfr8, exts );
    
    VolumeRefFormatReader<int16_t> *rfr16 = new VolumeRefFormatReader<int16_t>;
    rfr16->attach( rc_ptr<ImageReader<int16_t> >( new GisImageReader<int16_t> ) );
    FormatDictionary<rc_ptr<Volume<int16_t> > >::registerFormat( "GIS", rfr16, exts );
    
    VolumeRefFormatReader<int32_t> *rfr32 = new VolumeRefFormatReader<int32_t>;
    rfr32->attach( rc_ptr<ImageReader<int32_t> >( new GisImageReader<int32_t> ) );
    FormatDictionary<rc_ptr<Volume<int32_t> > >::registerFormat( "GIS", rfr32, exts );
    
    //--- UINT -----------------------------------------------------------------
    
    VolumeRefFormatReader<uint8_t> *rfru8 = new VolumeRefFormatReader<uint8_t>;
    rfru8->attach( rc_ptr<ImageReader<uint8_t> >( new GisImageReader<uint8_t> ) );
    FormatDictionary<rc_ptr<Volume<uint8_t> > >::registerFormat( "GIS", rfru8, exts );
    
    VolumeRefFormatReader<uint16_t> *rfru16 = new VolumeRefFormatReader<uint16_t>;
    rfru16->attach( rc_ptr<ImageReader<uint16_t> >( new GisImageReader<uint16_t> ) );
    FormatDictionary<rc_ptr<Volume<uint16_t> > >::registerFormat( "GIS", rfru16, exts );
    
    VolumeRefFormatReader<uint32_t> *rfru32 = new VolumeRefFormatReader<uint32_t>;
    rfru32->attach( rc_ptr<ImageReader<uint32_t> >( new GisImageReader<uint32_t> ) );
    FormatDictionary<rc_ptr<Volume<uint32_t> > >::registerFormat( "GIS", rfru32, exts );
    
    //--- FLOAT ----------------------------------------------------------------
    
    VolumeRefFormatReader<float> *rfrf = new VolumeRefFormatReader<float>;
    rfrf->attach( rc_ptr<ImageReader<float> >( new GisImageReader<float> ) );
    FormatDictionary<rc_ptr<Volume<float> > >::registerFormat( "GIS", rfrf, exts );
    /*
    VolumeRefFormatReader<cfloat> *rfrcf = new VolumeRefFormatReader<cfloat>;
    rfrcf->attach( rc_ptr<ImageReader<cfloat> >( new GisImageReader<cfloat> ) );
    FormatDictionary<rc_ptr<Volume<cfloat> > >::registerFormat( "GIS", rfrcf, exts );
    */
    VolumeRefFormatReader<double> *rfrd = new VolumeRefFormatReader<double>;
    rfrd->attach( rc_ptr<ImageReader<double> >( new GisImageReader<double> ) );
    FormatDictionary<rc_ptr<Volume<double> > >::registerFormat( "GIS", rfrd, exts );
    /*
    VolumeRefFormatReader<cdouble> *rfrcd = new VolumeRefFormatReader<cdouble>;
    rfrcd->attach( rc_ptr<ImageReader<cdouble> >( new GisImageReader<cdouble> ) );
    FormatDictionary<rc_ptr<Volume<cdouble> > >::registerFormat( "GIS", rfrcd, exts );
    */
    return true;
  }

  bool  _dummy = initMe();
}
template class VolumeFormatReader<int8_t>;
template class VolumeFormatReader<int16_t>;
template class VolumeFormatReader<int32_t>;
template class VolumeFormatReader<uint8_t>;
template class VolumeFormatReader<uint16_t>;
template class VolumeFormatReader<uint32_t>;
template class VolumeFormatReader<float>;
template class VolumeFormatReader<double>;

template class VolumeRefFormatReader<int8_t>;
template class VolumeRefFormatReader<int16_t>;
template class VolumeRefFormatReader<int32_t>;
template class VolumeRefFormatReader<uint8_t>;
template class VolumeRefFormatReader<uint16_t>;
template class VolumeRefFormatReader<uint32_t>;
template class VolumeRefFormatReader<float>;
template class VolumeRefFormatReader<double>;