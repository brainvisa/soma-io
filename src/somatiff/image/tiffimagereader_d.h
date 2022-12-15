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

#ifndef SOMAIO_IMAGE_TIFFIMAGEREADER_D_H
#define SOMAIO_IMAGE_TIFFIMAGEREADER_D_H

//--- tiff library -----------------------------------------------------------
extern "C" {
#include <tiffio.h>
}
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/tiffimagereader.h>                 // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/datasource.h>
//#include <soma-io/datasource/chaindatasource.h>                    // heritage
#include <soma-io/reader/itemreader.h>                      // read + byteswap
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "TIFFIMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma {
  
  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T> 
  void TiffImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    
    // Get multi file format info
    dsi.privateIOData()->getProperty( "tiff_info", _mfi );
    
    // Initializes 
  }
  
  template <typename T> 
  void TiffImageReader<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
  }
  
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  /* base constructors usage :
   * - FileDataSource( rc_ptr<DataSource> ds, const string & url = string() );
   * - /!\ no copy constructor
   * - ImageReader( DataSourceInfo & dsi = 0, bool threadsafe = false );
   * - ImageReader( const ImageReader<T> & );
   */
  
  template <typename T>
  TiffImageReader<T>::TiffImageReader() :
    ImageReader<T>()
  {
      //localMsg("building tiff image reader for " + carto::DataTypeCode<T>::name());
  }
  
  template <typename T>
  TiffImageReader<T>::~TiffImageReader()
  {
  }
  
  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void TiffImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<long> & stride,
                                carto::Object      options )
  {
    //localMsg( "Reading using TIFF reader ..." );
    if( _sizes.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "S16" )
      readType<int16_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "FLOAT" )
      readType<float>( dest, dsi, pos, size, stride, options );
    else if( dt == "S8" )
      readType<int8_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U8" )
      readType<uint8_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U16" )
      readType<uint16_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "S32" )
      readType<int32_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U32" )
      readType<uint32_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "DOUBLE" )
      readType<double>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }

  // specialize RGB and RGBA
  template <>
  void TiffImageReader<carto::VoxelRGB>::read( carto::VoxelRGB * dest,
                                               DataSourceInfo & dsi,
                                               std::vector<int> & pos,
                                               std::vector<int> & size,
                                               std::vector<long> & stride,
                                               carto::Object      options )
  {
    if( _sizes.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    // RGB data are necessary read using RGBA types
    if( dt == "RGBA" )
      readType<carto::VoxelRGBA>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }


  template <>
  void TiffImageReader<carto::VoxelRGBA>::read( carto::VoxelRGBA * dest,
                                DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<long> & stride,
                                carto::Object      options )
  {
    if( _sizes.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "RGBA" )
      readType<carto::VoxelRGBA>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }

  
  template <typename T>
  template <typename U>
  void TiffImageReader<T>::readType( T * dest, DataSourceInfo & /* dsi */,
                                     std::vector<int> & pos,
                                     std::vector<int> & size,
                                     std::vector<long> & stride,
                                     carto::Object      /* options */ )
  {
    // dest is supposed to be allocated

    // region size
    int  vx = size[ 0 ];
    int  vy = size[ 1 ];
    int  vz = size[ 2 ];
    int  vt = size[ 3 ];
    // region position
    int  ox = pos[ 0 ];
    int  oy = pos[ 1 ];
    int  oz = pos[ 2 ];
    int  ot = pos[ 3 ];
    
    // strides in dest bytes data
    if( stride.size() < 4 )
      stride.resize( 4, 0 );
    if( stride[0] == 0 )
      stride[0] = 1;
    if( stride[1] == 0 )
      stride[1] = vx * stride[0];
    if( stride[2] == 0 )
      stride[2] = vy * stride[1];
    if( stride[3] == 0 )
      stride[3] = vz * stride[2];
    
    // Get file names to use during region reading
    std::string filename;
    int32_t tiffsx, tiffsy; // sizes of read tiff image
    int tiled, sstart, send, srows;
    long x, y, z, t, s, ystrip, bsize;
    size_t c;
    size_t bpos = 0, // position in bytes buffer
           dpos = 0; // position in destination data
    uint dirmin = 0, dirmax = 0; // tiff directories to use in files
    ushort photometric;
    byte* buffer;
    bool isrgba = (DataTypeCode<U>::dataType() == DataTypeCode<carto::VoxelRGBA>::dataType()); // Disk data type is rgba

    // Get files and tiff directories to use for reading
    localMsg("Try to read tiff file using format: " + _mfi.type); 
    switch (_mfi.type) {
      case MultiFileFormatInfo::Single :
      case MultiFileFormatInfo::Time :
        // Z dimension is stored in tiff directories
        dirmin = oz;
        dirmax = oz + vz;
        break;
        
      case MultiFileFormatInfo::Slice :
      case MultiFileFormatInfo::SliceTime :
        // Only use the first tiff directories because Z dimension
        // is stored in different files
        dirmin = 0;
        dirmax = 1;
        break;
    }

    TIFFSetWarningHandler( 0 );
    
    for (t = 0; t < vt; ++t) {
      for ( z = 0; z < vz; z += (dirmax - dirmin) ) {
        filename = MultiFileFormat::filename( _mfi,
                                              _mfi.slicemin + oz + z,
                                              _mfi.timemin + ot + t );
        
        localMsg("Try to read slice " + carto::toString(z) + " for time " + carto::toString(t)
                 + " from file: " + filename); 
        // Open the tiff file to read
        TIFF* tif = TIFFOpen(filename.c_str(), "r");
        if (tif) {            
          localMsg("File opened successfully"); 
          tiled = TIFFIsTiled(tif);
          localMsg("File contain tiles: " + carto::toString(tiled)); 
          if (tiled) {
            throw carto::invalid_format_error( "TIFF image reader does not support tiled image : " 
                                               "can't read" );
          }
          else {
            // Check file sizes
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &tiffsx);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &tiffsy);
            
//             if ( (tiffsx != sx ) 
//                  || (tiffsy != sy ) ) {
//               throw std::runtime_error( "TIFF image " + filename + " has no consistent dimensions within the volume" );
//             }
            
            for ( uint d = 0; d < (dirmax - dirmin); ++d ) {
              TIFFSetDirectory(tif,  d + dirmin );
              // Read tif directory properties
              TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &srows);
              TIFFGetFieldDefaulted(tif, TIFFTAG_PHOTOMETRIC, &photometric);

              // As TIFFStripSize does not return number of bytes in RGBA cases,
              // we do not use it anymore
              bsize = srows * tiffsx * sizeof(U);
              sstart = oy / srows; // Starting strip for the region
              send = (oy + vy + srows - 1) / srows; // Ending strip for the region
              buffer = new byte[ bsize ];
              std::vector<long> bstride(2, 0), // Strides in buffer
                                boffset(2, 0); // Offsets in buffer
              bstride[0] = 1;
              bstride[1] = bstride[0] * tiffsx;
              boffset[0] = ox;
              boffset[1] = oy % srows;
              
              localMsg( "Read data type: " + carto::DataTypeCode<T>::name() );
              localMsg( "Read disk data type: " + carto::DataTypeCode<U>::name() );
              localMsg( "Buffer size: " + carto::toString(bsize));
              localMsg( "Strip size: " + carto::toString(TIFFStripSize(tif)) );
              localMsg( "Strip rows: " + carto::toString(srows) );
              localMsg( "Photometric does not use palette: " + carto::toString(photometric != PHOTOMETRIC_PALETTE) );
              localMsg( "Strip start: " + carto::toString(sstart) );
              localMsg( "Strip end: " + carto::toString(send) );
              localMsg( "Strides in buffer: [" + carto::toString(bstride[0])
                                         + ", " + carto::toString(bstride[1]) + "]");
              localMsg( "Offsets in buffer: [" + carto::toString(boffset[0])
                                         + ", " + carto::toString(boffset[1]) + "]");
              localMsg( "Strip range: [" + carto::toString(sstart)
                                         + ", " + carto::toString(send) + "]");
              localMsg( "Region size: [" + carto::toString(vx)
                                         + ", " + carto::toString(vy) + "]");
              // Read strips for the region
              y = 0;
              for ( s = sstart; s < send; ++s  ) {
                if (!isrgba) {
                  TIFFReadEncodedStrip(tif, s, buffer, bsize);
                }
                else {
//                   std::cout << "Reading strip: " << carto::toString(s) << " ...";
                  TIFFReadRGBAStrip(tif, s * srows, (uint32_t*)buffer );
//                   std::cout << " OK" << std::endl;
                }

                // Copy buffer region to data
                for (ystrip = boffset[1]; ((ystrip < srows) && (y < vy)); ++y, ++ystrip )
                  for (x = 0; x < vx; ++x) {
                    if ((x < tiffsx)
                     && (y < tiffsy)) {
                      // Position in buffer
                      bpos = (!isrgba ? ystrip : srows - ystrip - 1) * bstride[1]
                           + (boffset[0] + x) * bstride[0];
                      // Position in destination data
                      dpos = t * stride[3]
                           + (z + d) * stride[2]
                           + y * stride[1]
                           + x * stride[0];

                      if(photometric == PHOTOMETRIC_MINISWHITE) {
                        for (c = 0; (c < sizeof(T)) && (c < sizeof(U)); ++c) {
                          buffer[bpos * sizeof(U) + c] =~ buffer[bpos * sizeof(U) + c];
                        }
                      }
                      
                      dest[dpos] = ((U*)buffer)[bpos];
                    }
                    else {
                      dest[dpos] = (T)0;
                    }
                  }
                  
                // offset y in buffer is only for the first stride read
                // because only the first and last strides can be incomplete
                boffset[1] = 0;
              }
              delete[] buffer;
            }
          }
          
          TIFFClose(tif);
        }
        else {
          throw carto::invalid_format_error( "TIFF image reader can't read file : "
                                             + filename );
        }
      }
    }
  }

  template <typename T>
  ImageReader<T>* TiffImageReader<T>::cloneReader() const
  {
    return new TiffImageReader;
  }

}

#undef localMsg
#endif
