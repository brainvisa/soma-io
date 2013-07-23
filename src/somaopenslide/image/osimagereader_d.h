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

#ifndef SOMAIO_IMAGE_OSIMAGEREADER_D_H
#define SOMAIO_IMAGE_OSIMAGEREADER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/osimagereader.h>                  // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/config/verbose.h>                       // verbosity level
//--- system -----------------------------------------------------------------
#include <openslide.h>
#include <memory>
#include <vector>
#include <string>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii -> used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "OSIMAGEWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma {

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T> 
  void OSImageReader<T>::updateParams( DataSourceInfo & dsi )
  {

    int rcount = 1;
    try {
      rcount = dsi.header()->getProperty( "resolutions_dimension" )->size();
    } catch( ... ) {
    }
    _sizes = std::vector< std::vector<int> >( rcount, std::vector<int>(4) );
    int i, j;
    for( i=0; i<rcount; ++i )
      for( j=0; j<4; ++j )
        _sizes[i][j] = dsi.header()->getProperty( "resolutions_dimension" )
                                   ->getArrayItem( i )
                                   ->getArrayItem( j )
                                   ->getScalar();

    std::string fname = dsi.list().dataSource( "ima", 0 )->url();
    if( !( _osimage = openslide_open( fname.c_str() ) ) ) {
      localMsg( "can't open file." );
      throw carto::open_error( "data source not available", fname );
    }
  }

  template <typename T> 
  void OSImageReader<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
    openslide_close( _osimage );
  }

  template <typename T> 
  void OSImageReader<T>::swapVoxels( T* buffer, int64_t size,
                                     const bool & byteswap )
  {
    int64_t i;
    uint8_t *ptr8;
    uint32_t *ptr32;

    for( i=0; i<size; ++i )
    {
      // byte swapping if necessary
      ptr8 = (uint8_t*) ( buffer + i );
      ptr32 = (uint32_t*) ( buffer + i );
      if( byteswap )
      {
        for( size_t b=0; b<sizeof(T)/2; ++b )
          std::swap( ptr8[b], ptr8[sizeof(T)-1-b] );
      }
      // transform ARGB -> RGBA
      if( byteswap )
        *ptr32 = ( ( *ptr32 >> 8 ) | 0x11000000 );
      else
        *ptr32 = ( ( *ptr32 << 8 ) | 0x00000011 );
    }
  }

  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================

  template <typename T>
  OSImageReader<T>::OSImageReader() : ImageReader<T>()
  {
  }

  template <typename T>
  OSImageReader<T>::~OSImageReader()
  {
  }

  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void OSImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                               std::vector<int> & pos,
                               std::vector<int> & size,
                               std::vector<int> & /* stride */,
                               carto::Object      options )
  {
    if( _sizes.empty() )
      updateParams( dsi );

    int32_t level = 0;
    try {
      if( options.get() )
        level = options->getProperty( "resolution_level" )->getScalar();
    } catch( ... ) {
    }

    int64_t posx = (int64_t) pos[0] * _sizes[0][0] / _sizes[ level ][0];
    int64_t posy = (int64_t) pos[1] * _sizes[0][1] / _sizes[ level ][1];
    int64_t sizex = (int64_t) size[0];
    int64_t sizey = (int64_t) size[1];

    openslide_read_region( _osimage, (uint32_t *) dest, posx, posy,
                           level, sizex, sizey );

    uint32_t bo = SOMAIO_BYTE_ORDER;
    std::string byte_order( (char*) &bo );
//     localMsg( "swapVoxels( " + carto::toString( byte_order != "ABCD" ) + " )" );
    swapVoxels( dest, sizex*sizey, byte_order != "ABCD" );
  }
}

#undef localMsg
#endif
