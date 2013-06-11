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
#include <soma-io/datasource/datasource.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/exception/assert.h>
//--- system -------------------------------------------------------------------
//#define SOMA_IO_DEBUG_VOXELVALUE
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
      bool operator == ( const VoxelValue<T,C> & );
      bool operator != ( const VoxelValue<T,C> & );
      bool operator == ( const T & );
      bool operator != ( const T & );
      
      
      //=== ACCESSORS ==========================================================
      inline const T & operator[] ( unsigned int i ) const  { return _voxel[i]; }
      inline       T & operator[] ( unsigned int i )        { return _voxel[i]; }
      
    protected:
      T _voxel[C];
  };

  template <typename T, unsigned int C> inline
  std::ostream& operator << ( std::ostream &out, const VoxelValue<T,C> &aa )
  {
    int i;
    out << '(';
    for( i=0; i<C-1; ++i )
      out << (int) aa[i] << ',';
    out << (int) aa[C-1] << ')';
    return( out );
  }

  template <typename T, unsigned int C> inline
  std::istream& operator >> ( std::istream &in , VoxelValue<T,C> &aa )
  {
    int read;
    char ch = 0;
    int i;
    VoxelValue<T,C> result;
    
    while( in && ( in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\n' ) )
      in >> ch;
    if ( in.peek () == '(' )
      in >> ch;
    
    in >> read >> ch;
    result[0] = (T) read;
    for( i=1; i<C; ++i )
      if( ch == ',' ) {
        in >> read >> ch;
        result[i] = (T) read;
      }
    
    if( ch != 0 && ch != ')' )
      in.setstate (std::ios::failbit);
    else if (in.good())
      aa = result;
    
    return in;
  }
  
  template <typename T, unsigned int C>
  class AsciiDataSourceTraits< VoxelValue<T,C> >
  {
  public:
    static bool read( DataSource & ds, VoxelValue<T,C>  & item );
    static bool write( DataSource & ds, const VoxelValue<T,C>  & item );
  };
  
  template <typename T, unsigned int C> inline
  bool AsciiDataSourceTraits<VoxelValue<T,C> >::read( DataSource & ds, 
                                                      VoxelValue<T,C> & item )
  {
    int c, i;
    VoxelValue<T,C> result;

    while( true ) {
      c = ds.getch();
      if( c != ' ' && c != '\t' && c != '\n' && c != '(' )
        break;
      if( c == '(' ) {
        AsciiDataSourceTraits<T>::read( ds, result[0] );
        c = ds.getch();
        for( i=1; i<C; ++i )
          if( c == ',' ) {
            AsciiDataSourceTraits<T>::read( ds, result[i] );
            c = ds.getch();
          }
        if( c == 0 || c == ')' ) {
          item = result;
          return true;
        }
        return false;
      }
    }
    return false;
  }

  template <typename T, unsigned int C> inline
  bool AsciiDataSourceTraits<VoxelValue<T,C> >::write( DataSource & ds, 
                                                       const VoxelValue<T,C> & item )
  {
    int i;
    ds << '(';
    for( i=0; i<C-1; ++i )
      ds << (int) item[i] << ',';
    ds << (int) item[C-1] << ')';
    return true;
  }
  
  template<typename T, unsigned int C> inline DataSource & 
  operator << ( DataSource & ds, const VoxelValue<T,C> & x )
  {
    AsciiDataSourceTraits<VoxelValue<T,C> >::write( ds, x );
    return ds;
  }

}

#endif