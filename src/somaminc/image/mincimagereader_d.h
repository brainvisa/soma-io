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

#ifndef SOMAIO_IMAGE_MINCIMAGEREADER_D_H
#define SOMAIO_IMAGE_MINCIMAGEREADER_D_H

//MINC I/O support
#include <soma-io/config/somaminc_config.h>
#define OMPI_SKIP_MPICXX // avoid including mpi c++ stuff in an extern "C" context
#define BOOLEAN MINC_BOOLEAN
#define FLOAT MINC_FLOAT
#define DOUBLE MINC_DOUBLE
#define SHORT MINC_SHORT
#include <cstdlib>
extern "C"
{
/* redefine some stupid macros that conflict with
   those of the stupid system on Windows
*/
#include <volume_io.h>
#ifdef AIMS_HAS_MINC2
#include <minc2.h>
#endif
}
#undef SHORT
#undef DOUBLE
#undef FLOAT
#undef BOOLEAN
#undef X
#undef Y
#undef OMPI_SKIP_MPICXX

//--- plugin -----------------------------------------------------------------
#include <soma-io/image/mincimagereader.h>                // class declaration
#include <soma-io/checker/mincformatchecker.h>          // to access the mutex
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/datasource.h>
#include <soma-io/datasource/chaindatasource.h>                    // heritage
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/containers/nditerator.h>
#include <cartobase/stream/fileutil.h>
#include <cartobase/thread/mutex.h>
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "MINCIMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{
  
  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T> 
  void MincImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    if( !dsi.header()->getProperty( "volume_dimension", _sizes[ 0 ] ) )
    {
      dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
      dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
      dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
      dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    }
  }
  
  template <typename T> 
  void MincImageReader<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
  }
  
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  
  template <typename T>
  MincImageReader<T>::MincImageReader() :
    ImageReader<T>()
  {
  }
  
  template <typename T>
  MincImageReader<T>::~MincImageReader()
  {
  }
  
  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void MincImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                 std::vector<int> & pos,
                                 std::vector<int> & size,
                                 std::vector<long> & strides,
                                 carto::Object options )
  {
    // std::cout << "MincImageReader<T>::read\n";

    if( _sizes.empty() )
      updateParams( dsi );

    // dest is supposed to be allocated

#if 0
// #if defined( AIMS_HAS_MINC2 ) && !defined( MINC_MNC2_SUPPORT )
    /* if MINC_MNC2_SUPPORT then the general API also supports mnc2, so we
        don't need this readMinc2(), which moreover hangs on some versions.
    */
    try
    {
      readMinc2( dest, dsi, pos, size, strides, options );
    }
    catch( ... )
    {
      // std::cout << "Minc2 read failed, using volume_io\n";
      readMinc1( dest, dsi, pos, size, strides, options );
    }
#else
    // only Minc1 available
    readMinc1( dest, dsi, pos, size, strides, options );
#endif

    //Should we delete the header "hdr" ?? No.

  }


  template <typename T>
  ImageReader<T>* MincImageReader<T>::cloneReader() const
  {
    return new MincImageReader;
  }


  template <typename T>
  void MincImageReader<T>::readMinc1( T * dest, DataSourceInfo & dsi,
                                      std::vector<int> & pos,
                                      std::vector<int> & size,
                                      std::vector<long> & strides,
                                      carto::Object options )
  {
    carto::Object hdr = dsi.header();

    VIO_Volume volume;
    VIO_STR fileName
      = create_string( const_cast<char*>(carto::FileUtil::linuxFilename(
        dsi.url() ).c_str() ) );

    std::vector<VIO_STR> dim_names( VIO_MAX_DIMENSIONS );

    dim_names[0] = create_string( const_cast<char*>( MIxspace ) );
    dim_names[1] = create_string( const_cast<char*>( MIyspace ) );
    dim_names[2] = create_string( const_cast<char*>( MIzspace ) );
    dim_names[3] = create_string( const_cast<char*>( MItime ) );
    for( unsigned i=4; i<VIO_MAX_DIMENSIONS; ++i )
      dim_names[i] = 0;

    set_print_error_function( MincFormatChecker::my_empty_print_error );

    try
    {
      MincFormatChecker::mincMutex().lock();

      int res = input_volume( fileName, 0, &dim_names[0],
                              MI_ORIGINAL_TYPE, TRUE,
                              0.0, 0.0, TRUE, &volume,
                              (minc_input_options *) NULL );
      MincFormatChecker::mincMutex().unlock();
      if( res != VIO_OK )
      {
        std::cout << "input_volume failed.\n";
        throw carto::corrupt_stream_error( "could not read MINC file",
                                            dsi.url() );
      }


      //Handle positive/negative voxel size.
      //Variables dirX, dirY and dirZ are defined as 1 if voxel size is positive and as -1 if voxel size is negative.
      int dirX = (int)(volume->separations[0]/fabs(volume->separations[0]));
      int dirY = (int)(volume->separations[1]/fabs(volume->separations[1]));
      int dirZ = (int)(volume->separations[2]/fabs(volume->separations[2]));

      //Variables X_pos, Y_pos and Z_pos are defined as 1 if voxel size is positive and 0 if voxel size is negative.
      //This is further used during the actual reading.
      int X_pos, Y_pos, Z_pos;
      if( dirX > 0 )
        X_pos = 1;
      else
        X_pos = 0;
      if( dirY > 0 )
        Y_pos = 1;
      else
        Y_pos = 0;
      if( dirZ > 0 )
        Z_pos = 1;
      else
        Z_pos = 0;

      //std::cout << "X:"<< dirX<<","<<X_pos<<"Y:"<< dirY<<","<<Y_pos<<"Z:"<< dirZ<<","<<Z_pos<<"\n";

      std::vector<int> tsize( size.begin() + 3, size.end() );
      std::vector<int> tstrides( strides.begin() + 3, strides.end() );
      offset_t offset;
      T *target;

      //std::cout << "reading\n";

      carto::DataTypeCode<T>  dtc;
      //std::cout << "Output type: " << dtc.dataType() << "\n";

      //Read the volume according to the output data type (T, which can be U8, S8, S16, FLOAT...) and to the read mode which can be "real" or "voxel"
      //In every case, when the voxel size is negative the values are read "as usual" and when the voxel size is positive, values are read using a "mirror symmetry". (this is quite odd, I don't know why it is not the other way round.
      std::string read_mode = "real";
      try
      {
        read_mode = options->getProperty( "read_mode" )->getString();
      }
      catch( ... )
      {
      }

      //Use the read mode "real" (i.e. read real values using the function "get_volume_real_value"
      if( read_mode=="real" )
      {
        // std::cout << "minc read\n";
        //If the output type is integer, "round" the values to the nearest integer (mostly necessary for label volumes)
        if ( (dtc.dataType()=="U8") || (dtc.dataType()=="S8")
              || (dtc.dataType()=="U16") || (dtc.dataType()=="S16")
              || (dtc.dataType()=="U32") || (dtc.dataType()=="S32") )
        {
          NDIterator_base it( tsize, tstrides );
          for( ; !it.ended(); ++it )
            for( int z=0; z<size[2]; ++z )
              for( int y=0; y<size[1]; ++y )
              {
                offset = it.offset() + z * strides[2] + y * strides[1];
                target = dest + offset;
                for( int x=0; x<size[0]; ++x )
                {
                  *target++ = (T)rint(get_volume_real_value(
                    volume,
                    (X_pos) * _sizes[0][0] - dirX * ( x + pos[0] ) - (X_pos),
                    (Y_pos) * _sizes[0][1] - dirY * ( y + pos[1] ) - (Y_pos),
                    (Z_pos) * _sizes[0][2] - dirZ * ( z + pos[2] ) - (Z_pos),
                    pos[3] + it.position()[0], 0 ));
                }
              }
        }
        else
        {
          NDIterator_base it( tsize, tstrides );
          for( ; !it.ended(); ++it )
            for( int z=0; z<size[2]; ++z )
              for( int y=0; y<size[1]; ++y )
              {
                offset = it.offset() + z * strides[2] + y * strides[1];
                target = dest + offset;
                for( int x=0; x<size[0]; ++x )
                {
                  *target++ = (T)get_volume_real_value(
                    volume,
                    (X_pos) * _sizes[0][0] - dirX * ( x + pos[0] ) - (X_pos),
                    (Y_pos) * _sizes[0][1] - dirY * ( y + pos[1] ) - (Y_pos),
                    (Z_pos) * _sizes[0][2] - dirZ * ( z + pos[2] ) - (Z_pos),
                    pos[3] + it.position()[0], 0 );
                }
              }
        }

      }
      //Use the read mode "voxel" (i.e. read voxel values using the function "get_volume_voxel_value"
      else
      {
        std::cout << "minc voxel mode\n";
        NDIterator_base it( tsize, tstrides );
        for( ; !it.ended(); ++it )
          for( int z=0; z<size[2]; ++z )
            for( int y=0; y<size[1]; ++y )
            {
              offset = it.offset() + z * strides[2] + y * strides[1];
              target = dest + offset;
              for( int x=0; x<size[0]; ++x )
              {
                *target++ = (T)get_volume_voxel_value(
                  volume,
                    (X_pos) * _sizes[0][0] - dirX * ( x + pos[0] ) - (X_pos),
                    (Y_pos) * _sizes[0][1] - dirY * ( y + pos[1] ) - (Y_pos),
                    (Z_pos) * _sizes[0][2] - dirZ * ( z + pos[2] ) - (Z_pos),
                    pos[3] + it.position()[0], 0 );
              }
            }
      }
      delete_volume(volume);
      delete_string(fileName);
      delete_string(dim_names[0]);
      delete_string(dim_names[1]);
      delete_string(dim_names[2]);
      delete_string(dim_names[3]);
    }
    catch( ... )
    {
      delete_volume(volume);
      delete_string(fileName);
      delete_string(dim_names[0]);
      delete_string(dim_names[1]);
      delete_string(dim_names[2]);
      delete_string(dim_names[3]);

      throw;
    }

  }


  // ----- Freesurfer support ------

    template <typename T>
  FreeSurferMincImageReader<T>::FreeSurferMincImageReader() :
    MincImageReader<T>()
  {
  }

  template <typename T>
  FreeSurferMincImageReader<T>::~FreeSurferMincImageReader()
  {
  }

  template <typename T>
  ImageReader<T>* FreeSurferMincImageReader<T>::cloneReader() const
  {
    return new FreeSurferMincImageReader;
  }

}

#undef localMsg
#endif
