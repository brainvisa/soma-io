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

#ifndef SOMAIO_IMAGE_VOXELVALUE_D_H
#define SOMAIO_IMAGE_VOXELVALUE_D_H

//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/voxelvalue.h>
//--- cartobase ----------------------------------------------------------------
//--- system -------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace soma {
  
  //============================================================================
  //   C O N S T R U C T O R S
  //============================================================================
  
  template <typename T, unsigned int C>
  VoxelValue<T,C>::VoxelValue()
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: constructor()" << std::endl;
    #endif
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C>::VoxelValue( const VoxelValue<T,C> & other )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: constructor( other )" << std::endl;
    #endif
    *this = other;
  }
  
  template <typename T, unsigned int C>
  VoxelValue<T,C>::~VoxelValue()
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: destructor()" << std::endl;
    #endif
  }
  
  //============================================================================
  //   I N T E R N   O P E R A T O R S
  //============================================================================
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator  = ( const VoxelValue<T,C> & other )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator=( other )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] = other._voxel[i];
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator += ( const VoxelValue<T,C> & other )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator+=( other )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] += other._voxel[i];
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator -= ( const VoxelValue<T,C> & other )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator-=( other)" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] -= other._voxel[i];
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator  = ( const T & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator=( " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] = value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator += ( const T & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator+=( " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] += value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator -= ( const T & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator-=( " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] -= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator *= ( const uint8_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*=( (uint8_t) " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] *= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator *= ( const uint16_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*=( (uint16_t) " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] *= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator *= ( const uint32_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*=( (uint32_t) " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] *= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator *= ( const uint64_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*=( (uint64_t) " << (int) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] *= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator *= ( const float & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*=( (float) " << (float) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] *= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator *= ( const double & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator*=( (double) " << (float) value << " )" << std::endl;
    #endif
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] *= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator /= ( const uint8_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/=( (uint8_t) " << (int) value << " )" << std::endl;
    #endif
    ASSERT( value != 0 );
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] /= value;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator /= ( const uint16_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/=( (uint16_t) " << (int) value << " )" << std::endl;
    #endif
    ASSERT( value != 0 );
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] /= value;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator /= ( const uint32_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/=( (uint32_t) " << (int) value << " )" << std::endl;
    #endif
    ASSERT( value != 0 );
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] /= value;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator /= ( const uint64_t & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/=( (uint64_t) " << (int) value << " )" << std::endl;
    #endif
    ASSERT( value != 0 );
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] /= value;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator /= ( const float & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator/=( (float) " << (float) value << " )" << std::endl;
    #endif
    ASSERT( value != 0 );
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] /= value;
    return *this;
  }
  
  template <typename T, unsigned int C> VoxelValue<T,C> & 
  VoxelValue<T,C>::operator /= ( const double & value )
  {
    #ifdef SOMA_IO_DEBUG_VOXELVALUE
      std::cout << "VOXELVALUE:: operator=( (double) " << (float) value << " )" << std::endl;
    #endif
    ASSERT( value != 0 );
    unsigned int i;
    for( i=0; i<C; ++i )
      _voxel[ i ] /= value;
    return *this;
  }
  
}

#endif