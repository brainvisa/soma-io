#ifndef CARTOBASE_TYPE_RGB_DECL_H
#define CARTOBASE_TYPE_RGB_DECL_H
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/voxelvalue.h>
#include <cartobase/type/types.h>
//----------------------------------------------------------------------------

/*****************************************************************************
 * Even though declarations are here, this is NOT the file you want to
 * include to use VoxelRGB.
 * Since all methods are inline and for readability issues, declarations
 * are contained in "_decl.h" file and definitions in "_def.h" file.
 * You should include ".h" file which binds all necessary headers.
 ****************************************************************************/

namespace carto {

  class VoxelRGBA;
  class VoxelRGB;

  template<> inline std::string DataTypeCode<VoxelRGB>::dataType()
  {
    return "RGB";
  }

  /// RGB Value
  ///
  /// This class replaces AimsRGB. \n
  /// All previous operators are implemented. \n
  /// Read/Write operators are implemented in
  /// \c soma-io/utilities/asciidatasourcetraits.h so you need to include this
  /// file for read/write operations.
  ///
  /// \warning Even though declarations are contained in
  /// \c cartobase/type/voxelrgb_decl.h, you need to include
  /// \c cartobase/type/voxelrgb.h in order to use this class
  /// (all methods are inline).
  class VoxelRGB : public VoxelValue<uint8_t,3>
  {
    public:

      //=== CONSTRUCTORS =====================================================
      VoxelRGB( const VoxelRGB &other );
      VoxelRGB( const VoxelRGBA &other );
      VoxelRGB( const VoxelValue<uint8_t,3> &other );
      VoxelRGB( const uint8_t &r = 0,
                const uint8_t &g = 0,
                const uint8_t &b = 0 );
      ~VoxelRGB();
      
      //=== AFFECTATION ======================================================
      VoxelRGB & operator = ( const VoxelRGB  & other );
      VoxelRGB & operator = ( const VoxelRGBA & other );
      VoxelRGB & operator = ( const uint8_t   & value );
      
      //=== OPERATORS ========================================================
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
      
      //=== ACCESSORS ========================================================
      inline const uint8_t& red  () const { return (*this)[0]; }
      inline const uint8_t& green() const { return (*this)[1]; }
      inline const uint8_t& blue () const { return (*this)[2]; }
      inline       uint8_t& red  ()       { return (*this)[0]; }
      inline       uint8_t& green()       { return (*this)[1]; }
      inline       uint8_t& blue ()       { return (*this)[2]; }
  };

  VoxelRGB operator + (const VoxelRGB &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const uint8_t  &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const uint16_t &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const uint32_t &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const uint64_t &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const float    &bb);
  VoxelRGB operator + (const VoxelRGB &aa, const double   &bb);
  VoxelRGB operator + (const uint8_t  &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const uint16_t &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const uint32_t &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const uint64_t &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const float    &aa, const VoxelRGB &bb);
  VoxelRGB operator + (const double   &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const uint8_t  &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const uint16_t &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const uint32_t &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const uint64_t &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const float    &bb);
  VoxelRGB operator - (const VoxelRGB &aa, const double   &bb);  
  VoxelRGB operator - (const uint8_t  &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const uint16_t &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const uint32_t &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const uint64_t &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const float    &aa, const VoxelRGB &bb);
  VoxelRGB operator - (const double   &aa, const VoxelRGB &bb);
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

  template <>
  struct KeyComparator<carto::VoxelRGB>
  {
    static inline bool less( const carto::VoxelRGB & lhs, const carto::VoxelRGB & rhs );
  };
}

#endif
