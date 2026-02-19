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

// // It is not obvious how to fix these warnings, make them non-fatal for now
// #pragma GCC diagnostic warning "-Wsign-compare"

#ifndef SOMAIO_IMAGE_MINCIMAGEWRITER_D_H
#define SOMAIO_IMAGE_MINCIMAGEWRITER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/mincimagewriter.h>               // class declaration
//MINC I/O support
#define OMPI_SKIP_MPICXX // avoid including mpi c++ stuff in an extern "C" context
extern "C"
{
#include <volume_io.h>
}
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

//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                             // heritage
#include <soma-io/io/writer.h>                                // to write minf
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasourceinfo/datasourceinfoloader.h>    // partial writing
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/streamdatasource.h>      // used by writeHeader()
#include <soma-io/datasource/datasource.h>
#include <soma-io/checker/mincformatchecker.h>
#include <soma-io/transformation/affinetransformation3d_base.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/type/types.h>                           // to write header
#include <cartobase/stream/fileutil.h>                          // utilitaires
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
#include <cartobase/config/version.h>
#include <cartobase/containers/nditerator.h>
//--- system -----------------------------------------------------------------
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <vector>
#include <zlib.h>
#include <sys/stat.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "NIFTIIMAGEWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void MincImageWriter<T>::updateParams( DataSourceInfo & dsi )
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
  void MincImageWriter<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
  }


  namespace minc_io
  {

    int _writeMincAttribute( carto::Object hdr,
                             carto::Syntax &sx, int mincid,
                             const std::string & minc_var,
                             nc_type minc_var_type,
                             const std::string & minc_var_parent,
                             const std::string & minc_att,
                             const std::string & aims_att_name );
    int _writeMincHistory( carto::Object hdr, int mincid,
                           const std::string & name );

  }


  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  MincImageWriter<T>::MincImageWriter() :
    ImageWriter<T>()
  {
  }

  template <typename T>
  MincImageWriter<T>::~MincImageWriter()
  {
  }

  //==========================================================================
  //   I M A G E W R I T E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void MincImageWriter<T>::write( const T * source, DataSourceInfo & dsi,
                                    const std::vector<int> & pos,
                                    const std::vector<int> & size,
                                    const std::vector<long> & strides,
                                    carto::Object options )
  {
    // std::cout << "MincImageWriter<T>::write\n";

    if( _sizes.empty() )
      updateParams( dsi );

    unsigned  st = static_cast<unsigned>( _sizes[ 0 ][ 3 ] );
    unsigned  vt = static_cast<unsigned>( size[ 3 ] );
    unsigned  ot = static_cast<unsigned>( pos[ 3 ] );

    carto::Object hdr = dsi.header();
    bool partial = false;
    try
    {
      Object pobj = options->getProperty( "partial_writing" );
      if( pobj )
        partial = pobj->getScalar();
    }
    catch( ... )
    {
    }

    //Only works for 3D volume. Should add 4D support.
    std::string fname = carto::FileUtil::linuxFilename( dsi.url() );

    int n_dimensions=4;

    // ------------------------------------------------------------------------
    // NS-2018-02-28: Current code is not able to deal with n_dimensions other than
    // 4. Today, if n_dimensions is set to 2, only,  y and z dimension information
    // is stored in the MINC file.
    //
    // TODO: Set correct dim_names and sizes vector values depending on n_dimensions
    // for n_dimensions == 1:
    // dim_names[0] and sizes[0] must contain X axis info
    // for n_dimensions == 2:
    // dim_names[0] and sizes[0] must contain Y axis info
    // dim_names[1] and sizes[1] must contain X axis info
    // for n_dimensions == 3:
    // dim_names[0] and sizes[0] must contain Z axis info
    // dim_names[1] and sizes[1] must contain Y axis info
    // dim_names[2] and sizes[2] must contain X axis info
    // for n_dimensions == 4:
    // dim_names[0] and sizes[0] must contain Z axis info
    // dim_names[1] and sizes[1] must contain Y axis info
    // dim_names[2] and sizes[2] must contain X axis info
    // dim_names[3] and sizes[3] must contain T axis info
    //
    // So I disabled the code below and always store data in a 4 dimensions volume
    // ------------------------------------------------------------------------

    //   if(thing.dimT()==1) {
    //     if(thing.dimZ()==1) {
    //       if(thing.dimY()==1) {
    //         n_dimensions=1;
    //       }
    //       else n_dimensions=2;
    //     }
    //     else n_dimensions=3;
    //   }

    VIO_Volume volume = 0;
    VIO_STR dim_names[4];

    dim_names[0] = create_string( const_cast<char*>( MIzspace) );
    dim_names[1] = create_string( const_cast<char*>( MIyspace ) );
    dim_names[2] = create_string( const_cast<char*>( MIxspace ) );
    dim_names[3] = create_string( const_cast<char*>( MItime ) );

    bool ok = true;

    try
    {

      nc_type nc_data_type, nc_disk_data_type;
      VIO_BOOL signed_flag;

      carto::DataTypeCode<T>        dtc;
      bool scaledcoding = false;

    //   T mini = thing.minimum(), maxi = thing.maximum();
      T mini = source[ pos[3] * strides[3] + pos[2] * strides[2]
                       + pos[1] * strides[1] + pos[0] * strides[0] ];
      T maxi = mini;
      const T *dat;

      for( int t=0; t<size[3]; ++t )
        for( int z=0; z<size[2]; ++z )
          for( int y=0; y<size[1]; ++y )
          {
            const T* src = source + (t + pos[3]) * strides[3]
              + (z + pos[2]) * strides[2]
              + (y + pos[1]) * strides[1]
              + pos[0];
            for( int x=0; x<size[0]; ++x )
            {
              dat = src + x * strides[0];
              if( *dat < mini )
                mini = *dat;
              if( *dat > maxi )
                maxi = *dat;
            }
          }

      // It is necessary to avoid dividing by 0 in case where
      // mini and maxi are identical otherwise minc writing
      // will fail.
      if (mini == maxi)
          maxi = mini + 1;

      double dmin = 0., dmax = 0.;
      //std::cout << "Type: " << dtc.dataType() << "\n";
      if(dtc.dataType()=="U8") {
        nc_data_type=NC_BYTE;
        signed_flag=FALSE;
      }
      else if(dtc.dataType()=="S8") {
        nc_data_type=NC_BYTE;
        signed_flag=TRUE;
      }
      else if(dtc.dataType()=="U16") {
        nc_data_type=NC_SHORT;
        signed_flag=FALSE;
      }
      else if(dtc.dataType()=="S16") {
        nc_data_type=NC_SHORT;
        signed_flag=TRUE;
      }
      else if(dtc.dataType()=="U32") {
        nc_data_type=NC_LONG;
        signed_flag=FALSE;
      }
      else if(dtc.dataType()=="S32") {
        nc_data_type=NC_LONG;
        signed_flag=TRUE;
      }
      else if(dtc.dataType()=="FLOAT") {
        nc_data_type=NC_FLOAT;
        /* scale factors don't seem to work.
        float slope = 1., offset = 0.;
        if( canEncodeAsScaledS16( *thing.volume(), slope, offset, true, 0 ) )
        {
          nc_disk_data_type=NC_SHORT;
          scaledcoding = true;
          dmin = rint( ( mini - offset ) / slope );
          dmax = rint( ( maxi - offset ) / slope );
          std::cout << "Minc: use scale factor " << slope << " / " << offset << ", min: " << dmin << ", max: " << dmax << endl;
        }
        */
        signed_flag=TRUE;
      }
      else if(dtc.dataType()=="DOUBLE") {
        nc_data_type=NC_DOUBLE;
        signed_flag=TRUE;
      }
      else
        throw datatype_format_error( std::string( "Unsupported data type " )
                                     + dtc.dataType(), dsi.url() );

      if( !scaledcoding )
      {
        nc_disk_data_type = nc_data_type;
        dmin = mini;
        dmax = maxi;
      }
    //   cout << "MincWriter<" << DataTypeCode<T>::name() << ">::write-1, "
    //        << "n_dimensions: " << carto::toString(n_dimensions)
    //        << "nc_data_type: " << carto::toString(nc_data_type)
    //        << "mini: " << carto::toString(mini)
    //        << "maxi: " << carto::toString(maxi)
    //        << std::endl << std::flush;
      volume = create_volume( n_dimensions,
                              dim_names,
                              nc_data_type,
                              signed_flag,
                              mini,
                              maxi);
      set_volume_real_range( volume, mini, maxi );

      int       sizes[VIO_MAX_DIMENSIONS];
      sizes[3] = size[3];
      sizes[2] = size[0];
      sizes[1] = size[1];
      sizes[0] = size[2];

      set_volume_sizes( volume, sizes );

      alloc_volume_data( volume );

      std::vector<float> vs;
      try
      {
        carto::Object vso = hdr->getProperty( "voxel_size" );
        if( vso && vso->size() != 0 )
        {
          carto::Object it = vso->objectIterator();
          for( ; it->isValid(); it->next() )
            vs.push_back( it->currentValue()->getScalar() );
        }
      }
      catch( ... )
      {
      }
      while( vs.size() < 4 )
        vs.push_back( 1. );
      float minc_sizeX = vs[0];
      float minc_sizeY = vs[1];
      float minc_sizeZ = vs[2];
      float minc_sizeT = vs[3];

      if( hdr->hasProperty( "MINC_voxel_size" ) )
      {
        std::vector<float> minc_vs;

        if( hdr->getProperty( "MINC_voxel_size", minc_vs ) )
        {
          minc_sizeX = minc_vs[0];
          minc_sizeY = minc_vs[1];
          minc_sizeZ = minc_vs[2];
          minc_sizeT = minc_vs[3];
        }
      }

      //Handle positive/negative voxel size.
      //Variables dirX, dirY and dirZ are defined as 1 if voxel size is positive and as -1 if voxel size is negative.
      int dirX = (int)rint( minc_sizeX / vs[0] );
      int dirY = (int)rint( minc_sizeY / vs[1] );
      int dirZ = (int)rint( minc_sizeZ / vs[2] );

      //Variables X_pos, Y_pos and Z_pos are defined as 1 if voxel size is positive and 0 if voxel size is negative.
      //This is further used during the actual reading.
      int X_pos,Y_pos,Z_pos;
      if(dirX>0) X_pos=1;
      else X_pos=0;
      if(dirY>0) Y_pos=1;
      else Y_pos=0;
      if(dirZ>0) Z_pos=1;
      else Z_pos=0;

      VIO_Real separations[VIO_MAX_DIMENSIONS];

      separations[3]=minc_sizeT;
      separations[2]=minc_sizeX;
      separations[1]=minc_sizeY;
      separations[0]=minc_sizeZ;

    //   std::cout << "===== MincWriter::write, separations: ("
    //             << separations[0] << ","
    //             << separations[1] << ","
    //             << separations[2] << ","
    //             << separations[3] << ")"
    //             << std::endl << std::flush;

      set_volume_separations( volume, separations );
      std::vector<int> volsize;
      try
      {
        carto::Object dimo = hdr->getProperty( "volume_dimension" );
        if( dimo && dimo->size() != 0 )
        {
          carto::Object it = dimo->objectIterator();
          for( ; it->isValid(); it->next() )
            volsize.push_back( int( rint(
              it->currentValue()->getScalar() ) ) );
        }
      }
      catch( ... )
      {
      }
      while( volsize.size() < 4 )
        volsize.push_back( 1 );

      for( int t=0; t<size[3]; ++t )
        for( int z=0; z<size[2]; ++z )
          for( int y=0; y<size[1]; ++y )
          {
            const T* src = source + (t + pos[3]) * strides[3]
              + (z + pos[2]) * strides[2]
              + (y + pos[1]) * strides[1]
              + pos[0];
            for( int x=0; x<size[0]; ++x )
            {
              dat = src + x * strides[0];
              set_volume_real_value(
                volume,
                (Z_pos) * volsize[2] - dirZ * z - (Z_pos),
                (Y_pos) * volsize[1] - dirY * y - (Y_pos),
                (X_pos) * volsize[0] - dirX * x - (X_pos),
                t, 0,
                *dat);
            }
          }


      //1) Voxel to world transform
      Object transs;
      try
      {
        // convert AIMS transformation to storage coords
    //     std::cout << "==== MINC::Before getting transformations"
    //               << std::endl << std::flush;
    //     if (! hdr.getProperty( "transformations", transs )) {
    //         // Set default transformation to identity
    //         std::vector<std::vector<float> > vtranss;
    //         vtranss.push_back(Motion().toVector());
    //         transs = Object::value(vtranss);
    //     }
    //     hdr.getProperty( "transformations", transs );
    //     std::cout << "==== MINC::After getting transformations"
    //               << std::endl << std::flush;
        if( hdr->getProperty( "transformations", transs ) &&
            !transs.isNull() && transs->size() != 0 )
        {
          Object t0 = transs->getArrayItem( 0 );
          if( !t0.isNull() )
          {
            AffineTransformation3dBase tr( t0 );
            AffineTransformation3dBase vsm;
            vsm.matrix()(0,0) = vs[0];
            vsm.matrix()(1,1) = vs[1];
            vsm.matrix()(2,2) = vs[2];
            tr *= vsm;
            carto::Object s2mvec;
            try
            {
              s2mvec = hdr->getProperty( "storage_to_memory" );
              AffineTransformation3dBase s2m( s2mvec );
              tr *= s2m;
            }
            catch( ... )
            {
            }
            // handle MINC transform
            VIO_General_transform *gt = get_voxel_to_world_transform(volume);
            gt->type=LINEAR;
            gt->inverse_flag=FALSE;
            gt->linear_transform=(VIO_Transform*)malloc(sizeof(VIO_Transform));
            for(int i=0;i<4;i++)
            {
              for(int j=0;j<4;j++)
                gt->linear_transform->m[j][i] = tr.matrix()(i,j);
            }
            convert_transform_to_starts_and_steps(
              gt,
              get_volume_n_dimensions(volume),
              separations,
              volume->spatial_axes,
              volume->starts,
              volume->separations,
              volume->direction_cosines );
          }
        }
      }
      catch( ... )
      {
        std::cout << "problem while reading transformation" << std::endl;
      }

      //2) Space name
      if( hdr->hasProperty( "MINC_space_type" ) )
      {
        std::string space_name;
        if( hdr->getProperty( "MINC_space_type", space_name) );
          set_volume_space_type(volume,(char*)space_name.c_str());
      }

    //   ncopts = NC_VERBOSE;
      ncopts = 0;

      //3) Other attributes
      bool ok2 = false;
      milog_init( CARTOBASE_STREAM_NULLDEVICE );
      milog_set_verbosity( 0 );
      //std::cout << "MINC Plugin::write: name: " << fname << std::endl << std::flush;
      if( output_volume( (char*)(fname.c_str()),
                          nc_disk_data_type,
                          signed_flag,
                          dmin,
                          dmax,
                          volume,
                          NULL,
                          NULL) != VIO_OK )
        ok = false;
    //   ncopts = NC_VERBOSE;
      ncopts = 0;
      ncerr = 0;
      errno = 0;

      int mincid = -1;
      if( ok )
      {
        mincid = miopen( (char*)(fname.c_str()), NC_WRITE );
        if( mincid >= 0 && ncerr == 0 )
          ok2 = true;
      }
      milog_init("stderr"); // allow again output on stderr
      if( ok2 )
      {
        ncredef(mincid);
        // ncredef sometimes fails with error -33, Not a valid ID
        // I don't know why. So let it go...
        ncerr = 0;
        errno = 0;

        using namespace soma::minc_io;

        SyntaxSet	& s = DataSourceInfoLoader::minfSyntax();
        Syntax	&sx = s[ "__generic__" ];

        _writeMincAttribute( hdr, sx, mincid, "patient", NC_LONG, "rootvariable",
                            "varid", "MINC_patient:varid" );
        _writeMincAttribute( hdr, sx, mincid, "patient", NC_LONG, "rootvariable",
                            "vartype", "MINC_patient:vartype" );
        _writeMincAttribute( hdr, sx, mincid, "patient", NC_LONG, "rootvariable",
                            "version", "MINC_patient:version" );
        _writeMincAttribute( hdr, sx,mincid, "patient", NC_LONG, "rootvariable",
                            "full_name", "patient_id" );
        _writeMincAttribute( hdr, sx,mincid, "patient", NC_LONG, "rootvariable",
                            "identification", "MINC_patient:identification" );
        _writeMincAttribute( hdr, sx,mincid, "patient", NC_LONG, "rootvariable",
                            "birthdate", "MINC_patient:birthdate" );
        _writeMincAttribute( hdr, sx,mincid, "patient", NC_LONG, "rootvariable",
                            "sex", "MINC_patient:sex" );
        _writeMincAttribute( hdr, sx,mincid, "patient", NC_LONG, "rootvariable",
                            "weight", "MINC_patient:weight" );

        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "varid", "MINC_study:varid" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "vartype", "MINC_study:vartype" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "version", "MINC_study:version" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "start_time", "MINC_study:start_time" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "modality", "modality" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "institution", "MINC_study:institution" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "station_id", "MINC_study:station_id" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "procedure", "MINC_study:procedure" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "study_id", "study_id" );
        _writeMincAttribute( hdr, sx,mincid, "study", NC_LONG, "rootvariable",
                            "acquisition_id", "MINC_study:acquisition_id" );

        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "varid", "MINC_acquisition:varid" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "vartype",
                            "MINC_acquisition:vartype" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "version",
                            "MINC_acquisition:version" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "scanning_sequence",
                            "MINC_acquisition:scanning_sequence" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "repetition_time", "tr" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "echo_time", "te" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "inversion_time",
                            "MINC_acquisition:inversion_time" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "flip_angle", "flip_angle" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "num_averages",
                            "MINC_acquisition:num_averages" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "imaging_frequency",
                            "MINC_acquisition:imaging_frequency" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "imaged_nucleus",
                            "MINC_acquisition:imaged_nucleus" );
        _writeMincAttribute( hdr, sx,mincid, "acquisition", NC_LONG,
                            "rootvariable", "comments",
                            "MINC_acquisition:comments" );

        _writeMincAttribute( hdr, sx,mincid, "image-min", NC_DOUBLE, "image",
                            "units", "MINC_image-min:units" );
        _writeMincAttribute( hdr, sx,mincid, "image-max", NC_DOUBLE, "image",
                            "units", "MINC_image-max:units" );

        //Siemens sonata
        _writeMincAttribute( hdr, sx,mincid, "patient", NC_LONG, "rootvariable",
                            "age", "MINC_patient:age" );


        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "start_date", "MINC_study:start_date" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "manufacturer", "MINC_study:manufacturer" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "model", "MINC_study:model" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "field_value", "MINC_study:field_value" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "software_version", "MINC_study:software_version" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "serial_no", "MINC_study:serial_no" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "performing_physician",
                            "MINC_study:performing_physician" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "operator", "MINC_study:operator" );
        _writeMincAttribute( hdr, sx,mincid,"study", NC_LONG, "rootvariable",
                            "calibration_date", "MINC_study:calibration_date" );

        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "acquisition_id",
                            "MINC_acquisition:acquisition_id" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "start_time",
                            "MINC_acquisition:start_time" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "protocol_name",
                            "MINC_acquisition:protocol_name" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "receive_coil",
                            "MINC_acquisition:receive_coil" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "transmit_coil",
                            "MINC_acquisition:transmit_coil" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "echo_number",
                            "MINC_acquisition:echo_number" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "slice_thickness",
                            "MINC_acquisition:slice_thickness" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "num_slices",
                            "MINC_acquisition:num_slices" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "num_dyn_scans",
                            "MINC_acquisition:num_dyn_scans" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "window_center",
                            "MINC_acquisition:window_center" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "window_width",
                            "MINC_acquisition:window_width" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "num_phase_enc_steps",
                            "MINC_acquisition:num_phase_enc_steps" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "percent_sampling",
                            "MINC_acquisition:percent_sampling" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "percent_phase_fov",
                            "MINC_acquisition:percent_phase_fov" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "pixel_bandwidth",
                            "MINC_acquisition:pixel_bandwidth" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "phase_enc_dir",
                            "MINC_acquisition:phase_enc_dir" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "SAR", "MINC_acquisition:SAR" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "mr_acq_type",
                            "MINC_acquisition:mr_acq_type" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "image_type",
                            "MINC_acquisition:image_type" );
        _writeMincAttribute( hdr, sx,mincid,"acquisition", NC_LONG,
                            "rootvariable", "MrProt_dump",
                            "MINC_acquisition:MrProt_dump" );

        _writeMincAttribute( hdr, sx,mincid,"processing", NC_LONG,
                            "rootvariable", "transformation0-filename",
                            "MINC_processing:transformation0-filename" );
        _writeMincAttribute( hdr, sx,mincid,"processing", NC_LONG,
                            "rootvariable", "transformation0-filedata",
                            "MINC_processing:transformation0-filedata" );
        _writeMincHistory( hdr, mincid, dsi.url() );

        miclose( mincid );
      }

      //std::cout << "Delete starts\n";
      delete_string(dim_names[0]);
      delete_string(dim_names[1]);
      delete_string(dim_names[2]);
      delete_string(dim_names[3]);
      //std::cout << "Delete ends\n";
      delete_volume(volume);
      //std::cout << "Delete ends2\n";

    }
    catch( ... )
    {
      //std::cout << "Delete starts\n";
      delete_string(dim_names[0]);
      delete_string(dim_names[1]);
      delete_string(dim_names[2]);
      delete_string(dim_names[3]);
      //std::cout << "Delete ends\n";
      delete_volume(volume);
      //std::cout << "Delete ends2\n";
    }

    if( !ok )
      io_error::launchErrnoExcept( dsi.url() );

    if( fname.substr( fname.length() - 3, 3 ) == ".gz" )
    {
      // gzipped format
      struct stat s;
      int r = stat( fname.c_str(), &s );
      if( r == 0 )
      {
        size_t len = s.st_size;
        char *buf = new char[len + 1];
        std::ifstream is( fname.c_str(), std::ios::in | std::ios::binary );
        is.read( buf, len );
        gzFile zbuf = gzopen( fname.c_str(), "wb" );
        gzwrite( zbuf, buf, len );
        gzclose( zbuf );
        delete buf;
      }
    }

  }


  template <typename T>
  DataSourceInfo MincImageWriter<T>::writeHeader(
    DataSourceInfo & dsi,
    const T * source,
    const std::vector<int> & /* pos */,
    const std::vector<int> & size,
    const std::vector<long> & strides,
    carto::Object options )
  {

    Object hdr = dsi.header();

    //--- build datasourcelist -----------------------------------------------
    bool dolist = dsi.list().typecount() == 1;
    if( dolist )
    {
      localMsg( "building DataSourceList..." );
      buildDSList( dsi.list(), options, dsi.header() );
    }
    //--- set header ---------------------------------------------------------
    localMsg( "setting Header..." );
    if( options->hasProperty( "partial_writing" ) )
    {
      DataSourceInfoLoader  dsil;
      carto::rc_ptr<DataSource> hdrds;
      try
      {
        hdrds = dsi.list().dataSource( "mnc" );
      }
      catch( ... )
      {
        hdrds = dsi.list().dataSource();
      }
      DataSourceInfo hdrdsi( hdrds );
      hdrdsi = dsil.check( hdrdsi );
      dsi.header() = hdrdsi.header();
      dsi.privateIOData()->copyProperties( hdrdsi.privateIOData() );
      return dsi;
    }

//     checkDiskDataType( hdr, source, strides, size, options );

    //--- write header -------------------------------------------------------
    localMsg( "building Header..." );

    updateParams( dsi );

    //--- write minf ---------------------------------------------------------
    localMsg( "writing Minf..." );
    carto::Object minf = dsi.header();
    if( minf->hasProperty( "file_type" ) )
      minf->removeProperty( "file_type" );
    minf->setProperty( "format", "MINC" );
    minf->setProperty( "data_type", carto::DataTypeCode<T>::dataType() );
    minf->setProperty( "object_type", std::string( "Volume" ) );
    std::vector<int> dims( 4, 0 );
    if( !minf->getProperty( "volume_dimension", dims ) )
    {
      minf->getProperty( "sizeX", dims[ 0 ] );
      minf->getProperty( "sizeY", dims[ 1 ] );
      minf->getProperty( "sizeZ", dims[ 2 ] );
      minf->getProperty( "sizeT", dims[ 3 ] );
      minf->setProperty( "volume_dimension", dims );
    }

    Writer<carto::GenericObject> minfw( dsi.list().dataSource( "minf" ) );
    minfw.write( *minf );
//     //--- partial-io case ----------------------------------------------------
//     if( options->hasProperty( "unallocated" ) )
//     {
//       localMsg( "building file for partial writing..." );
//       if( _sizes.empty() )
//         updateParams( dsi );
//
//
//       // TODO
//
//     }

    //------------------------------------------------------------------------
    localMsg( "done writing header." );
    return dsi;
  }

  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T>
  void MincImageWriter<T>::buildDSList( DataSourceList & dsl,
                                        carto::Object options,
                                        carto::Object header ) const
  {
    DataSource* pds = dsl.dataSource().get();
    std::string mncname, minfname, basename;

    mncname = pds->url();
    if( mncname.empty() )
      throw carto::file_not_found_error( "needs a filename", mncname );

    minfname = mncname + ".minf";

    dsl.addDataSource( "mnc", carto::rc_ptr<DataSource>( pds ) );
    dsl.addDataSource( "minf",
      carto::rc_ptr<DataSource>( new FileDataSource( minfname ) ) );

  }

}

#undef localMsg
#endif
