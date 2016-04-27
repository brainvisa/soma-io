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
#include <soma-io/config/os_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/config/verbose.h>                       // verbosity level
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
#include <string>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii -> used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "OSIMAGEREADER" )
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
      localMsg( "OpenSlide can't open file" + fname );
      throw carto::open_error( "data source not available", fname );
    }
    if( openslide_get_error( _osimage ) ) {
      localMsg( "OpenSlide can't open file : " + fname );
      localMsg( "OpenSlide: " + std::string( openslide_get_error( _osimage ) ) );
      openslide_close( _osimage );
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
        *ptr32 = ( ( *ptr32 >> 8 ) | ( *ptr32 << 24 ) );
      else
        *ptr32 = ( ( *ptr32 << 8 ) | ( *ptr32 >> 24 ) );
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
                               std::vector<int>  & pos,
                               std::vector<int>  & size,
                               std::vector<long> & stride,
                               carto::Object       options )
  {
    if( _sizes.empty() )
      updateParams( dsi );

    int32_t level = 0;
    try {
      if( options.get() ) {
        level = options->getProperty( "resolution_level" )->getScalar();
        if (level < 0) {
          try {
            // Try to solve negative level values
            level += _sizes.size();
          }
          catch(...){}
        }
      }
    } catch( ... ) {
    }
    
    // strides in dest data
    if( stride.size() < 4 )
      stride.resize( 4, 0 );
    if( stride[0] == 0 )
      stride[0] = 1;
    if( stride[1] == 0 )
      stride[1] = ((long)size[0]) * stride[0];
    if( stride[2] == 0 )
      stride[2] = ((long)size[1]) * stride[1];
    if( stride[3] == 0 )
      stride[3] = ((long)size[2]) * stride[2];

//     localMsg("OpenSlide: strides [" + carto::toString(stride[0]) + ", "
//                                     + carto::toString(stride[1]) + ", "
//                                     + carto::toString(stride[2]) + ", "
//                                     + carto::toString(stride[3]) + "]");
    
    // Get tile sizes for the level
    std::vector<std::vector<int64_t> > tsizes;
    try {
      dsi.privateIOData()->getProperty("tile_size", tsizes);
    } catch( ... ) {
    }

    int64_t posx = (int64_t) pos[0] * _sizes[0][0] / _sizes[ level ][0];
    int64_t posy = (int64_t) pos[1] * _sizes[0][1] / _sizes[ level ][1];
    int64_t sizex = (int64_t) size[0];
    int64_t sizey = (int64_t) size[1];
    
    uint32_t bo = SOMAIO_BYTE_ORDER;
    std::string byte_order( (char*) &bo );
//     localMsg( "swapVoxels( " + carto::toString( byte_order != "ABCD" ) + " )" );

    if ((stride[0] == 1)
     && (stride[1] == ((long)size[0]) * stride[0])
     && (stride[2] == ((long)size[1]) * stride[1])
     && (stride[3] == ((long)size[2]) * stride[2])) {
      localMsg("OpenSlide: reading contiguous area")
      // Optimal case where data can be read completly to contiguous memory
      // area
      openslide_read_region(_osimage,
                            (uint32_t*)dest,
                            posx, posy,
                            level,
                            sizex, sizey);
      swapVoxels(dest, sizex * sizey, byte_order != "ABCD");
    }
    else {
      localMsg("OpenSlide: strides [" 
                                    + carto::toString(stride[0]) + ", "
                                    + carto::toString(stride[1]) + ", "
                                    + carto::toString(stride[2]) + ", "
                                    + carto::toString(stride[3]) + "]"
                       + " differs from ones processed using view sizes ["
                                    + carto::toString(1) + ", "
                                    + carto::toString(((long)size[0]) * stride[0]) + ", "
                                    + carto::toString(((long)size[1]) * stride[1]) + ", "
                                    + carto::toString(((long)size[2]) * stride[2]) + "]");
      localMsg("OpenSlide: reading to not contiguous area using strides")
      // The default is to consider that the region read is an entire tile
      // but if we get tile sizes in the header information, we try to optimize
      // the read using the given tile sizes
      int64_t tsizex = sizex,
              tsizey = sizey;
      
      if (tsizes.size() > 1) {
        try {
          // Due to cache limitations of openslide (32MB that approximatevely 
          // corresponds to 4 tiles RGBA of 1200x1600), it is important
          // to read the data accordingly to this limitation and to optimize 
          // the loading process it is necessary to divide the tile size by 2.
          // It allows to only have to maintain 2 tiles in cache instead of 5.
          // This consequently reduces the number of times that data are read
          // from disk.
          tsizex = tsizes[level][0] / 2;
          tsizey = tsizes[level][1] / 2;
        }
        catch(...){}
      }
      
      // Estimate the number of tiles to read in the region
      int64_t tcountx = (sizex + tsizex - 1) / tsizex;
      int64_t tcounty = (sizey + tsizey - 1) / tsizey;
      localMsg("OpenSlide: tile size " + carto::toString(tsizex)
                                       + "x" + carto::toString(tsizey)
                      + " tile count " + carto::toString(tcountx)
                                       + "x" + carto::toString(tcounty) );

      uint32_t * pdest;
      int64_t offset, tsizexmax, tsizeymax;
//       int32_t progress = 0, progress2 = 0;
        
      for (int64_t t = 0; t < size[3]; ++t) {      
        for (int64_t z = 0; z < size[2]; ++z) {
          // Read tile data by line to the destination buffer.
          // This is necessary to optimize read using
          // openslide cache mechanisms
          for (int64_t ty = 0; ty < tcounty; ++ty) {               
            tsizeymax = std::min(tsizey, sizey - (tsizey * ty));
            for (int64_t tx = 0; tx < tcountx; ++tx) {
              tsizexmax = std::min(tsizex, sizex - (tsizex * tx));
//               progress2 = progress;
//               progress = ((((t * size[2] + z) * tcounty) + ty) * tcountx + tx) * 100
//                        / (size[3] * size[2] * tcounty * tcountx);
//               if (progress != progress2) {
//                 localMsg("tile reading: " + carto::toString(progress) + "%");
//               }
//               localMsg("start tile reading: tx " + carto::toString(tx)
//                                         + " ty " + carto::toString(ty)
//                                         + " " + carto::toString(tsizexmax)
//                                         + "x" + carto::toString(tsizeymax));

              for (int64_t l = 0; l < tsizeymax; ++l) {
                offset = tsizex * tx * stride[0]
                       + (tsizey * ty + l) * stride[1]
                       + z * stride[2]
                       + t * stride[3];

                pdest = ((uint32_t *)dest) + offset;
                openslide_read_region(_osimage,
                                      pdest,
                                      (int64_t)(pos[0] + tsizex * tx)
                                              * _sizes[0][0]
                                              / _sizes[ level ][0], 
                                      (int64_t)(pos[1] + tsizey * ty + l)
                                              * _sizes[0][1]
                                              / _sizes[ level ][1],
                                      level, tsizexmax, 1);
                
                swapVoxels((T*)pdest, tsizexmax, byte_order != "ABCD");
              }

              // localMsg("end tile reading: tx " + carto::toString(tx)
              //                         + " ty " + carto::toString(ty));
            }
          }
        }
      }
    }
  }


  template <typename T>
  ImageReader<T>* OSImageReader<T>::cloneReader() const
  {
    return new OSImageReader;
  }

}

#undef localMsg
#endif
