#ifndef CARTOBASE_TYPE_RGBA_DECL_H
#define CARTOBASE_TYPE_RGBA_DECL_H
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/type/voxelvalue.h>
//----------------------------------------------------------------------------

/*****************************************************************************
 * Even though declarations are here, this is NOT the file you want to
 * include to use VoxelRGBA.
 * Since all methods are inline and for readability issues, declarations
 * are contained in "_decl.h" file and definitions in "_def.h" file.
 * You should include ".h" file which binds all necessary headers.
 ****************************************************************************/

namespace carto {

  class VoxelRGBA;
  class VoxelRGB;

  template<> inline std::string DataTypeCode<VoxelRGBA>::dataType()
  {
    return "RGBA";
  }

  /// RGBA Value
  ///
  /// This class replaces AimsRGBA. \n
  /// All previous operators are implemented. \n
  /// Read/Write operators are implemented in
  /// \c soma-io/utilities/asciidatasourcetraits.h so you need to include this
  /// file for read/write operations.
  ///
  /// \warning Even though declarations are contained in
  /// \c cartobase/type/voxelrgba_decl.h, you need to include
  /// \c cartobase/type/voxelrgba.h in order to use this class
  /// (all methods are inline).
  class VoxelRGBA : public VoxelValue<uint8_t,4>
  {
    public:
      //=== TYPES =====================================================
      typedef uint8_t ChannelType;
      
      //=== CONSTRUCTORS =====================================================
      VoxelRGBA( const VoxelRGBA &other );
      VoxelRGBA( const VoxelRGB &other);
      VoxelRGBA( const VoxelValue<uint8_t,4> &other );
      VoxelRGBA( const uint8_t &r = 0, const uint8_t &g = 0, 
                 const uint8_t &b = 0, const uint8_t &a = 0 );
      ~VoxelRGBA();
      
      //=== AFFECTATION ======================================================
      VoxelRGBA & operator = ( const VoxelRGBA & other );
      VoxelRGBA & operator = ( const VoxelRGB  & other );
      VoxelRGBA & operator = ( const uint8_t   & value );
      
      //=== OPERATORS ========================================================
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
      
      //=== ACCESSORS ========================================================
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
  VoxelRGBA operator + (const VoxelRGBA &aa, const uint16_t  &bb);
  VoxelRGBA operator + (const VoxelRGBA &aa, const uint32_t  &bb);
  VoxelRGBA operator + (const VoxelRGBA &aa, const uint64_t  &bb);
  VoxelRGBA operator + (const VoxelRGBA &aa, const float     &bb);
  VoxelRGBA operator + (const VoxelRGBA &aa, const double    &bb);
  VoxelRGBA operator + (const uint8_t   &aa, const VoxelRGBA &bb);
  VoxelRGBA operator + (const uint16_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator + (const uint32_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator + (const uint64_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator + (const float     &aa, const VoxelRGBA &bb);
  VoxelRGBA operator + (const double    &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const uint8_t   &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const uint16_t  &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const uint32_t  &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const uint64_t  &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const float     &bb);
  VoxelRGBA operator - (const VoxelRGBA &aa, const double    &bb);
  VoxelRGBA operator - (const uint8_t   &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const uint16_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const uint32_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const uint64_t  &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const float     &aa, const VoxelRGBA &bb);
  VoxelRGBA operator - (const double    &aa, const VoxelRGBA &bb);
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

  template <>
  struct KeyComparator<carto::VoxelRGBA>
  {
    static inline bool less( const carto::VoxelRGBA & lhs, const carto::VoxelRGBA & rhs );
  };
}

#endif
