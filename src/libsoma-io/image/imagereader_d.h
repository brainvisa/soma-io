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

#ifndef SOMAIO_IMAGE_IMAGEREADER_D_H
#define SOMAIO_IMAGE_IMAGEREADER_D_H
//--- soma io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                      // class declaration
#include <soma-io/datasourceinfo/datasourceinfo.h>                     // member
//--- cartobase ----------------------------------------------------------------
#include <cartobase/object/object.h>         // updateParams(): to access header
#include <cartobase/object/property.h>         // updateParams(): to read header
//--- system -------------------------------------------------------------------
#include <vector>
//------------------------------------------------------------------------------

namespace soma {
  
  //============================================================================
  //   U T I L I T I E S
  //============================================================================
  template <typename T> 
  void ImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    try {
      _binary = !(bool) 
        dsi.header()->getProperty( "ascii" )->getScalar();
    } catch( ... ) {
      _binary = true;
    }
    
    try {
      _byteswap = (bool) 
        dsi.header()->getProperty( "byte_swapping" )->getScalar();
    } catch( ... ) {
      _byteswap = false;
    }
    
    int rescount = 1;
    if( dsi.header()->hasProperty( "resolutions_dimension" ) ) {
      rescount = dsi.header()->getProperty( "resolutions_dimension" )->size();
      _sizes = std::vector< std::vector<int> >( rescount, std::vector<int>(4) );
      int i;
      for( int i=0; i<rescount; ++i ) {
        _sizes[i][0] = dsi.header()->getProperty( "resolutions_dimension" )
                                   ->getArrayItem(i)->getArrayItem(0)
                                   ->getScalar();
        _sizes[i][1] = dsi.header()->getProperty( "resolutions_dimension" )
                                   ->getArrayItem(i)->getArrayItem(1)
                                   ->getScalar();
        _sizes[i][2] = dsi.header()->getProperty( "resolutions_dimension" )
                                   ->getArrayItem(i)->getArrayItem(2)
                                   ->getScalar();
        _sizes[i][3] = dsi.header()->getProperty( "resolutions_dimension" )
                                   ->getArrayItem(i)->getArrayItem(3)
                                   ->getScalar();
      }
    } else {
      _sizes = std::vector< std::vector<int> >( rescount, std::vector<int>(4) );
      dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
      dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
      dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
      dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    }
  }
  
  template <typename T>
  void ImageReader<T>::resetParams()
  {
    _binary = false;
    _byteswap = false;
    _sizes = std::vector< std::vector<int> >();
  }
  
  //============================================================================
  //   C O N S T R U C T O R S
  //============================================================================
  template <typename T>
  ImageReader<T>::ImageReader() :
    _binary( false ), _byteswap( false ), _sizes()
  {
  }
  
  template <typename T>
  ImageReader<T>::~ImageReader()
  {
  }
  
  //============================================================================
  //   M E T H O D S
  //============================================================================
  //--- read(...) --------------------------------------------------------------
  /* This is an abstract method that is defined in format-specific readers
   * (like GisImageReader)
   */
  template <typename T>
  void ImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                             std::vector<int> &  /* pos */,
                             std::vector<int> &  /*size */,
                             std::vector<int>    /* stride */,
                             int                 /* level */, 
                             carto::Object       /* options */ )
  {
    carto::rc_ptr<DataSource> ds = dsi.list().dataSource( "default", 0 );
    throw carto::invalid_format_error( "format reader not implemented yet...", 
                                       ds ? ds->url() : "" );
  }
}

#endif