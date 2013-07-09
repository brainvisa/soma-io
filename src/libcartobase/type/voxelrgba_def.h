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

#ifndef CARTOBASE_TYPE_RGBA_DEF_H
#define CARTOBASE_TYPE_RGBA_DEF_H
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
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
  VoxelRGBA::VoxelRGBA( const VoxelRGBA &other )
  : VoxelValue<uint8_t,4>( other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: Constructor( RGBA )" << std::endl;
    #endif
  }

  inline
  VoxelRGBA::VoxelRGBA( const VoxelRGB &other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: Constructor( RGB )" << std::endl;
    #endif
    red()   = other.red();
    green() = other.green();
    blue()  = other.blue();
    alpha() = 255;
  }

  inline
  VoxelRGBA::VoxelRGBA( const VoxelValue<uint8_t,4> &other )
  : VoxelValue<uint8_t,4>( other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: Constructor( VV<4> )" << std::endl;
    #endif
  }

  inline
  VoxelRGBA::VoxelRGBA( const uint8_t &r, const uint8_t &g, 
                        const uint8_t &b, const uint8_t &a )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: Constructor( r,g,b,a )" << std::endl;
    #endif
    red()   = r;
    green() = g;
    blue()  = b;
    alpha() = a;
  }

  inline
  VoxelRGBA::~VoxelRGBA()
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: Destructor" << std::endl;
    #endif
  }

  //=== AFFECTATION ==========================================================

  inline
  VoxelRGBA & VoxelRGBA::operator = ( const VoxelRGBA & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator = ( RGBA )" << std::endl;
    #endif
    red()   = other.red();
    green() = other.green();
    blue()  = other.blue();
    alpha() = other.alpha();
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator = ( const VoxelRGB & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator = ( RGB )" << std::endl;
    #endif
    red()   = other.red();
    green() = other.green();
    blue()  = other.blue();
    alpha() = 255;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator = ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator = ( uint8_t )" << std::endl;
    #endif
    red()   = value;
    green() = value;
    blue()  = value;
    alpha() = 255;
    return *this;
  }

  //=== OPERATORS ============================================================

  inline
  VoxelRGBA & VoxelRGBA::operator += ( const VoxelRGBA & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator += ( RGBA )" << std::endl;
    #endif
    if( other.alpha() == 0 ) {
      if( alpha() == 0 ) {
        red()   = 0;
        green() = 0;
        blue()  = 0;
      } else {
        // other is "0"
        // do nothing
      }
    } else if( alpha() == 0 ) {
      red()   = other.red();
      green() = other.green();
      blue()  = other.blue();
      alpha() = other.alpha();
    } else {
      red()   += other.red();
      green() += other.green();
      blue()  += other.blue();
      // keep greater alpha
      if( alpha() < other.alpha() )
        alpha() = other.alpha();
    }
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator += ( const VoxelRGB & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator += ( RGB )" << std::endl;
    #endif
    if( alpha() == 0 ) {
      red()   = other.red();
      green() = other.green();
      blue()  = other.blue();
    } else {
      red()   += other.red();
      green() += other.green();
      blue()  += other.blue();
    }
    alpha() = 255;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator -= ( const VoxelRGBA & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator -= ( RGBA )" << std::endl;
    #endif
    if( other.alpha() == 0 ) {
      if( alpha() == 0 ) {
        red()   = 0;
        green() = 0;
        blue()  = 0;
      } else {
        // other is "0"
        // do nothing
      }
    } else if( alpha() == 0 ) {
      // this is "0"
      // do nothing
    } else {
      red()   -= other.red();
      green() -= other.green();
      blue()  -= other.blue();
      // keep greater alpha
      if( alpha() < other.alpha() )
        alpha() = other.alpha();
    }
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator -= ( const VoxelRGB & other )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator -= ( RGB )" << std::endl;
    #endif
    if( alpha() == 0 ) {
      // this is "0"
      // do nothing
    } else {
      red()   -= other.red();
      green() -= other.green();
      blue()  -= other.blue();
    }
    alpha() = 255;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator += ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator += ( uint8_t )" << std::endl;
    #endif
    if( alpha() == 0 ) {
      red()   = value;
      green() = value;
      blue()  = value;
    } else {
      red()   += value;
      green() += value;
      blue()  += value;
    }
    alpha() = 255;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator -= ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator -= ( uint8_t )" << std::endl;
    #endif
    if( alpha() == 0 ) {
      red()   = value;
      green() = value;
      blue()  = value;
    } else {
      red()   -= value;
      green() -= value;
      blue()  -= value;
    }
    alpha() = 255;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( uint8_t )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const uint16_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( uint16_t )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const uint32_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( uint32_t )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const uint64_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( uint64_t )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const float & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( float )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const double & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( double )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const uint8_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( uint8_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const uint16_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( uint16_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const uint32_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( uint32_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const uint64_t & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( uint64_t )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const float & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( float )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }

  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const double & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( double )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }

  //=== EXTERN OPERATORS =====================================================

  inline
  VoxelRGBA operator + (const VoxelRGBA &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA + RGBA" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result += bb;
  }

  inline
  VoxelRGBA operator + (const VoxelRGBA &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA + uint8_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result += bb;
  }

  inline
  VoxelRGBA operator - (const VoxelRGBA &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA - RGBA" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result -= bb;
  }

  inline
  VoxelRGBA operator - (const VoxelRGBA &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA - uint8_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result -= bb;
  }

  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * uint8_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }

  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * uint16_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }

  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * uint32_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }

  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * uint64_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }

  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * float" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }

  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * double" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }

  inline
  VoxelRGBA operator * (const uint8_t &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: uint8_t * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }

  inline
  VoxelRGBA operator * (const uint16_t &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: uint16_t * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }

  inline
  VoxelRGBA operator * (const uint32_t &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: uint32_t * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }

  inline
  VoxelRGBA operator * (const uint64_t &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: uint64_t * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }

  inline
  VoxelRGBA operator * (const float &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: float * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }

  inline
  VoxelRGBA operator * (const double &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: double * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }

  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint8_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / uint8_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }

  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint16_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / uint16_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }

  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint32_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / uint32_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }

  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const uint64_t &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / uint64_t" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }

  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const float &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / float" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }

  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const double &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / double" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }
  
  //=== LONG INT OPERATORS ===================================================
  
  inline
  VoxelRGBA & VoxelRGBA::operator *= ( const long & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator *= ( long )" << std::endl;
    #endif
    red()   *= value;
    green() *= value;
    blue()  *= value;
    return *this;
  }
  
  inline
  VoxelRGBA & VoxelRGBA::operator /= ( const long & value )
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: operator /= ( long )" << std::endl;
    #endif
    ASSERT( value != 0 );
    red()   /= value;
    green() /= value;
    blue()  /= value;
    return *this;
  }
  
  inline
  VoxelRGBA operator * (const VoxelRGBA &aa, const long &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA * long" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result *= bb;
  }
  
  inline
  VoxelRGBA operator * (const long &aa, const VoxelRGBA &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: long * RGBA" << std::endl;
    #endif
    VoxelRGBA result( bb );
    return result *= aa;
  }
  
  inline
  VoxelRGBA operator / (const VoxelRGBA &aa, const long &bb)
  {
    #ifdef CARTO_DEBUG_RGB
      std::cout << "RGBA:: RGBA / long" << std::endl;
    #endif
    VoxelRGBA result( aa );
    return result /= bb;
  }

}

#endif