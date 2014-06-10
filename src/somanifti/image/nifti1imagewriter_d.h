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

#ifndef SOMAIO_IMAGE_NIFTI1IMAGEWRITER_D_H
#define SOMAIO_IMAGE_NIFTI1IMAGEWRITER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/nifti1imagewriter.h>               // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                             // heritage
#include <soma-io/io/writer.h>                                // to write minf
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasourceinfo/datasourceinfoloader.h>    // partial writing
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/streamdatasource.h>      // used by writeHeader()
#include <soma-io/datasource/datasource.h>
#include <soma-io/nifticlib/niftilib/nifti1_io.h>
#include <soma-io/checker/nifti1formatchecker.h>
#include <soma-io/checker/transformation.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/type/types.h>                           // to write header
#include <cartobase/stream/fileutil.h>                          // utilitaires
#include <cartobase/stream/fdinhibitor.h>
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
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
  void Nifti1ImageWriter<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );

    dsi.privateIOData()->getProperty( "nifti_structure", _nim );
  }

  template <typename T>
  void Nifti1ImageWriter<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
  }

}

namespace
{
  void dataTOnim_checks2m( soma::AffineTransformation3d & m, 
                           const soma::Point3df & p,
                           const soma::Point3df & tp, int ip, int il )
  {
    if( tp[il] != 0 )
    {
      if( tp[il] < 0 )
        m.matrix(il, 3) = p[ip];
      else
        m.matrix(il, 3) = 0;
    }
  }


  template <typename T>
  bool expandNiftiScaleFactor( const carto::Object & hdr,
                               nifti_image *nim, 
                               const soma::AffineTransformation3d & m,
                               const T* data, int tmin, int tmax,
                               znzFile zfp, 
                               const std::vector<long> & strides )
  {
    std::cout << "in expandNiftiScaleFactor\n";
    soma::Point3df d0f;
    std::vector<int> d0(4);
    soma::Point3df incf = m.transform( soma::Point3df( 1, 0, 0 ) )
        - m.transform( soma::Point3df( 0, 0, 0 ) );
    std::vector<int> inc(3);
    inc[0] = int( rint( incf[0] ) );
    inc[1] = int( rint( incf[1] ) );
    inc[2] = int( rint( incf[2] ) );
    long pinc = inc[2] * strides[2]
      + inc[1] * strides[1]
      + inc[0] * strides[0];
    const T *p0;
    bool scalef;
    std::vector<float> s(2);
    if( hdr->getProperty( "scale_factor_applied", scalef ) && scalef
        && hdr->getProperty( "scale_factor", s[0] )
        && hdr->getProperty( "scale_offset", s[1] ) )
    {
      size_t numbytes = nim->nx * sizeof( int16_t ), ss;
      std::vector<int16_t> buf( nim->nx );
      int16_t *d = 0;
      for( int t=tmin; t<tmax; ++t )
        for( int z=0; z<nim->nz; ++z )
          for( int y=0; y<nim->ny; ++y )
          {
            d0f = m.transform( soma::Point3df( 0, y, z ) );
            d0[0] = int( rint( d0f[0] ) );
            d0[1] = int( rint( d0f[1] ) );
            d0[2] = int( rint( d0f[2] ) );
            d0[3] = t;
            d = &buf[0];
            p0 = data + d0[3] * strides[3] + d0[2] * strides[2]
              + d0[1] * strides[1] + d0[0] * strides[0];
            for( int x=0; x<nim->nx; ++x, p0 += pinc )
              *d++ = (int16_t) rint( (*p0 - s[1]) / s[0] );

            ss = znzwrite( (void*) &buf[0] , 1 , numbytes , zfp );
            if( ss != numbytes )
            {
              y = nim->ny;
              z = nim->nz;
              t = tmax;
              break;
            }
          }
      return true;
    }
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const carto::VoxelRGB*,
                               int, int, znzFile, 
                               const std::vector<long> & )
  {
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const carto::VoxelRGBA*,
                               int, int, znzFile, 
                               const std::vector<long> & )
  {
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const carto::VoxelHSV*,
                               int, int, znzFile, 
                               const std::vector<long> & )
  {
    return false;
  }


  template <typename T>
  void dataTOnim( nifti_image *nim, carto::Object & hdr,
                  const T* source, int tt, znzFile zfp, 
                  std::vector<long> & strides )
  {
    soma::AffineTransformation3d m;
    try
    {
      carto::Object storage_to_memory;
      storage_to_memory = hdr->getProperty( "storage_to_memory" );
      m = storage_to_memory;
      /* adjust translations so that they fit (in case the vol size has
         changed) */
      soma::Point3df p( nim->nx - 1, 0, 0 ), p0( 0, 0, 0 );
      soma::Point3df tp = m.transform( p ) - m.transform( p0 );
      dataTOnim_checks2m( m, p, tp, 0, 0 );
      dataTOnim_checks2m( m, p, tp, 0, 1 );
      dataTOnim_checks2m( m, p, tp, 0, 2 );
      p = soma::Point3df( 0, nim->ny - 1, 0 );
      tp = m.transform( p ) - m.transform( p0 );
      dataTOnim_checks2m( m, p, tp, 1, 0 );
      dataTOnim_checks2m( m, p, tp, 1, 1 );
      dataTOnim_checks2m( m, p, tp, 1, 2 );
      p = soma::Point3df( 0, 0, nim->nz - 1 );
      tp = m.transform( p ) - m.transform( p0 );
      dataTOnim_checks2m( m, p, tp, 2, 0 );
      dataTOnim_checks2m( m, p, tp, 2, 1 );
      dataTOnim_checks2m( m, p, tp, 2, 2 );
    }
    catch( ... )
    {
      m.matrix(0, 3) = nim->nx - 1;
      m.matrix(1, 3) = nim->ny - 1;
      m.matrix(2, 3) = nim->nz - 1;
      m.matrix(0,0) = -1.0;
      m.matrix(1,1) = -1.0;
      m.matrix(2,2) = -1.0;
    }
    hdr->setProperty( "storage_to_memory", m.toVector() );

    int tmin, tmax;
    if(tt < 0)
    {
      tmin = 0;
      tmax = nim->nt;
    }
    else
    {
      tmin = tt;
      tmax = tt+1;
    }

    if( !expandNiftiScaleFactor( hdr, nim, m, source, tmin, tmax, zfp, 
      strides ) )
    {
      soma::Point3df d0f;
      std::vector<int> d0(4);
      soma::Point3df incf = m.transform( soma::Point3df( 1, 0, 0 ) )
          - m.transform( soma::Point3df( 0, 0, 0 ) );
      std::vector<int> inc(3);
      inc[0] = int( rint( incf[0] ) );
      inc[1] = int( rint( incf[1] ) );
      inc[2] = int( rint( incf[2] ) );

      long pinc = inc[2] * strides[2]
        + inc[1] * strides[1]
        + inc[0] * strides[0];
      const T *p0;
      size_t numbytes = nim->nx * sizeof( T ), ss;
      std::vector<T> buf( nim->nx );
      T *d = 0;

      for( int t=tmin; t<tmax; ++t )
        for( int z=0; z<nim->nz; ++z )
          for( int y=0; y<nim->ny; ++y )
          {
            d0f = m.transform( soma::Point3df( 0, y, z ) );
            d0[0] = int( rint( d0f[0] ) );
            d0[1] = int( rint( d0f[1] ) );
            d0[2] = int( rint( d0f[2] ) );
            d0[3] = t;
            p0 = source + d0[3] * strides[3] + d0[2] * strides[2]
              + d0[1] * strides[1] + d0[0] * strides[0];
            d = &buf[0];
            for( int x=0; x<nim->nx; ++x, p0 += pinc )
              *d++ = *p0;
            ss = znzwrite( (void*) &buf[0] , 1 , numbytes , zfp );
            if( ss != numbytes )
            {
              // ok = false;
              y = nim->ny;
              z = nim->nz;
              t = tmax;
              break;
            }
          }
    }
  }

} // namespace {}


namespace soma
{
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  Nifti1ImageWriter<T>::Nifti1ImageWriter() :
    ImageWriter<T>()
  {
  }

  template <typename T>
  Nifti1ImageWriter<T>::~Nifti1ImageWriter()
  {
  }

  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void Nifti1ImageWriter<T>::write( T * source, DataSourceInfo & dsi,
                                 std::vector<int> & pos,
                                 std::vector<int> & size,
                                 carto::Object options )
  {
    std::cout << "Nifti1ImageWriter.write\n";
    if( _sizes.empty() || _nim.isNull() )
      updateParams( dsi );

    // total volume size
    int  sx = _sizes[ 0 ][ 0 ];
    int  sy = _sizes[ 0 ][ 1 ];
    int  sz = _sizes[ 0 ][ 2 ];
    int  st = _sizes[ 0 ][ 3 ];

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

    int  y, z, t;
    // region line size
    offset_t offset;

//     if( options->hasProperty( "partial_writing" )
//       && !open( DataSource::ReadWrite ) )
//       throw carto::open_error( "data source not available", url() );
//     else if( !open( DataSource::Write ) )
//       throw carto::open_error( "data source not available", url() );

    // TODO
    bool  write4d = true;
    try
    {
      if( !options.isNull() )
        write4d = (bool)
          options->getProperty( "nifti_output_4d_volumes" )->getScalar();
    }
    catch( std::exception & )
    {
    }

    if( sx >= 0x8000 || sy >= 0x8000 || sz >= 0x8000 )
      throw carto::invalid_format_error( "NIFTI-1 cannot handle volume "
        "dimensions exceeding 32737", dsi.url() );
    int ntime = st;
    if( write4d && st >= 0x8000 )
    {
      ntime = 1;
      write4d = false;
    }

    std::string code = carto::DataTypeCode<T>().dataType();
    carto::Object hdr = dsi.header();
    nifti_image *nim = _nim->nim;

#if 0
    NiftiHeader hdr( thing.dimX(), thing.dimY(), thing.dimZ(), ntime,
                    thing.sizeX(), thing.sizeY(), thing.sizeZ(),
                    thing.sizeT(), name );

    const PythonHeader 
      *ph = dynamic_cast<const PythonHeader *>( thing.header() );
    if( ph )
    {
      hdr.copy( *ph );
      std::vector<int> dims(4);
      dims[0] = thing.dimX();
      dims[1] = thing.dimY();
      dims[2] = thing.dimZ();
      dims[3] = ntime;
      hdr.setProperty( "volume_dimension", dims );
      std::vector<float> vs(4);
      vs[0] = thing.sizeX();
      vs[1] = thing.sizeY();
      vs[2] = thing.sizeZ();
      vs[3] = thing.sizeT();
      hdr.setProperty( "voxel_size", vs );
      // remove specific attribute that may be incompatible
      if( hdr.hasProperty( "disk_data_type" ) )
        hdr.removeProperty( "disk_data_type" );
      if( hdr.hasProperty( "possible_data_types" ) )
        hdr.removeProperty( "possible_data_types" );
      hdr.setProperty( "data_type", code );
    }

    if( hdr.hasProperty( "scale_factor" ) )
      hdr.removeProperty( "scale_factor" );
    if( hdr.hasProperty( "scale_offset" ) )
      hdr.removeProperty( "scale_offset" );

    // Can float data be saved as integer without loss?
    if( code == "FLOAT" || code == "DOUBLE" )
    {
      bool forcedDT = false;
      try
      {
        if( !options.isNull() )
        {
          carto::Object aso = options->getProperty( "force_disk_data_type" );
          if( !aso.isNull() )
            forcedDT = (bool) aso->getScalar();
        }
      }
      catch( ... )
      {
      }
      if( !forcedDT )
      {
        // double maxm = 0;
        float scale = 1, offset = 0;
        bool shen = canEncodeAsScaledS16( *thing.volume(), scale, offset, true,
                                          0 /* &maxm */ );
        if( shen )
        {
          /* std::cout << "16 bit coding possible: scale: " << scale
              << ", offset: " << offset << ", max error: " << maxm << std::endl;
          */
          hdr.setProperty( "disk_data_type",
                          carto::DataTypeCode<int16_t>().dataType() );
          hdr.setProperty( "scale_factor_applied", true );
          hdr.setProperty( "scale_factor", scale );
          hdr.setProperty( "scale_offset", offset );
        }
        /* else
        std::cout << "matching interval not found. Not 16 bit codable\n"; */
      }
    }
#endif

    bool ok = true;
    carto::fdinhibitor fdi( 2 );
    fdi.close(); // disable output on stderr
    std::vector<long> strides(4);
    strides[0] = 1;
    strides[1] = sx;
    strides[2] = strides[1] * sy;
    strides[3] = strides[2] * sz;

    if( write4d || st == 1 )
    {
      // write Nifti header and data
      znzFile zfp = nifti_image_write_hdr_img( nim, 2, "wb" );
      // don't close file because we have no other way to know if it failed
      if( znz_isnull( zfp ) )
        ok = false;

      if( ok )
      {
        dataTOnim( nim, hdr, source, -1, zfp, strides );

        if( znz_isnull( zfp ) )
          ok = false;
        else
        {
          znzclose(zfp);
        }
      }

      // unload data in the nifti_image struct
      nifti_image_unload( nim );
      std::cout << "done.\n";
    }
#if 0
    else
    {
      char sequence[16];
      int f;
      std::vector<std::string> fnames;
      std::string dname, bname;
      dname = carto::FileUtil::dirname( name )
          + carto::FileUtil::separator();
      bname = carto::FileUtil::basename( removeExtension( name ) );
      fnames.reserve( vt );
      for( f=0; f<vt; ++f )
      {
        sprintf( sequence, "%04d", f + ot );
        fnames.push_back( bname + std::string( sequence ) + ext );
      }
//       hdr.setProperty( "series_filenames", fnames );
//       std::vector<int> vdim(4);
//       vdim[0] = sx;
//       vdim[1] = sy;
//       vdim[2] = sz;
//       vdim[3] = st;
//       hdr.setProperty( "volume_dimension", vdim );

      for( f=0; f < nt; ++f )
      {
//         hdr.setName( dname + fnames[f] );
//         hdr.fillNim( false ); // allow4D set to false
//         nifti_image *nim = hdr.niftiNim();

        znzFile zfp = nifti_image_write_hdr_img( nim, 2, "wb" );
        if( znz_isnull( zfp ) )
          ok = false;
        else
        {
//           dataTOnim( nim, hdr, thing, f, zfp);
          if( znz_isnull( zfp ) )
            ok = false;
          else
            znzclose( zfp );
        }
        nifti_image_unload( nim );
        if( !ok )
          break;
      }
    }
#endif
    fdi.open(); // enable stderr
    if( !ok )
      carto::io_error::launchErrnoExcept();
    std::cout << "write finished.\n";
  }

  template <typename T>
  DataSourceInfo Nifti1ImageWriter<T>::writeHeader( DataSourceInfo & dsi,
                                                    carto::Object options )
  {
    std::cout << "Nifti1ImageWriter.writeHeader\n";
    //--- build datasourcelist -----------------------------------------------
    bool dolist = dsi.list().typecount() == 1;
    if( dolist )
    {
      localMsg( "building DataSourceList..." );
      buildDSList( dsi.list(), options );
    }
    //--- set header ---------------------------------------------------------
    localMsg( "setting Header..." );
    if( options->hasProperty( "partial_writing" ) )
    {
      DataSourceInfoLoader  dsil;
      carto::rc_ptr<DataSource> hdrds = dsi.list().dataSource( "hdr" );
      if( hdrds.isNull() )
        hdrds = dsi.list().dataSource( "nii" );
      DataSourceInfo hdrdsi( hdrds );
      hdrdsi = dsil.check( hdrdsi );
      dsi.header()->setProperty( "sizeX",
                                 hdrdsi.header()->getProperty( "sizeX" ) );
      dsi.header()->setProperty( "sizeY",
                                 hdrdsi.header()->getProperty( "sizeY" ) );
      dsi.header()->setProperty( "sizeZ",
                                 hdrdsi.header()->getProperty( "sizeZ" ) );
      dsi.header()->setProperty( "sizeT",
                                 hdrdsi.header()->getProperty( "sizeT" ) );
      return dsi;
    }
    //--- write header -------------------------------------------------------
    localMsg( "writing Header..." );

    // TODO
    bool allow4d = true;
    fillNiftiHeader( dsi, options, allow4d );


    //--- write minf ---------------------------------------------------------
    localMsg( "writing Minf..." );
    carto::Object minf = dsi.header(); //carto::Object::value( carto::PropertySet() );
    minf->setProperty( "file_type", std::string( "NIFTI-1" ) );
    minf->setProperty( "data_type", carto::DataTypeCode<T>::dataType() );
    minf->setProperty( "object_type", std::string( "Volume" ) );
    std::vector<int> dims( 4, 0 );
    dsi.header()->getProperty( "sizeX", dims[ 0 ] );
    dsi.header()->getProperty( "sizeY", dims[ 1 ] );
    dsi.header()->getProperty( "sizeZ", dims[ 2 ] );
    dsi.header()->getProperty( "sizeT", dims[ 3 ] );
    minf->setProperty( "volume_dimension", dims );
    minf->setProperty( "voxel_size",
                       dsi.header()->getProperty( "voxel_size" ) );

    Writer<carto::GenericObject> minfw( dsi.list().dataSource( "minf" ) );
    minfw.write( *minf );
    //--- partial-io case ----------------------------------------------------
    if( options->hasProperty( "unallocated" ) )
    {
      localMsg( "building file for partial writing..." );
      if( _sizes.empty() )
        updateParams( dsi );


      // TODO

    }

    //------------------------------------------------------------------------
    localMsg( "done writing header." );
    return dsi;
  }

  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T>
  void Nifti1ImageWriter<T>::buildDSList( DataSourceList & dsl,
                                          carto::Object /*options*/ ) const
  {
    DataSource* pds = dsl.dataSource().get();
    std::string niiname, hdrname, minfname;

    niiname = pds->url();
    if( niiname.empty() )
    {
      throw carto::file_not_found_error( "needs a filename", niiname );
    }
    else
    {
      std::string ext = carto::FileUtil::extension( niiname );
      std::string basename = carto::FileUtil::removeExtension( niiname );

      if( ext == "gz" )
      {
        ext = ext = carto::FileUtil::extension( basename );
        basename = carto::FileUtil::removeExtension( basename );
        if( ext != "nii" )
          ext = "";
        else
          ext = "nii.gz";
      }
      if( ext == "nii" || ext == "nii.gz" || ext == "img" )
      {
        niiname = basename + "." + ext;
        if( ext == "img" )
          hdrname = basename + ".hdr";
      }
      else if( ext == "hdr" )
      {
        niiname = basename + ".img";
        hdrname = basename + ".hdr";
      }
      else
        ext.clear();

      if( ext.empty() )
      {
        basename = niiname;
        hdrname = "";
        niiname = basename + ".nii";
      }
      minfname = niiname + ".minf";

      if( hdrname == pds->url() )
      {
        // if hdrname is original url
        dsl.addDataSource( "hdr", carto::rc_ptr<DataSource>( pds ) );
      }
      else if( !hdrname.empty() )
      {
        dsl.addDataSource( "hdr", carto::rc_ptr<DataSource>
                                  ( new FileDataSource( hdrname ) ) );
      }
      if( niiname == pds->url() )
      {
        // if niiname is original url
        dsl.addDataSource( "nii", carto::rc_ptr<DataSource>( pds ) );
      }
      else
      {
        dsl.addDataSource( "nii", carto::rc_ptr<DataSource>
                                  ( new FileDataSource( niiname ) ) );
      }
    }

    dsl.addDataSource( "minf", carto::rc_ptr<DataSource>
                               ( new FileDataSource( minfname ) ) );

  }


  template <typename T>
  void Nifti1ImageWriter<T>::fillNiftiHeader( DataSourceInfo & dsi,
                                              carto::Object options, 
                                              bool allow4d )
  {
    /* initialization of a nifti image struct */
    nifti_image *nim = nifti_simple_init_nim();

    Object hdr = dsi.header();

    // cf nifti_convert_nim2nhdr() to know which fields need to be filled 

    /***********************/
    /* DATA DIMENSIONALITY */
    /***********************/

    /* Dimensions of grid array */
    std::vector<int> dims(4, 1);
    hdr->getProperty( "sizeX", dims[0] );
    hdr->getProperty( "sizeY", dims[1] );
    hdr->getProperty( "sizeZ", dims[2] );
    hdr->getProperty( "sizeT", dims[3] );

    if( dims[3] == 1 )
    {
      dims.erase( dims.begin() + 3 );
      if( dims[2] == 1 )
      {
        dims.erase( dims.begin() + 2 );
        if( dims[1] == 1 )
          dims.erase( dims.begin() + 1 );
      }
    }

    AffineTransformation3d s2m;
    try
    {
      Object storage_to_memory;
      storage_to_memory = hdr->getProperty( "storage_to_memory" );
      s2m = storage_to_memory;
    }
    catch( ... )
    {
      // cout << "Saving in neurological orientation: ";
      /* TODO */ // Should use 'spm_force_output_convention' and 'spm_output_radio_convention'
      /* TODO */ // and also see if 'spm_radio_convention' is present
      s2m.matrix(0,3) = dims[0] - 1;
      s2m.matrix(1,3) = dims[1] - 1;
      s2m.matrix(2,3) = dims[2] - 1;
      s2m.matrix(0,0) = -1.0;
      s2m.matrix(1,1) = -1.0;
      s2m.matrix(2,2) = -1.0;
    }

    AffineTransformation3d m2s = s2m.inverse();
    Point3df df = m2s.transform( Point3df( dims[0], dims[1], dims[2] ) )
        - m2s.transform( Point3df( 0, 0, 0 ) );
    std::vector<int> tdims(4);
    tdims[0] = short( rint( fabs( df[0] ) ) );
    tdims[1] = short( rint( fabs( df[1] ) ) );
    tdims[2] = short( rint( fabs( df[2] ) ) );
    tdims[3] = dims[3];
    // fix s2m dims if needed (happens in case image dims have changed)
    s2m.matrix(0,3) = 0; // erase initial translation
    s2m.matrix(1,3) = 0;
    s2m.matrix(2,3) = 0;
    Point3df p = s2m.transform( Point3df( tdims[0], tdims[1], tdims[2] ) ),
      pp( 0. );
    if( p[0] < 0 )
      pp[0] = -p[0] - 1;
    if( p[1] < 0 )
      pp[1] = -p[1] - 1;
    if( p[2] < 0 )
      pp[2] = -p[2] - 1;
    s2m.matrix(0,3) = pp[0];
    s2m.matrix(1,3) = pp[1];
    s2m.matrix(2,3) = pp[2];

    nim->ndim = nim->dim[0] = dims.size();
    nim->dim[1] = tdims[0];
    nim->dim[2] = tdims[1];
    nim->dim[3] = tdims[2];
    nim->dim[4] = tdims[3];
    int i;
    for( i=dims.size()+1; i<8; ++i )
      nim->dim[i] = 1;
    nim->nx = nim->dim[1];
    nim->ny = nim->dim[2];
    nim->nz = nim->dim[3];
    nim->nt = nim->dim[4];
    nim->nu = nim->dim[5];
    nim->nv = nim->dim[6];
    nim->nw = nim->dim[7];
    if(nim->dim[dims.size()] == 1)
      nim->ndim = nim->dim[0] -= 1;

    if (!allow4d)
    {
      nim->nt = nim->nu = nim->nv = nim->nw = 1;
      nim->dim[4] = nim->dim[5] = nim->dim[6] = nim->dim[7] = 1;
      nim->ndim = nim->dim[0] = (dims.size() >= 4)? 3 : dims.size();
    }

    nim->nvox =  nim->nx * nim->ny * nim->nz
              * nim->nt * nim->nu * nim->nv * nim->nw ;

    /* Grid spacings */
    std::vector<float> vs;
    hdr->getProperty( "voxel_size", vs );
    for( i=vs.size(); i<4; ++i )
      vs.push_back( 1. );
    df = m2s.transform( Point3df( vs[0], vs[1], vs[2] ) )
        - m2s.transform( Point3df( 0, 0, 0 ) );
    std::vector<float> tvs(4);
    tvs[0] = fabs( df[0] );
    tvs[1] = fabs( df[1] );
    tvs[2] = fabs( df[2] );
    tvs[3] = vs[3];

    nim->pixdim[0] = 0;
    nim->pixdim[1] = tvs[0];
    nim->pixdim[2] = tvs[1];
    nim->pixdim[3] = tvs[2];
    nim->pixdim[4] = tvs[3];
    for( i=vs.size()+1; i<8; ++i )
      nim->pixdim[i] = 1.0;
    nim->dx = nim->pixdim[1];
    nim->dy = nim->pixdim[2];
    nim->dz = nim->pixdim[3];
    nim->dt = nim->pixdim[4];
    nim->du = nim->pixdim[5];
    nim->dv = nim->pixdim[6];
    nim->dw = nim->pixdim[7];

    if (!allow4d)
    {
      nim->dt = nim->du = nim->dv = nim->dw = 1.0;
      nim->pixdim[4] = nim->pixdim[5] = nim->pixdim[6] = nim->pixdim[7] = 1.0;
    }

    /****************/
    /* TYPE OF DATA */
    /****************/

    /* Type of data in voxels: DT_* code */
    std::string type;
    if( !hdr->getProperty( "disk_data_type", type ) )
      type = carto::DataTypeCode<T>::dataType();

    if( type == "U8" )
      nim->datatype = DT_UINT8;
    else if( type == "S16" )
      nim->datatype = DT_INT16;
    else if( type == "S32" )
      nim->datatype = DT_INT32;
    else if( type == "FLOAT" )
      nim->datatype = DT_FLOAT32;
    else if( type == "DOUBLE" )
      nim->datatype = DT_DOUBLE;
    else if( type == "S8" )
      nim->datatype = DT_INT8;
    else if( type == "U16" )
      nim->datatype = DT_UINT16;
    else if( type == "U32" )
      nim->datatype = DT_UINT32;
    else if( type == "RGB" )
      nim->datatype = DT_RGB;
    else if( type == "RGBA" )
      nim->datatype = DT_RGBA32;
    else
    {
      std::cerr << "Unknown NIFTI1 datatype, using FLOAT: " << type
        << std::endl;
      nim->datatype = DT_FLOAT32;
    }

    /* set nbyper,swapsize from datatype */
    nifti_datatype_sizes( nim->datatype , &nim->nbyper, &nim->swapsize );

    /****************/
    /* DATA SCALING */
    /****************/

    /* Scaling parameter: slope, intercept */
    float scl_slope = 0.0;
    if( hdr->getProperty( "scale_factor", scl_slope ) )
      nim->scl_slope = scl_slope;
    else
      nim->scl_slope = 0.0;

    float scl_inter = 0.0;
    if( hdr->getProperty( "scale_offset", scl_inter ) )
      nim->scl_inter = scl_inter;
    else
      nim->scl_inter = 0.0;

    /* Calibration parameter: min, max */
    float cal_min = 0.0;
    if( hdr->getProperty( "cal_min", cal_min ) )
      nim->cal_min = cal_min;
    else
      nim->cal_min = 0.0;

    float cal_max = 0.0;
    if( hdr->getProperty( "cal_max", cal_max ) )
      nim->cal_max = cal_max;
    else
      nim->cal_max = 0.0;

    /*************************************************/
    /* MRI-SPECIFIC SPATIAL AND TEMPORAL INFORMATION */
    /*************************************************/

    int freq_dim = 0;
    if( hdr->getProperty( "freq_dim", freq_dim ) )
      nim->freq_dim = freq_dim;
    else
      nim->freq_dim = 0;

    int phase_dim = 0;
    if( hdr->getProperty( "phase_dim", phase_dim ) )
      nim->phase_dim = phase_dim;
    else
      nim->phase_dim = 0;

    int slice_dim = 0;
    if( hdr->getProperty( "slice_dim", slice_dim ) )
      nim->slice_dim = slice_dim;
    else
      nim->slice_dim = 0; /* TODO */ // should be nim->nz ?

    int slice_code = 0;
    if( hdr->getProperty( "slice_code", slice_code ) )
      nim->slice_code = slice_code;
    else
      nim->slice_code = 0;

    int slice_start = 0;
    if( hdr->getProperty( "slice_start", slice_start ) )
      nim->slice_start = slice_start;
    else
      nim->slice_start = 0;

    int slice_end = 0;
    if( hdr->getProperty( "slice_end", slice_end ) )
      nim->slice_end = slice_end;
    else
      nim->slice_end = 0;

    float slice_duration = 0.0;
    if( hdr->getProperty( "slice_duration", slice_duration ) )
      nim->slice_duration = slice_duration;
    else
      nim->slice_duration = 0.0; /* TODO */ // should be tr?

    /**********************************************/
    /* 3D IMAGE ORIENTATION AND LOCATION IN SPACE */
    /**********************************************/

    carto::Object referentials;
    carto::Object transformations;

    try
    {
      referentials = hdr->getProperty( "referentials" );
      transformations = hdr->getProperty( "transformations" );
    }
    catch( ... )
    {
    }

    if( transformations.isNull() )
    {
      // cout << "no transformations\n";
      transformations = Object::value( std::vector<std::vector<float> >() );
      referentials = Object::value( std::vector<std::string>() );
    }

    // voxel size transformation in memory orientation
    AffineTransformation3d voxsz;
    voxsz.matrix(0,0) = vs[0];
    voxsz.matrix(1,1) = vs[1];
    voxsz.matrix(2,2) = vs[2];

    nim->qform_code = NIFTI_XFORM_UNKNOWN;
    nim->sform_code = NIFTI_XFORM_UNKNOWN;
    // cout << "transformations: " << transformations->size() << endl;
    unsigned nt = transformations->size();
    std::vector<AffineTransformation3d> mv( nt );
    if( nt > 0 )
      mv[0] = AffineTransformation3d( transformations->getArrayItem(0) );
    unsigned j;
    bool skip;
    carto::Object transformations2 = carto::Object::value( 
      std::vector<std::vector<float> >() );
    std::vector<std::vector<float> > & t2
        = transformations2->value<std::vector<std::vector<float> > >();
    carto::Object referentials2 = carto::Object::value(
      std::vector<std::string>() );
    std::vector<std::string> & r2
      = referentials2->value<std::vector<std::string> >();

    // s2m orientation codes
    std::vector<float> mvec = s2m.toVector();
    mat44 S2M_m44;
    for( i=0; i<4; ++i )
      for( int j=0; j<4; ++j )
        S2M_m44.m[i][j] = mvec[j+4*i];
    int icod, jcod, kcod;
    nifti_mat44_to_orientation( S2M_m44, &icod, &jcod, &kcod );

    bool s2moriented = false, s2morientedbis;

    for( i=0; i<nt; ++i )
    {
      mv[i] = AffineTransformation3d( transformations->getArrayItem(i) );
      skip = false;
      for( j=0; j<i; ++j )
        if( mv[j] == mv[i] )
        {
          /* cout << "duplicate transformation " << i << " identical to " << j
              << endl; */
          skip = true;
          break;
        }
      if( !skip )
      {
        AffineTransformation3d mot( mv[i] );
        std::string ref = referentials->getArrayItem(i)->getString();
        // cout << "store transformation " << i << ":\n";
        /* cout << mot << endl;
        cout << mot * voxsz * s2m << endl; */
        r2.push_back( ref );
        t2.push_back( mot.toVector() );

        std::vector<float> m = (mot * voxsz * s2m).toVector();
        mat44 R;

        s2morientedbis = false;
        if( !s2moriented )
        {
          // check if it can also store s2m
          AffineTransformation3d mi;
          mi.matrix( 0, 0 ) = -1;
          mi.matrix( 1, 1 ) = -1;
          mi.matrix( 2, 2 ) = -1;
          std::vector<float> m = (mot * voxsz * s2m * mi).toVector();
          for( int x=0; x<4; ++x )
            for( int j=0; j<4; ++j )
              R.m[x][j] = m[j+4*x];
          int icod2, jcod2, kcod2;
          nifti_mat44_to_orientation( R, &icod2, &jcod2, &kcod2 );
          s2morientedbis = ( icod == icod2 && jcod == jcod2 && kcod == kcod2 );
          // cout << "s2moriented: " << s2morientedbis << endl;
        }

        for( int x=0; x<4; ++x )
          for( int j=0; j<4; ++j )
            R.m[x][j] = m[j+4*x];

        bool ok = false;
        int xform_code = NiftiReferential( ref );
        if( ( ( xform_code == NIFTI_XFORM_SCANNER_ANAT )
              /* || (xform_code == NIFTI_XFORM_ALIGNED_ANAT) */ )
              && (nim->qform_code == NIFTI_XFORM_UNKNOWN ) && s2morientedbis )
        {
          // cout << "save transformation " << i << " as qform\n";
          nim->qform_code = xform_code;
          nifti_mat44_to_quatern( R, &nim->quatern_b, &nim->quatern_c,
                                  &nim->quatern_d, &nim->qoffset_x,
                                  &nim->qoffset_y, &nim->qoffset_z,
                                  NULL, NULL, NULL, &nim->qfac );
          // check if the matrix can actually be stored as a quaternion
          mat44 P = nifti_quatern_to_mat44( nim->quatern_b, nim->quatern_c,
                                            nim->quatern_d, nim->qoffset_x,
                                            nim->qoffset_y, nim->qoffset_z,
                                            tvs[0], tvs[1], tvs[2], nim->qfac
                                          );
          ok = true;
          s2moriented = true;
          for( int x=0; x<3 && ok; ++x )
            for( int j=0; j<3; ++j )
              if( fabs( P.m[x][j] - R.m[x][j] ) > 1e-5 )
              {
                // cancel
                // cout << "transformation does not fit as a quaternion" << endl;
                ok = false;
                nim->qform_code = NIFTI_XFORM_UNKNOWN;
                break;
              }
        }
        if( !ok && nim->sform_code == NIFTI_XFORM_UNKNOWN )
        {
          // cout << "save transformation " << i << " as sform\n";
          ok = true;
          nim->sform_code = xform_code;
          for( int x=0; x<4; ++x )
            for( int j=0; j<4; ++j )
              nim->sto_xyz.m[x][j] = m[j+x*4];
        }
        if( !ok )
        {
          std::cout << "Could not save transformation." << std::endl;
        }
      }
    }

    if( !s2moriented && nim->qform_code == NIFTI_XFORM_UNKNOWN )
    {
      /* cout << "add s2m\n";
      cout << s2m << endl; */
      AffineTransformation3d NIs2m_aims;
      NIs2m_aims.matrix( 0, 0 ) = -1; // invert all axes
      NIs2m_aims.matrix( 1, 1 ) = -1;
      NIs2m_aims.matrix( 2, 2 ) = -1;
      AffineTransformation3d NIs2m = s2m * NIs2m_aims;
      /* if( NIs2m.rotation()(0,0) == 1 && NIs2m.rotation()(1,1) == 1
        && NIs2m.rotation()(2,2) == 1)
      {
        cout << "default orientation\n";
      } */
      mvec = NIs2m.toVector();
      mat44 P;
      for( int x=0; x<4; ++x )
        for( int j=0; j<4; ++j )
          P.m[x][j] = mvec[j+4*x];
      // cout << "matrix: " << NIs2m << endl;
      nim->qform_code = NIFTI_XFORM_SCANNER_ANAT;
      nifti_mat44_to_quatern( P, &nim->quatern_b, &nim->quatern_c,
                              &nim->quatern_d, &nim->qoffset_x,
                              &nim->qoffset_y, &nim->qoffset_z,
                              NULL, NULL, NULL, &nim->qfac );
      /* TODO */ // use 'origin' field
      nim->qoffset_x = -(nim->nx / 2.0 - 1.0) * nim->dx;
      nim->qoffset_y = -(nim->ny / 2.0) * nim->dy;
      nim->qoffset_z = -(nim->nz / 2.0) * nim->dz;

      NIs2m_aims.matrix(0, 3) = dims[0] * vs[0] / 2;
      NIs2m_aims.matrix(1, 3) = ( dims[1] - 2 ) * vs[1] / 2;
      NIs2m_aims.matrix(2, 3) = ( dims[2] - 2 ) * vs[2] / 2;
      t2.insert( t2.begin(), NIs2m_aims.toVector() );
      r2.insert( r2.begin(), NiftiReferential( NIFTI_XFORM_SCANNER_ANAT ) );
    }

    if( !t2.empty() )
    {
      hdr->setProperty( "referentials", referentials2 );
      hdr->setProperty( "transformations", transformations2 );
    }
    hdr->setProperty( "storage_to_memory", s2m.toVector() );

    /********************************************/
    /* UNITS OF SPATIAL AND TEMPORAL DIMENSIONS */
    /********************************************/

    float toffset = 0.0;
    if( hdr->getProperty( "toffset", toffset ) )
      nim->toffset = toffset;
    else
      nim->toffset = 0.0;

    int xyz_units = 0;
    if( hdr->getProperty( "xyz_units", xyz_units ) )
      nim->xyz_units = xyz_units;
    else
      nim->xyz_units = 0;

    int time_units = 0;
    if( hdr->getProperty( "time_units", time_units ) )
      nim->time_units = time_units;
    else
      nim->time_units = 0;

    /********************************/
    /* INTERPRETATION OF VOXEL DATA */
    /********************************/

    int intent_code = NIFTI_INTENT_NONE;
    if( hdr->getProperty( "intent_code", intent_code ) )
      nim->intent_code = intent_code;
    else
      nim->intent_code = NIFTI_INTENT_NONE;

    float intent_p1 = 0.0;
    if( hdr->getProperty( "intent_p1", intent_p1 ) )
      nim->intent_p1 = intent_p1;
    else
      nim->intent_p1 = 0.0;

    float intent_p2 = 0.0;
    if( hdr->getProperty( "intent_p2", intent_p2 ) )
      nim->intent_p2 = intent_p2;
    else
      nim->intent_p2 = 0.0;

    float intent_p3 = 0.0;
    if( hdr->getProperty( "intent_p3", intent_p3 ) )
      nim->intent_p3 = intent_p3;
    else
      nim->intent_p3 = 0.0;

    std::string intent_name;
    if( hdr->getProperty( "intent_name", intent_name ) )
    {
      nim->intent_name[15] = '\0';
      strncpy(nim->intent_name, intent_name.c_str(), 15);
    }
    else
      nim->intent_name[0] = '\0';

    /**********************************/
    /* DESCRIPTION AND AUXILIARY FILE */
    /**********************************/

    strcpy( nim->descrip, "NIFTI-1 (writer: Soma IO 4.5).\0" );
    std::string descrip;
    if( hdr->getProperty( "descrip", descrip ) )
    {
      nim->descrip[79] = '\0';
      strncpy( nim->descrip, descrip.c_str(), 79 );
    }
    else
      nim->descrip[0] = '\0';

    std::string aux_file;
    if( hdr->getProperty( "aux_file", aux_file ) )
    {
      nim->aux_file[23] = '\0';
      strncpy( nim->aux_file, aux_file.c_str(), 23 );
    }
    else
      nim->aux_file[0] = '\0';

    /*********************/
    /* HEADER EXTENSIONS */
    /*********************/

    std::vector<int> extcode;
    if( hdr->getProperty( "extcode", extcode ) )
    {
      int next = extcode.size();
      std::vector<std::vector<char> > extdata( next );
      hdr->getProperty( "extdata", extdata );
      if( extdata.size() < next )
        next = extdata.size();
      for( int i=0; i<next; ++i )
      {
        nifti_add_extension( nim, &extdata[i][0], extdata[i].size(),
                             extcode[i] );
      }
    }

    /* Set Nifti type from given extension (default is Nifti 1 file) */
    carto::rc_ptr<DataSource> mainfile = dsi.list().dataSource( "nii" );
    std::string Extension = FileUtil::extension( mainfile->url() );
    if( (Extension == "hdr") || (Extension == "gz") ||
      (Extension == "img") )
      nim->nifti_type = NIFTI_FTYPE_NIFTI1_2;
    else
      nim->nifti_type = NIFTI_FTYPE_NIFTI1_1;
    /* set fname, iname, byteorder from filenames or nifti_type */
    nifti_set_filenames( nim, mainfile->url().c_str(), 0, 1 );
    /* set nifti_type from  */
    //nifti_set_type_from_names( nim );
    /* set iname_offset from nifti_type */
    //nifti_set_iname_offset( nim );


    // store nifti structure in DataSourceInfo

    carto::rc_ptr<Nifti1StructWrapper> nimw( new Nifti1StructWrapper( nim ) );
    dsi.privateIOData()->setProperty( "nifti_structure", nimw );
  }

}

#undef localMsg
#endif
