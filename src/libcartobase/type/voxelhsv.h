#ifndef CARTOBASE_TYPE_HSV_H
#define CARTOBASE_TYPE_HSV_H
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/type/voxelvalue.h>
#include <cartobase/exception/assert.h>
//--- system -----------------------------------------------------------------
//#define CARTO_DEBUG_RGB
#ifdef CARTO_DEBUG_RGB
  #include <iostream>
#endif
//----------------------------------------------------------------------------

namespace carto {

  class VoxelHSV;

  template<> inline std::string DataTypeCode<VoxelHSV>::dataType()
  {
    return "HSV";
  }

  //==========================================================================
  //   D E C L A R A T I O N
  //==========================================================================

  /// HSV Value
  ///
  /// This class replaces AimsHSV. \n
  /// All previous operators are implemented. \n
  /// Read/Write operators are implemented in
  /// \c soma-io/utilities/asciidatasourcetraits.h so you need to include this
  /// file for read/write operations.
  class VoxelHSV : public VoxelValue<uint8_t,3>
  {
    public:
      //=== CONSTRUCTORS =====================================================
      VoxelHSV( const VoxelHSV &other );
      VoxelHSV( const VoxelValue<uint8_t,3> &other );
      VoxelHSV( const uint8_t &h = 0,
                const uint8_t &s = 0,
                const uint8_t &v = 0 );
      ~VoxelHSV();
      
      //=== AFFECTATION ======================================================
      VoxelHSV & operator = ( const VoxelHSV & other );
      VoxelHSV & operator = ( const uint8_t & val );
      
      //=== OPERATORS ========================================================
      VoxelHSV & operator += ( const VoxelHSV & other );
      VoxelHSV & operator -= ( const VoxelHSV & other );
      VoxelHSV & operator += ( const uint8_t  & val );
      VoxelHSV & operator -= ( const uint8_t  & val );
      VoxelHSV & operator *= ( const uint8_t  & val );
      VoxelHSV & operator *= ( const uint16_t & val );
      VoxelHSV & operator *= ( const uint32_t & val );
      VoxelHSV & operator *= ( const uint64_t & val );
      VoxelHSV & operator *= ( const float    & val );
      VoxelHSV & operator *= ( const double   & val );
      VoxelHSV & operator /= ( const uint8_t  & val );
      VoxelHSV & operator /= ( const uint16_t & val );
      VoxelHSV & operator /= ( const uint32_t & val );
      VoxelHSV & operator /= ( const uint64_t & val );
      VoxelHSV & operator /= ( const float    & val );
      VoxelHSV & operator /= ( const double   & val );
      // long int are still used in volumes
      VoxelHSV & operator *= ( const long     & val );
      VoxelHSV & operator /= ( const long     & val );
      
      //=== ACCESSORS ========================================================
      inline const uint8_t& hue        () const { return (*this)[0]; }
      inline const uint8_t& saturation () const { return (*this)[1]; }
      inline const uint8_t& value      () const { return (*this)[2]; }
      inline       uint8_t& hue        ()       { return (*this)[0]; }
      inline       uint8_t& saturation ()       { return (*this)[1]; }
      inline       uint8_t& value      ()       { return (*this)[2]; }
  };

  VoxelHSV operator + (const VoxelHSV &aa, const VoxelHSV &bb);
  VoxelHSV operator + (const VoxelHSV &aa, const uint8_t  &bb);
  VoxelHSV operator + (const uint8_t  &aa, const VoxelHSV &bb);
  VoxelHSV operator - (const VoxelHSV &aa, const VoxelHSV &bb);
  VoxelHSV operator - (const VoxelHSV &aa, const uint8_t  &bb);
  VoxelHSV operator - (const uint8_t  &aa, const VoxelHSV &bb);
  VoxelHSV operator * (const VoxelHSV &aa, const uint8_t  &bb);
  VoxelHSV operator * (const VoxelHSV &aa, const uint16_t &bb);
  VoxelHSV operator * (const VoxelHSV &aa, const uint32_t &bb);
  VoxelHSV operator * (const VoxelHSV &aa, const uint64_t &bb);
  VoxelHSV operator * (const VoxelHSV &aa, const float    &bb);
  VoxelHSV operator * (const VoxelHSV &aa, const double   &bb);
  VoxelHSV operator * (const uint8_t  &aa, const VoxelHSV &bb);
  VoxelHSV operator * (const uint16_t &aa, const VoxelHSV &bb);
  VoxelHSV operator * (const uint32_t &aa, const VoxelHSV &bb);
  VoxelHSV operator * (const uint64_t &aa, const VoxelHSV &bb);
  VoxelHSV operator * (const float    &aa, const VoxelHSV &bb);
  VoxelHSV operator * (const double   &aa, const VoxelHSV &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const uint8_t  &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const uint16_t &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const uint32_t &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const uint64_t &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const float    &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const double   &bb);
  // long int are still used in volumes
  VoxelHSV operator * (const VoxelHSV &aa, const long     &bb);
  VoxelHSV operator * (const long     &aa, const VoxelHSV &bb);
  VoxelHSV operator / (const VoxelHSV &aa, const long     &bb);

  template <>
  struct KeyComparator<carto::VoxelHSV>
  {
    static inline bool less( const carto::VoxelHSV & lhs, const carto::VoxelHSV & rhs );
  };

  //==========================================================================
  //   D E F I N I T I O N
  //==========================================================================

  //=== CONSTRUCTORS =========================================================

  inline
  VoxelHSV::VoxelHSV( const VoxelHSV &other )
  : VoxelValue<uint8_t,3>( other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: Constructor( HSV )" << std::endl;
    #endif
  }

  inline
  VoxelHSV::VoxelHSV( const VoxelValue<uint8_t,3> &other )
  : VoxelValue<uint8_t,3>( other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: Constructor( VV<3> )" << std::endl;
    #endif
  }

  inline
  VoxelHSV::VoxelHSV( const uint8_t &h, const uint8_t &s, const uint8_t &v )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: Constructor( h,s,v )" << std::endl;
    #endif
    hue()        = h;
    saturation() = s;
    value()      = v;
  }

  inline
  VoxelHSV::~VoxelHSV()
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: Destructor" << std::endl;
    #endif
  }

  //=== AFFECTATION ==========================================================

  inline
  VoxelHSV & VoxelHSV::operator = ( const VoxelHSV & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator = ( HSV )" << std::endl;
    #endif
    hue()        = other.hue();
    saturation() = other.saturation();
    value()      = other.value();
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator = ( const uint8_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator = ( uint8_t )" << std::endl;
    #endif
    hue()        = val;
    saturation() = val;
    value()      = val;
    return *this;
  }

  //=== OPERATORS ============================================================

  inline
  VoxelHSV & VoxelHSV::operator += ( const VoxelHSV & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator += ( HSV )" << std::endl;
    #endif
    hue()        += other.hue();
    saturation() += other.saturation();
    value()      += other.value();
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator -= ( const VoxelHSV & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator -= ( HSV )" << std::endl;
    #endif
    hue()        -= other.hue();
    saturation() -= other.saturation();
    value()      -= other.value();
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator += ( const uint8_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator += ( uint8_t )" << std::endl;
    #endif
    hue()        += val;
    saturation() += val;
    value()      += val;
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator -= ( const uint8_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator -= ( uint8_t )" << std::endl;
    #endif
    hue()        -= val;
    saturation() -= val;
    value()      -= val;
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator *= ( const uint8_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( uint8_t )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator *= ( const uint16_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( uint16_t )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator *= ( const uint32_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( uint32_t )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator *= ( const uint64_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( uint64_t )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator *= ( const float & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( float )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator *= ( const double & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( double )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator /= ( const uint8_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( uint8_t )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator /= ( const uint16_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( uint16_t )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator /= ( const uint32_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( uint32_t )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator /= ( const uint64_t & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( uint64_t )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator /= ( const float & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( float )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }

  inline
  VoxelHSV & VoxelHSV::operator /= ( const double & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( double )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }

  //=== EXTERN OPERATORS =====================================================

  inline
  VoxelHSV operator + (const VoxelHSV &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }

  inline
  VoxelHSV operator + (const VoxelHSV &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + uint8_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
 
  inline
  VoxelHSV operator + (const VoxelHSV &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + uint16_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
 
  inline
  VoxelHSV operator + (const VoxelHSV &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + uint32_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
 
  inline
  VoxelHSV operator + (const VoxelHSV &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + uint64_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
 
  inline
  VoxelHSV operator + (const VoxelHSV &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + float" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += static_cast<uint8_t>(bb);
  }
 
  inline
  VoxelHSV operator + (const VoxelHSV &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV + double" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += static_cast<uint8_t>(bb);
  }
  
  inline
  VoxelHSV operator + (const uint8_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint8_t + HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
  
  inline
  VoxelHSV operator + (const uint16_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint16_t + HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
  
  inline
  VoxelHSV operator + (const uint32_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint32_t + HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
  
  inline
  VoxelHSV operator + (const uint64_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint64_t + HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result += bb;
  }
  
  inline
  VoxelHSV operator + (const float &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: float + HSV" << std::endl;
    #endif
    VoxelHSV result( static_cast<uint8_t>(aa) );
    return result += bb;
  }
  
  inline
  VoxelHSV operator + (const double &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: double + HSV" << std::endl;
    #endif
    VoxelHSV result( static_cast<uint8_t>(aa) );
    return result += bb;
  }

  inline
  VoxelHSV operator - (const VoxelHSV &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }

  inline
  VoxelHSV operator - (const VoxelHSV &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - uint8_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }

  inline
  VoxelHSV operator - (const VoxelHSV &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - uint16_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
 
  inline
  VoxelHSV operator - (const VoxelHSV &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - uint32_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
 
  inline
  VoxelHSV operator - (const VoxelHSV &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - uint64_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
 
  inline
  VoxelHSV operator - (const VoxelHSV &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - float" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= static_cast<uint8_t>(bb);
  }
 
  inline
  VoxelHSV operator - (const VoxelHSV &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV - double" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= static_cast<uint8_t>(bb);
  }
  
  inline
  VoxelHSV operator - (const uint8_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint8_t - HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
  
  inline
  VoxelHSV operator - (const uint16_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint16_t - HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
  
  inline
  VoxelHSV operator - (const uint32_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint32_t - HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
  
  inline
  VoxelHSV operator - (const uint64_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint64_t - HSV" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result -= bb;
  }
  
  inline
  VoxelHSV operator - (const float &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: float - HSV" << std::endl;
    #endif
    VoxelHSV result( static_cast<uint8_t>(aa) );
    return result -= bb;
  }
  
  inline
  VoxelHSV operator - (const double &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: double - HSV" << std::endl;
    #endif
    VoxelHSV result( static_cast<uint8_t>(aa) );
    return result -= bb;
  }

  inline
  VoxelHSV operator * (const VoxelHSV &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * uint8_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }

  inline
  VoxelHSV operator * (const VoxelHSV &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * uint16_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }

  inline
  VoxelHSV operator * (const VoxelHSV &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * uint32_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }

  inline
  VoxelHSV operator * (const VoxelHSV &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * uint64_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }

  inline
  VoxelHSV operator * (const VoxelHSV &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * float" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }

  inline
  VoxelHSV operator * (const VoxelHSV &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * double" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }

  inline
  VoxelHSV operator * (const uint8_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint8_t * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }

  inline
  VoxelHSV operator * (const uint16_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint16_t * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }

  inline
  VoxelHSV operator * (const uint32_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint32_t * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }

  inline
  VoxelHSV operator * (const uint64_t &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: uint64_t * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }

  inline
  VoxelHSV operator * (const float &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: float * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }

  inline
  VoxelHSV operator * (const double &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: double * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }

  inline
  VoxelHSV operator / (const VoxelHSV &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / uint8_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }

  inline
  VoxelHSV operator / (const VoxelHSV &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / uint16_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }

  inline
  VoxelHSV operator / (const VoxelHSV &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / uint32_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }

  inline
  VoxelHSV operator / (const VoxelHSV &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / uint64_t" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }

  inline
  VoxelHSV operator / (const VoxelHSV &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / float" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }

  inline
  VoxelHSV operator / (const VoxelHSV &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / double" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }
  
  //=== LONG INT OPERATORS ===================================================
  
  inline
  VoxelHSV & VoxelHSV::operator *= ( const long & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator *= ( long )" << std::endl;
    #endif
    hue()        *= static_cast<uint8_t>( val );
    saturation() *= static_cast<uint8_t>( val );
    value()      *= static_cast<uint8_t>( val );
    return *this;
  }
  
  inline
  VoxelHSV & VoxelHSV::operator /= ( const long & val )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: operator /= ( long )" << std::endl;
    #endif
    ASSERT( val != 0 );
    hue()        /= static_cast<uint8_t>( val );
    saturation() /= static_cast<uint8_t>( val );
    value()      /= static_cast<uint8_t>( val );
    return *this;
  }
  
  inline
  VoxelHSV operator * (const VoxelHSV &aa, const long &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV * long" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result *= bb;
  }
  
  inline
  VoxelHSV operator * (const long &aa, const VoxelHSV &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: long * HSV" << std::endl;
    #endif
    VoxelHSV result( bb );
    return result *= aa;
  }
  
  inline
  VoxelHSV operator / (const VoxelHSV &aa, const long &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: HSV / long" << std::endl;
    #endif
    VoxelHSV result( aa );
    return result /= bb;
  }

  // KeyComparator VoxelHSV specialization
  inline
  bool KeyComparator<carto::VoxelHSV>::less( const carto::VoxelHSV & lhs, const carto::VoxelHSV & rhs )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "HSV:: KeyComparator less()" << std::endl;
    #endif
    return carto::KeyComparator<carto::VoxelValue<uint8_t, 3> >::less( lhs, rhs );
  }
  
}

#endif
