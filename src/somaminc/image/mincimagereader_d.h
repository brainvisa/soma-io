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

#ifndef VIO_FLOOR
// most recent type changes - is there a version anywhere ?
#define VIO_OK OK
typedef unsigned int aims_misize_t;
#else
typedef misize_t aims_misize_t;
#endif
#ifndef VIO_MAX_DIMENSIONS
// happens in some versions of minc...
#define VIO_MAX_DIMENSIONS MAX_DIMENSIONS
#endif

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

      minc_input_options minc_opt;
      set_default_minc_input_options( &minc_opt );
      // preserve values (doesn't seem to work for inf...)
      set_minc_input_promote_invalid_to_min_flag( &minc_opt, FALSE );

//       int res = input_volume( fileName, 0, &dim_names[0],
//                               MI_ORIGINAL_TYPE, TRUE,
//                               0.0, 0.0, TRUE, &volume,
//                               &minc_opt );
      volume_input_struct input_info;
      int res = start_volume_input( fileName, 1, &dim_names[0],
                                    MI_ORIGINAL_TYPE, TRUE,
                                    0.0, 0.0, TRUE, &volume,
                                    &minc_opt, &input_info );
      if( res != VIO_OK )
      {
        MincFormatChecker::mincMutex().unlock();
        throw carto::corrupt_stream_error( "could not read MINC file",
                                            dsi.url() );
      }

      delete_volume_input( &input_info );

//       Minc_file minc_file = get_volume_input_minc_file( &input_info );
//       int minc_id = get_minc_file_id( minc_file );

      MincFormatChecker::mincMutex().unlock();


      //Handle positive/negative voxel size.
      //Variables dirX, dirY and dirZ are defined as 1 if voxel size is positive and as -1 if voxel size is negative.
      std::vector<float> mvs;
      hdr->getProperty( "MINC_voxel_size", mvs );
      int dirX = (int)(mvs[0]/fabs(mvs[0]));
      int dirY = (int)(mvs[1]/fabs(mvs[1]));
      int dirZ = (int)(mvs[2]/fabs(mvs[2]));

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
//       std::cout << "size: " << size[0] << ", " << size[1] << ", " << size[2] << ", " << size[3] << std::endl;
//       std::cout << "pos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << std::endl;

      std::vector<int> tsize; //( size.begin() + 3, size.end() );
      std::vector<int> tstrides; //( strides.begin() + 3, strides.end() );
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
      int mode = 0; // 0: float, 1: int, 2: voxel
      if( read_mode != "real" )
        mode = 2;
      else
      {
        if( (dtc.dataType()=="U8") || (dtc.dataType()=="S8")
            || (dtc.dataType()=="U16") || (dtc.dataType()=="S16")
            || (dtc.dataType()=="U32") || (dtc.dataType()=="S32") )
          mode = 1;
      }

      // std::cout << "minc read\n";

      Minc_file mf = initialize_minc_input( fileName, volume, &minc_opt );
      VIO_Real fdone = 0;

      std::cout << "spatial_axes: " << mf->spatial_axes[0] << ", " << mf->spatial_axes[1] << ", " << mf->spatial_axes[2] << std::endl;

      std::cout << "n vols: " << get_n_input_volumes( mf ) << std::endl;

      std::cout << "dim_names: " << mf->dim_names[0] << ", " << mf->dim_names[1] << ", " << mf->dim_names[2] << std::endl;
      std::cout << "to_file_index: " << mf->to_file_index[0] << ", " << mf->to_file_index[1] << ", " << mf->to_file_index[2] << std::endl;
      std::cout << "to_volume_index: " << mf->to_volume_index[0] << ", " << mf->to_volume_index[1] << ", " << mf->to_volume_index[2] << ", " << mf->to_volume_index[3] << std::endl;
      std::cout << "indices: " << mf->indices[0] << ", " << mf->indices[1] << ", " << mf->indices[2] << ", " << mf->indices[3] << std::endl;
      std::cout << "sizes_in_file: " << mf->sizes_in_file[0] << ", " << mf->sizes_in_file[1] << ", " << mf->sizes_in_file[2] << std::endl;

      int d = 0;
      std::vector<int> index;
      for( int i=mf->n_file_dimensions-1; i>=0; --i )
      {
        if( std::string(mf->dim_names[i]) == MIxspace )
          d = 0;
        else if( std::string(mf->dim_names[i]) == MIyspace )
          d = 1;
        else if( std::string(mf->dim_names[i]) == MIzspace )
          d = 2;
        else if( std::string(mf->dim_names[i]) == MItime )
          d = 3;
        else if( i < mf->n_file_dimensions-1 )
          ++d;
        index.push_back( d );
        std::cout << "index: " << i << ", " << index[index.size()-1] << std::endl;
      }
      for( int i=index.size(); i<_sizes[0].size(); ++i )
        index.push_back(i);

      for( int i=3; i<index.size(); ++i )
      {
        if( index[i] < size.size() )
        {
          tsize.push_back( size[index[i]] );
          tstrides.push_back( strides[index[i]] );
        }
        else
        {
          tsize.push_back( 1 );
          tstrides.push_back( 1 );
        }
        std::cout << "tsize: " << tsize[i-3] << ", str: " << tstrides[i-3] << std::endl;
      }

      // skip volumes under min
      int nskip = 0;
      std::vector<int> skipsize( 2, 1 );
      for( int i=0; i<pos.size(); ++i )
      {
        if( i >= 2 )
          skipsize.push_back( _sizes[0][index[i-1]] );
        skipsize[i] *= _sizes[0][index[i]];
        if( i >= 3 )
          nskip += skipsize[index[i]] * pos[index[i]];
      }
      std::cout << "skip rows before vol: " << nskip << std::endl;
      for( int i=0; i<nskip; ++i )
        advance_input_volume( mf );

      NDIterator_base it( tsize, tstrides );
      for( ; !it.ended(); ++it )
      {
        int skip_z = std::min(
          (Z_pos) * _sizes[0][index[2]] - dirZ * pos[index[2]],
          (Z_pos) * _sizes[0][index[2]] - dirZ * ( size[index[2]] + pos[index[2]] ) );
        std::cout << "skip slices: " << skip_z << std::endl;
        skip_z *= _sizes[0][index[1]];
        std::cout << "rows: " << skip_z << std::endl;
        for( int z=0; z<skip_z; ++z )
          advance_input_volume( mf );

        for( int z=0; z<size[index[2]]; ++z )
        {
          int minc_z = (Z_pos) * size[index[2]] - dirZ * z - Z_pos;
          int skip_y = std::min(
            (Y_pos) * _sizes[0][index[1]] - dirY * pos[index[1]],
            (Y_pos) * _sizes[0][index[1]] - dirY * ( size[index[1]] + pos[index[1]] ) );
          std::cout << "skip rows: " << skip_y << std::endl;
          for( int y=0; y<skip_y; ++y )
            advance_input_volume( mf );

          for( int y=0; y<size[index[1]]; ++y )
          {
            // read the next row
            while( input_more_minc_file( mf, &fdone ) )
            {
//               std::cout << "\r" << z << " / " << size[2] << ": " << fdone << "  " << std::flush;
            }
            advance_input_volume( mf );

            int minc_y = (Y_pos) * size[index[1]] - dirY * y - Y_pos;
            offset = it.offset() + minc_z * strides[index[2]] + minc_y * strides[index[1]];
            target = dest + offset;

            switch( mode )
            {
              // 0, 1: Use the read mode "real" (i.e. read real values using the function "get_volume_real_value"
            case 0:
              // float mode
              for( int x=0; x<size[index[0]]; ++x )
              {
                target[strides[index[0]] * x] = (T)get_volume_real_value(
                  volume,
                  (X_pos) * _sizes[0][index[0]] - dirX * ( x + pos[index[0]] ) - (X_pos),
                  0, 0, 0, 0 );
              }
              break;
            case 1:
              // If the output type is integer, "round" the values to the nearest integer (mostly necessary for label volumes)
              for( int x=0; x<size[0]; ++x )
              {
                target[strides[index[0]] * x] = (T)rint( get_volume_real_value(
                  volume,
                  (X_pos) * _sizes[0][index[0]] - dirX * ( x + pos[index[0]] ) - (X_pos),
//                   (Y_pos) * _sizes[0][1] - dirY * ( y + pos[1] ) - (Y_pos),
//                   (Z_pos) * _sizes[0][2] - dirZ * ( z + pos[2] ) - (Z_pos),
//                   pos[3] + it.position()[0], 0 ));
                  0, 0, 0, 0 ) );
              }
              break;
            default:
              // Use the read mode "voxel" (i.e. read voxel values using the function "get_volume_voxel_value"
              for( int x=0; x<size[index[0]]; ++x )
              {
                target[strides[index[0]] * x] = (T)get_volume_voxel_value(
                  volume,
                  (X_pos) * _sizes[0][index[0]] - dirX * ( x + pos[index[0]] ) - (X_pos),
                  0, 0, 0, 0 );
              }
            }
          }

          std::cout << "EOS, skip: " << _sizes[0][index[1]] - skip_y - size[index[1]] << std::endl;
          for( int y=skip_y + size[index[1]]; y<_sizes[0][index[1]]; ++y )
            advance_input_volume( mf );

        }
        std::cout << "EOV, skip: " << _sizes[0][index[1]] * (_sizes[0][index[2]] - skip_z/_sizes[0][index[1]] - size[index[2]]) << std::endl;
        for( int z=skip_z/_sizes[0][index[1]] + size[index[2]]; z<_sizes[0][index[2]]; ++z )
          for( int y=0; y<_sizes[0][index[1]]; ++y )
            advance_input_volume( mf );
      }
      close_minc_input( mf );

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


#ifdef MINC_MGH_SUPPORT

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

#endif // MINC_MGH_SUPPORT

}

#undef localMsg
#endif
