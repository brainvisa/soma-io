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

#ifndef SOMAIO_IMAGE_RGBA_H
#define SOMAIO_IMAGE_RGBA_H
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
  template<> inline std::string DataTypeCode<soma::VoxelRGBA>::dataType()
  {
    return "RGBA";
  }
}

namespace soma {

  class VoxelRGBA : public soma::VoxelValue<uint8_t,4>
  {
    public:
      //=== CONSTRUCTORS =======================================================
      VoxelRGBA( const VoxelRGBA &other );
      VoxelRGBA( const VoxelRGB &other);
      VoxelRGBA( const soma::VoxelValue<uint8_t,4> &other );
      VoxelRGBA( const uint8_t &r = 0, const uint8_t &g = 0, 
                 const uint8_t &b = 0, const uint8_t &a = 0 );
      ~VoxelRGBA();
      
      //=== AFFECTATION ========================================================
      VoxelRGBA & operator = ( const VoxelRGBA & other );
      VoxelRGBA & operator = ( const VoxelRGB  & other );
      VoxelRGBA & operator = ( const uint8_t   & value );
      
      //=== OPERATORS ==========================================================
      VoxelRGBA & operator += ( const VoxelRGBA & other );
      VoxelRGBA & operator += ( const VoxelRGB  & other );
      VoxelRGBA & operator -= ( const VoxelRGBA & other );
      VoxelRGBA & operator -= ( const VoxelRGB  & other );
      VoxelRGBA & operator += ( const uint8_t   & value );
      VoxelRGBA & operator -= ( const uint8_t   & value );
      VoxelRGBA & operator *= ( const uint8_t   & value );
      VoxelRGBA & operator *= ( const uint16_t  & value );
      VoxelRGBA & operator *= ( const uint32_t  & value );
      VoxelRGBA & operator *= ( const uint64_t  & value );
      VoxelRGBA & operator *= ( const float     & value );
      VoxelRGBA & operator *= ( const double    & value );
      VoxelRGBA & operator /= ( const uint8_t   & value );
      VoxelRGBA & operator /= ( const uint16_t  & value );
      VoxelRGBA & operator /= ( const uint32_t  & value );
      VoxelRGBA & operator /= ( const uint64_t  & value );
      VoxelRGBA & operator /= ( const float     & value );
      VoxelRGBA & operator /= ( const double    & value );
      // long int are still used in volumes
      VoxelRGBA & operator *= ( const long      & value );
      VoxelRGBA & operator /= ( const long      & value );
      
      //=== ACCESSORS ==========================================================
      inline const uint8_t& red   () const { return (*this)[0]; }
      inline const uint8_t& green () const { return (*this)[1]; }
      inline const uint8_t& blue  () const { return (*this)[2]; }
      inline const uint8_t& alpha () const { return (*this)[3]; }
      inline       uint8_t& red   ()       { return (*this)[0]; }
      inline       uint8_t& green ()       { return (*this)[1]; }
      inline       uint8_t& blue  ()       { return (*this)[2]; }
      inline       uint8_t& alpha ()       { return (*this)[3]; }
  };

  VoxelRGBA operator + (const VoxelRGBA &aa, const VoxelRGBA &bb);
  VoxelRGBA operator + (const VoxelRGBA &aa, const uint8_t   &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const uint8_t   &bb);
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint8_t   &bb);
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint16_t  &bb);
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint32_t  &bb);
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint64_t  &bb);
  VoxelRGBA operator * (const VoxelRGBA &aa, const float     &bb);
  VoxelRGBA operator * (const VoxelRGBA &aa, const double    &bb);
  VoxelRGBA operator * (const uint8_t   &aa, const VoxelRGBA &bb);
  VoxelRGBA operator * (const uint16_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator * (const uint32_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator * (const uint64_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator * (const float     &aa, const VoxelRGBA &bb);
  VoxelRGBA operator * (const double    &aa, const VoxelRGBA &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint8_t   &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint16_t  &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint32_t  &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint64_t  &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const float     &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const double    &bb);
  // long int are still used in volumes
  VoxelRGBA operator * (const VoxelRGBA &aa, const long      &bb);
  VoxelRGBA operator * (const long      &aa, const VoxelRGBA &bb);
  VoxelRGBA operator / (const VoxelRGBA &aa, const long      &bb);

  //--- AsciiDataSourceTraits
  
  template <> inline
  bool AsciiDataSourceTraits<VoxelRGBA>::read( DataSource & ds, 
                                               VoxelRGBA & item )
  {
    return AsciiDataSourceTraits<VoxelValue<uint8_t,4> >::read( ds, item );
  }
  
  template <> inline
  bool AsciiDataSourceTraits<VoxelRGBA>::write( DataSource & ds, 
                                                const VoxelRGBA & item )
  {
    return AsciiDataSourceTraits<VoxelValue<uint8_t,4> >::write( ds, item );
  }
  
  inline DataSource & 
  operator << ( DataSource & ds, const VoxelRGBA & x )
  {
    AsciiDataSourceTraits<VoxelRGBA>::write( ds, x );
    return ds;
  }

}

#endif