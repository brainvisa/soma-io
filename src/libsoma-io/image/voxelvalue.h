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

#ifndef SOMAIO_IMAGE_VOXELVALUE_H
#define SOMAIO_IMAGE_VOXELVALUE_H

//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/exception/assert.h>
//--- system -------------------------------------------------------------------
#define SOMA_IO_DEBUG_VOXELVALUE
#ifdef SOMA_IO_DEBUG_VOXELVALUE
  #include <iostream>
#endif
//------------------------------------------------------------------------------

namespace soma {
  
  template <typename T, unsigned int C>
  class VoxelValue
  {
    public:
      //=== CONSTRUCTORS =======================================================
      VoxelValue();
      VoxelValue( const VoxelValue<T,C> & other );
      ~VoxelValue();
      
      //=== OPERATORS ==========================================================
      VoxelValue<T,C> & operator  = ( const VoxelValue<T,C> & other );
      VoxelValue<T,C> & operator += ( const VoxelValue<T,C> & other );
      VoxelValue<T,C> & operator -= ( const VoxelValue<T,C> & other );
      VoxelValue<T,C> & operator  = ( const T & value );
      VoxelValue<T,C> & operator += ( const T & value );
      VoxelValue<T,C> & operator -= ( const T & value );
      VoxelValue<T,C> & operator *= ( const uint8_t & value );
      VoxelValue<T,C> & operator *= ( const uint16_t & value );
      VoxelValue<T,C> & operator *= ( const uint32_t & value );
      VoxelValue<T,C> & operator *= ( const uint64_t & value );
      VoxelValue<T,C> & operator *= ( const float & value );
      VoxelValue<T,C> & operator *= ( const double & value );
      VoxelValue<T,C> & operator /= ( const uint8_t & value );
      VoxelValue<T,C> & operator /= ( const uint16_t & value );
      VoxelValue<T,C> & operator /= ( const uint32_t & value );
      VoxelValue<T,C> & operator /= ( const uint64_t & value );
      VoxelValue<T,C> & operator /= ( const float & value );
      VoxelValue<T,C> & operator /= ( const double & value );
      
      //=== ACCESSORS ==========================================================
      const T & operator[] ( unsigned int i ) const  { return _voxel[i]; }
            T & operator[] ( unsigned int i )        { return _voxel[i]; }
      
    protected:
      T _voxel[C];
  };
  
  //============================================================================
  //   E X T E R N   O P E R A T O R S
  //============================================================================
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator + (const VoxelValue<T,C> &aa, const VoxelValue<T,C> &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator+( VV,VV )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result += bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator - (const VoxelValue<T,C> &aa, const VoxelValue<T,C> &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator-( VV,VV )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result -= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator + (const VoxelValue<T,C> &aa, const T &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator+( VV,const )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result += bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator - (const VoxelValue<T,C> &aa, const T &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator-( VV,const )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result -= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const VoxelValue<T,C> &aa, const uint8_t  &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( VV,uint8_t )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result *= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const VoxelValue<T,C> &aa, const uint16_t &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( VV,uint16_t )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result *= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const VoxelValue<T,C> &aa, const uint32_t &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( VV,uint32_t )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result *= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const VoxelValue<T,C> &aa, const uint64_t &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( VV,uint64_t )" << std::endl;
    #endif
    VoxelValue<T,C> result( aa );
    result *= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const uint8_t  &aa, const VoxelValue<T,C>  &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( uint8_t,VV )" << std::endl;
    #endif
    VoxelValue<T,C> result( bb );
    result *= aa;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const uint16_t &aa, const VoxelValue<T,C>  &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( uint16_t,VV )" << std::endl;
    #endif
    VoxelValue<T,C> result( bb );
    result *= aa;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const uint32_t &aa, const VoxelValue<T,C>  &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( uint32_t,VV )" << std::endl;
    #endif
    VoxelValue<T,C> result( bb );
    result *= aa;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator * (const uint64_t &aa, const VoxelValue<T,C>  &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*( uint64_t,VV )" << std::endl;
    #endif
    VoxelValue<T,C> result( bb );
    result *= aa;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator / (const VoxelValue<T,C> &aa, const uint8_t  &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/( VV,uint8_t )" << std::endl;
    #endif
    ASSERT( bb != 0 );
    VoxelValue<T,C> result( aa );
    result /= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator / (const VoxelValue<T,C> &aa, const uint16_t &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/( VV,uint16_t )" << std::endl;
    #endif
    ASSERT( bb != 0 );
    VoxelValue<T,C> result( aa );
    result /= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator / (const VoxelValue<T,C> &aa, const uint32_t &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/( VV,uint32_t )" << std::endl;
    #endif
    ASSERT( bb != 0 );
    VoxelValue<T,C> result( aa );
    result /= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C> operator / (const VoxelValue<T,C> &aa, const uint64_t &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/( VV,uint64_t )" << std::endl;
    #endif
    ASSERT( bb != 0 );
    VoxelValue<T,C> result( aa );
    result /= bb;
    return result;
  }
  
  template <typename T, unsigned int C>
  bool  operator == (const VoxelValue<T,C> &aa, const VoxelValue<T,C> &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator==( VV,VV )" << std::endl;
    #endif
    int i;
    for( i=0; i<C; ++i )
      if( aa[ i ] != bb[ i ] )
        return false;
    return true;
  }
  
  template <typename T, unsigned int C>
  bool  operator != (const VoxelValue<T,C> &aa, const VoxelValue<T,C> &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator!=( VV,VV )" << std::endl;
    #endif
    return !( aa == bb );
  }
  
  template <typename T, unsigned int C>
  bool  operator == (const VoxelValue<T,C> &aa, const T &bb)
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator==( VV,const )" << std::endl;
    #endif
    int i;
    for( i=0; i<C; ++i )
      if( aa[ i ] != bb )
        return false;
    return true;
  }
  
}

#endif