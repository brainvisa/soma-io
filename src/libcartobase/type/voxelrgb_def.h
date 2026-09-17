#ifndef CARTOBASE_TYPE_RGB_DEF_H
#define CARTOBASE_TYPE_RGB_DEF_H
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/type/voxelvalue_d.h>
#include <cartobase/exception/assert.h>
//--- system -----------------------------------------------------------------
//#define CARTO_DEBUG_RGB
#ifdef CARTO_DEBUG_RGB
  #include <iostream>
#endif
//----------------------------------------------------------------------------

/*****************************************************************************
 * Since all methods are inline and for readability issues, declarations
 * are contained in "_decl.h" file and definitions in "_def.h" file.
 * You should include ".h" file which binds all necessary headers.
 *
 * Read/Write methods are defined in soma-io/utilities/asciidatasourcetraits.h
 * so you must include this file to read or write voxels as ASCII.
 ****************************************************************************/

namespace carto {

  //=== CONSTRUCTORS =========================================================

  inline
  VoxelRGB::VoxelRGB( const VoxelRGB &other )
  : VoxelValue<uint8_t,3>( other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: Constructor( RGB )" << std::endl;
    #endif
  }

  inline
  VoxelRGB::VoxelRGB( const VoxelRGBA &other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: Constructor( RGBA )" << std::endl;
    #endif
    red()   = other.red();
    green() = other.green();
    blue()  = other.blue();
  }

  inline
  VoxelRGB::VoxelRGB( const VoxelValue<uint8_t,3> &other )
  : VoxelValue<uint8_t,3>( other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: Constructor( VV<3> )" << std::endl;
    #endif
  }

  inline
  VoxelRGB::VoxelRGB( const uint8_t &r, const uint8_t &g, const uint8_t &b )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: Constructor( r,g,b )" << std::endl;
    #endif
    red()   = r;
    green() = g;
    blue()  = b;
  }

  inline
  VoxelRGB::~VoxelRGB()
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: Destructor" << std::endl;
    #endif
  }

  //=== AFFECTATION ==========================================================

  inline
  VoxelRGB & VoxelRGB::operator = ( const VoxelRGB & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator = ( RGB )" << std::endl;
    #endif
    red()   = other.red();
    green() = other.green();
    blue()  = other.blue();
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator = ( const VoxelRGBA & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator = ( RGBA )" << std::endl;
    #endif
    red()   = other.red();
    green() = other.green();
    blue()  = other.blue();
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator = ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator = ( uint8_t )" << std::endl;
    #endif
    red()   = value;
    green() = value;
    blue()  = value;
    return *this;
  }

  //=== OPERATORS ============================================================

  inline
  VoxelRGB & VoxelRGB::operator += ( const VoxelRGB & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator += ( RGB )" << std::endl;
    #endif
    red()   += other.red();
    green() += other.green();
    blue()  += other.blue();
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator += ( const VoxelRGBA & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator += ( RGBA )" << std::endl;
    #endif
    red()   += other.red();
    green() += other.green();
    blue()  += other.blue();
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator -= ( const VoxelRGB & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator -= ( RGB )" << std::endl;
    #endif
    red()   -= other.red();
    green() -= other.green();
    blue()  -= other.blue();
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator -= ( const VoxelRGBA & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator -= ( RGBA )" << std::endl;
    #endif
    red()   -= other.red();
    green() -= other.green();
    blue()  -= other.blue();
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator += ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator += ( uint8_t )" << std::endl;
    #endif
    red()   += value;
    green() += value;
    blue()  += value;
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator -= ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator -= ( uint8_t )" << std::endl;
    #endif
    red()   -= value;
    green() -= value;
    blue()  -= value;
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator *= ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( uint8_t )" << std::endl;
    #endif
    red()   *= static_cast<uint8_t>( value );
    green() *= static_cast<uint8_t>( value );
    blue()  *= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator *= ( const uint16_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( uint16_t )" << std::endl;
    #endif
    red()   *= static_cast<uint8_t>( value );
    green() *= static_cast<uint8_t>( value );
    blue()  *= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator *= ( const uint32_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( uint32_t )" << std::endl;
    #endif
    red()   *= static_cast<uint8_t>( value );
    green() *= static_cast<uint8_t>( value );
    blue()  *= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator *= ( const uint64_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( uint64_t )" << std::endl;
    #endif
    red()   *= static_cast<uint8_t>( value );
    green() *= static_cast<uint8_t>( value );
    blue()  *= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator *= ( const float & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( float )" << std::endl;
    #endif
    // not using *= operator to perform float operations
    red()   = static_cast<uint8_t>( red() * value );
    green() = static_cast<uint8_t>( green() * value );
    blue()  = static_cast<uint8_t>( blue() * value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator *= ( const double & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( double )" << std::endl;
    #endif
    // not using *= operator to perform float operations
    red()   = static_cast<uint8_t>( red() * value );
    green() = static_cast<uint8_t>( green() * value );
    blue()  = static_cast<uint8_t>( blue() * value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator /= ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( uint8_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= static_cast<uint8_t>( value );
    green() /= static_cast<uint8_t>( value );
    blue()  /= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator /= ( const uint16_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( uint16_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= static_cast<uint8_t>( value );
    green() /= static_cast<uint8_t>( value );
    blue()  /= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator /= ( const uint32_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( uint32_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= static_cast<uint8_t>( value );
    green() /= static_cast<uint8_t>( value );
    blue()  /= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator /= ( const uint64_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( uint64_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= static_cast<uint8_t>( value );
    green() /= static_cast<uint8_t>( value );
    blue()  /= static_cast<uint8_t>( value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator /= ( const float & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( float )" << std::endl;
    #endif
    ASSERT( value != 0 );
    // not using /= operator to perform float operations
    red()   = static_cast<uint8_t>( red() / value );
    green() = static_cast<uint8_t>( green() / value );
    blue()  = static_cast<uint8_t>( blue() / value );
    return *this;
  }

  inline
  VoxelRGB & VoxelRGB::operator /= ( const double & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( double )" << std::endl;
    #endif
    ASSERT( value != 0 );
    // not using /= operator to perform float operations
    red()   = static_cast<uint8_t>( red() / value );
    green() = static_cast<uint8_t>( green() / value );
    blue()  = static_cast<uint8_t>( blue() / value );
    return *this;
  }

  //=== EXTERN OPERATORS =====================================================

  inline
  VoxelRGB operator + (const VoxelRGB &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }

  inline
  VoxelRGB operator + (const VoxelRGB &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + uint8_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
 
  inline
  VoxelRGB operator + (const VoxelRGB &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + uint16_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
 
  inline
  VoxelRGB operator + (const VoxelRGB &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + uint32_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
 
  inline
  VoxelRGB operator + (const VoxelRGB &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + uint64_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
 
  inline
  VoxelRGB operator + (const VoxelRGB &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + float" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += static_cast<uint8_t>(bb);
  }
 
  inline
  VoxelRGB operator + (const VoxelRGB &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB + double" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += static_cast<uint8_t>(bb);
  }
  
  inline
  VoxelRGB operator + (const uint8_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint8_t + RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
  
  inline
  VoxelRGB operator + (const uint16_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint16_t + RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
  
  inline
  VoxelRGB operator + (const uint32_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint32_t + RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
  
  inline
  VoxelRGB operator + (const uint64_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint64_t + RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result += bb;
  }
  
  inline
  VoxelRGB operator + (const float &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: float + RGB" << std::endl;
    #endif
    VoxelRGB result( static_cast<uint8_t>(aa) );
    return result += bb;
  }
  
  inline
  VoxelRGB operator + (const double &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: double + RGB" << std::endl;
    #endif
    VoxelRGB result( static_cast<uint8_t>(aa) );
    return result += bb;
  }

  inline
  VoxelRGB operator - (const VoxelRGB &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }

  inline
  VoxelRGB operator - (const VoxelRGB &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - uint8_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
 
  inline
  VoxelRGB operator - (const VoxelRGB &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - uint16_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
 
  inline
  VoxelRGB operator - (const VoxelRGB &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - uint32_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
 
  inline
  VoxelRGB operator - (const VoxelRGB &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - uint64_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
 
  inline
  VoxelRGB operator - (const VoxelRGB &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - float" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= static_cast<uint8_t>(bb);
  }
 
  inline
  VoxelRGB operator - (const VoxelRGB &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB - double" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= static_cast<uint8_t>(bb);
  }
  
  inline
  VoxelRGB operator - (const uint8_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint8_t - RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
  
  inline
  VoxelRGB operator - (const uint16_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint16_t - RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
  
  inline
  VoxelRGB operator - (const uint32_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint32_t - RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
  
  inline
  VoxelRGB operator - (const uint64_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint64_t - RGB" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result -= bb;
  }
  
  inline
  VoxelRGB operator - (const float &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: float - RGB" << std::endl;
    #endif
    VoxelRGB result( static_cast<uint8_t>(aa) );
    return result -= bb;
  }
  
  inline
  VoxelRGB operator - (const double &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: double - RGB" << std::endl;
    #endif
    VoxelRGB result( static_cast<uint8_t>(aa) );
    return result -= bb;
  }

  inline
  VoxelRGB operator * (const VoxelRGB &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * uint8_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }

  inline
  VoxelRGB operator * (const VoxelRGB &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * uint16_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }

  inline
  VoxelRGB operator * (const VoxelRGB &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * uint32_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }

  inline
  VoxelRGB operator * (const VoxelRGB &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * uint64_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }

  inline
  VoxelRGB operator * (const VoxelRGB &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * float" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }

  inline
  VoxelRGB operator * (const VoxelRGB &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * double" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }

  inline
  VoxelRGB operator * (const uint8_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint8_t * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }

  inline
  VoxelRGB operator * (const uint16_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint16_t * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }

  inline
  VoxelRGB operator * (const uint32_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint32_t * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }

  inline
  VoxelRGB operator * (const uint64_t &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: uint64_t * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }

  inline
  VoxelRGB operator * (const float &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: float * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }

  inline
  VoxelRGB operator * (const double &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: double * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }

  inline
  VoxelRGB operator / (const VoxelRGB &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / uint8_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }

  inline
  VoxelRGB operator / (const VoxelRGB &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / uint16_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }

  inline
  VoxelRGB operator / (const VoxelRGB &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / uint32_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }

  inline
  VoxelRGB operator / (const VoxelRGB &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / uint64_t" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }

  inline
  VoxelRGB operator / (const VoxelRGB &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / float" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }

  inline
  VoxelRGB operator / (const VoxelRGB &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / double" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }
  
  //=== LONG INT OPERATORS ===================================================
  
  inline
  VoxelRGB & VoxelRGB::operator *= ( const long & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator *= ( long )" << std::endl;
    #endif
    red()   *= static_cast<uint8_t>( value );
    green() *= static_cast<uint8_t>( value );
    blue()  *= static_cast<uint8_t>( value );
    return *this;
  }
  
  inline
  VoxelRGB & VoxelRGB::operator /= ( const long & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: operator /= ( long )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= static_cast<uint8_t>( value );
    green() /= static_cast<uint8_t>( value );
    blue()  /= static_cast<uint8_t>( value );
    return *this;
  }
  
  inline
  VoxelRGB operator * (const VoxelRGB &aa, const long &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB * long" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result *= bb;
  }
  
  inline
  VoxelRGB operator * (const long &aa, const VoxelRGB &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: long * RGB" << std::endl;
    #endif
    VoxelRGB result( bb );
    return result *= aa;
  }
  
  inline
  VoxelRGB operator / (const VoxelRGB &aa, const long &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: RGB / long" << std::endl;
    #endif
    VoxelRGB result( aa );
    return result /= bb;
  }
  
  // KeyComparator VoxelRGB specialization
  inline
  bool KeyComparator<carto::VoxelRGB>::less( const carto::VoxelRGB & lhs, const carto::VoxelRGB & rhs )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGB:: KeyComparator less()" << std::endl;
    #endif
    return carto::KeyComparator<carto::VoxelValue<uint8_t, 3> >::less( lhs, rhs );
  }

}

#endif
