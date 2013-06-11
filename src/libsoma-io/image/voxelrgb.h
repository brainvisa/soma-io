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

#ifndef SOMAIO_IMAGE_RGB_H
#define SOMAIO_IMAGE_RGB_H
//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/voxelvalue_d.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/type/types.h>
//--- system -------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace soma {
  class VoxelRGBA;
  class VoxelRGB;
}

namespace carto {
  template<> inline std::string DataTypeCode<soma::VoxelRGB>::dataType()
  {
    return "RGB";
  }
}

namespace soma {

  class VoxelRGB : public soma::VoxelValue<uint8_t,3>
  {
    public:
      //=== CONSTRUCTORS =======================================================
      VoxelRGB( const VoxelRGB &other );
      VoxelRGB( const VoxelRGBA &other );
      VoxelRGB( const soma::VoxelValue<uint8_t,3> &other );
      VoxelRGB( const uint8_t &r = 0, const uint8_t &g = 0, const uint8_t &b = 0 );
      ~VoxelRGB();
      
      //=== AFFECTATION ========================================================
      VoxelRGB & operator = ( const VoxelRGB  & other );
      VoxelRGB & operator = ( const VoxelRGBA & other );
      VoxelRGB & operator = ( const uint8_t   & value );
      
      //=== OPERATORS ==========================================================
      VoxelRGB & operator += ( const VoxelRGB  & other );
      VoxelRGB & operator += ( const VoxelRGBA & other );
      VoxelRGB & operator -= ( const VoxelRGB  & other );
      VoxelRGB & operator -= ( const VoxelRGBA & other );
      VoxelRGB & operator += ( const uint8_t   & value );
      VoxelRGB & operator -= ( const uint8_t   & value );
      VoxelRGB & operator *= ( const uint8_t   & value );
      VoxelRGB & operator *= ( const uint16_t  & value );
      VoxelRGB & operator *= ( const uint32_t  & value );
      VoxelRGB & operator *= ( const uint64_t  & value );
      VoxelRGB & operator *= ( const float     & value );
      VoxelRGB & operator *= ( const double    & value );
      VoxelRGB & operator /= ( const uint8_t   & value );
      VoxelRGB & operator /= ( const uint16_t  & value );
      VoxelRGB & operator /= ( const uint32_t  & value );
      VoxelRGB & operator /= ( const uint64_t  & value );
      VoxelRGB & operator /= ( const float     & value );
      VoxelRGB & operator /= ( const double    & value );
      // long int are still used in volumes
      VoxelRGB & operator *= ( const long      & value );
      VoxelRGB & operator /= ( const long      & value );
      
      //=== ACCESSORS ==========================================================
      inline const uint8_t& red  () const { return (*this)[0]; }
      inline const uint8_t& green() const { return (*this)[1]; }
      inline const uint8_t& blue () const { return (*this)[2]; }
      inline       uint8_t& red  ()       { return (*this)[0]; }
      inline       uint8_t& green()       { return (*this)[1]; }
      inline       uint8_t& blue ()       { return (*this)[2]; }
  };

  VoxelRGB operator + (const VoxelRGB &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const uint8_t  &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const uint8_t  &bb);
  VoxelRGB operator * (const VoxelRGB &aa, const uint8_t  &bb);
  VoxelRGB operator * (const VoxelRGB &aa, const uint16_t &bb);
  VoxelRGB operator * (const VoxelRGB &aa, const uint32_t &bb);
  VoxelRGB operator * (const VoxelRGB &aa, const uint64_t &bb);
  VoxelRGB operator * (const VoxelRGB &aa, const float    &bb);
  VoxelRGB operator * (const VoxelRGB &aa, const double   &bb);
  VoxelRGB operator * (const uint8_t  &aa, const VoxelRGB &bb);
  VoxelRGB operator * (const uint16_t &aa, const VoxelRGB &bb);
  VoxelRGB operator * (const uint32_t &aa, const VoxelRGB &bb);
  VoxelRGB operator * (const uint64_t &aa, const VoxelRGB &bb);
  VoxelRGB operator * (const float    &aa, const VoxelRGB &bb);
  VoxelRGB operator * (const double   &aa, const VoxelRGB &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const uint8_t  &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const uint16_t &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const uint32_t &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const uint64_t &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const float    &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const double   &bb);
  // long int are still used in volumes
  VoxelRGB operator * (const VoxelRGB &aa, const long     &bb);
  VoxelRGB operator * (const long     &aa, const VoxelRGB &bb);
  VoxelRGB operator / (const VoxelRGB &aa, const long     &bb);
  
  //--- AsciiDataSourceTraits
  
  template <> inline
  bool AsciiDataSourceTraits<VoxelRGB>::read( DataSource & ds, 
                                              VoxelRGB & item )
  {
    return AsciiDataSourceTraits<VoxelValue<uint8_t,3> >::read( ds, item );
  }
  
  template <> inline
  bool AsciiDataSourceTraits<VoxelRGB>::write( DataSource & ds, 
                                               const VoxelRGB & item )
  {
    return AsciiDataSourceTraits<VoxelValue<uint8_t,3> >::write( ds, item );
  }
  
  inline DataSource & 
  operator << ( DataSource & ds, const VoxelRGB & x )
  {
    AsciiDataSourceTraits<VoxelRGB>::write( ds, x );
    return ds;
  }

}

#endif