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

// It is not obvious how to fix these warnings, make them non-fatal for now
#pragma GCC diagnostic warning "-Wsign-compare"

#ifndef SOMAIO_IMAGE_NIFTIIMAGEWRITER_D_H
#define SOMAIO_IMAGE_NIFTIIMAGEWRITER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/niftiimagewriter.h>               // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                             // heritage
#include <soma-io/io/writer.h>                                // to write minf
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasourceinfo/datasourceinfoloader.h>    // partial writing
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/streamdatasource.h>      // used by writeHeader()
#include <soma-io/datasource/datasource.h>
#include <soma-io/nifticlib/niftiapihelpers_p.h>
#include <soma-io/checker/niftiformatchecker.h>
#include <soma-io/checker/transformation.h>
#include <soma-io/io/scaledcoding.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/type/types.h>                           // to write header
#include <cartobase/stream/fileutil.h>                          // utilitaires
#include <cartobase/stream/fdinhibitor.h>
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
#include <cartobase/config/version.h>
#include <cartobase/containers/nditerator.h>
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
  void NiftiImageWriter<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    if( !dsi.header()->getProperty( "volume_dimension", _sizes[ 0 ] ) )
    {
      dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
      dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
      dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
      dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    }

    dsi.privateIOData()->getProperty( "nifti_structure", _nim );
  }

  template <typename T>
  void NiftiImageWriter<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
    _nim.reset( 0 );
    _znzfiles.clear();
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
                               const T* data,
                               znzFile zfp, 
                               const std::vector<long> & strides,
                               const std::vector<int> & vsz,
                               const std::vector<int> & opos )
  {
    bool scalef;
    std::vector<float> s(2);

    if( hdr->getProperty( "scale_factor_applied", scalef ) && scalef
        && hdr->getProperty( "scale_factor", s[0] )
        && hdr->getProperty( "scale_offset", s[1] ) )
    {
      soma::Point3df d0f;
      size_t dim, ndim = vsz.size();
      if( ndim > 7 )
        ndim = 7; // NIFTI is limited to 7D
      std::vector<int> d0( ndim, 0  );
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

      // region line size
      long offset, cur_offset = 0, offset2;
      std::vector<long> dstrides( ndim, 0 );
      dstrides[0] = sizeof(int16_t);
      for( dim=0; dim<ndim - 1; ++dim )
        dstrides[ dim + 1 ] = nim->dim[dim] * dstrides[dim];

      size_t vx = vsz[0];
      size_t numbytes = vx * sizeof( int16_t ), ss;
      std::vector<int16_t> buf( vx );
      int16_t *d = 0;

      carto::line_NDIterator_base it( vsz, strides );
      for( ; !it.ended(); ++it )
      {
        const std::vector<int> & volpos = it.position();
        d0 = volpos;
        d0f = m.transform( soma::Point3df( 0, volpos[1], volpos[2] ) );
        d0[0] = int( rint( d0f[0] ) );
        d0[1] = int( rint( d0f[1] ) );
        d0[2] = int( rint( d0f[2] ) );
        d = &buf[0];
        p0 = data;
        offset = 0;
        for( dim=0; dim<ndim; ++dim )
        {
          // memory pointer/offset
          p0 += d0[dim] * strides[dim];
          // disk offset
          offset += ( volpos[dim]  + opos[dim] ) * dstrides[dim];
        }
        for( int x=0; x<vx; ++x, p0 += pinc )
          *d++ = (int16_t) rint( (*p0 - s[1]) / s[0] );

        offset2 = offset;
        offset -= cur_offset;
        cur_offset = offset2 + numbytes;
        znzseek( zfp, offset, SEEK_CUR );
        // write at specified location
        ss = znzwrite( (void*) &buf[0] , 1 , numbytes , zfp );
        if( ss != numbytes )
          carto::io_error::launchErrnoExcept();
      }
      return true;
    }
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const carto::VoxelRGB*,
                               znzFile,
                               const std::vector<long> &,
                               const std::vector<int> &,
                               const std::vector<int> & )
  {
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const carto::VoxelRGBA*,
                               znzFile,
                               const std::vector<long> &,
                               const std::vector<int> &,
                               const std::vector<int> & )
  {
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const carto::VoxelHSV*,
                               znzFile,
                               const std::vector<long> &,
                               const std::vector<int> &,
                               const std::vector<int> & )
  {
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const cfloat*,
                               znzFile,
                               const std::vector<long> &,
                               const std::vector<int> &,
                               const std::vector<int> & )
  {
    return false;
  }


  template <>
  bool expandNiftiScaleFactor( const carto::Object &, nifti_image *,
                               const soma::AffineTransformation3d &,
                               const cdouble*,
                               znzFile,
                               const std::vector<long> &,
                               const std::vector<int> &,
                               const std::vector<int> & )
  {
    return false;
  }


  template <typename T>
  void writeWithoutScaleFactor( const carto::Object & hdr,
                                nifti_image *nim,
                                const soma::AffineTransformation3d & m,
                                const T* data,
                                znzFile zfp,
                                const std::vector<long> & strides,
                                const std::vector<int> & vsz,
                                const std::vector<int> & opos )
  {
    soma::Point3df d0f;
    size_t dim, ndim = vsz.size();
    if( ndim > 7 )
      ndim = 7; // NIFTI is limited to 7D
    std::vector<int> d0( ndim, 0  );
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

    // region line size
    long offset, cur_offset = 0, offset2;
    std::vector<long> dstrides( ndim, 0 );
    dstrides[0] = sizeof(T);
    for( dim=0; dim<ndim - 1; ++dim )
      dstrides[ dim + 1 ] = nim->dim[dim + 1] * dstrides[dim];

    size_t vx = vsz[0];
    size_t numbytes = vx * sizeof( T ), ss;
    std::vector<T> buf( vx );
    T *d = 0;

    carto::line_NDIterator_base it( vsz, strides );
    for( ; !it.ended(); ++it )
    {
      const std::vector<int> & volpos = it.position();
      d0 = volpos;
      d0f = m.transform( soma::Point3df( 0, volpos[1], volpos[2] ) );
      d0[0] = int( rint( d0f[0] ) );
      d0[1] = int( rint( d0f[1] ) );
      d0[2] = int( rint( d0f[2] ) );
      d = &buf[0];
      p0 = data;
      offset = 0;
      for( dim=0; dim<ndim; ++dim )
      {
        // memory pointer/offset
        p0 += d0[dim] * strides[dim];
        // disk offset
        offset += ( volpos[dim]  + opos[dim] ) * dstrides[dim];
      }
      for( int x=0; x<vx; ++x, p0 += pinc )
        *d++ = *p0;

      offset2 = offset;
      offset -= cur_offset;
      cur_offset = offset2 + numbytes;
      znzseek( zfp, offset, SEEK_CUR );
      // write at specified location
      ss = znzwrite( (void*) &buf[0] , 1 , numbytes , zfp );
      if( ss != numbytes )
        carto::io_error::launchErrnoExcept();
    }
  }


  template <typename T>
  void dataTOnim( nifti_image *nim, carto::Object & hdr,
                  const T* source, int tt, znzFile zfp, 
                  const std::vector<long> & strides,
                  const std::vector<int> & pos,
                  const std::vector<int> & size,
                  const std::vector<std::vector<int> > & sizes )
  {
    soma::AffineTransformation3d m, m2s, mems2m;
    soma::Point3df p, p0( 0, 0, 0 ), tp;

    try
    {
      carto::Object storage_to_memory;
      storage_to_memory = hdr->getProperty( "storage_to_memory" );
      m = storage_to_memory;
      /* adjust translations so that they fit (in case the vol size has
         changed) */
      p = soma::Point3df( nim->nx - 1, 0, 0 );
      tp = m.transform( soma::Point3df( 1, 0, 0 ) ) - m.transform( p0 );
      dataTOnim_checks2m( m, p, tp, 0, 0 );
      dataTOnim_checks2m( m, p, tp, 0, 1 );
      dataTOnim_checks2m( m, p, tp, 0, 2 );
      p = soma::Point3df( 0, nim->ny - 1, 0 );
      tp = m.transform( soma::Point3df( 0, 1, 0 ) ) - m.transform( p0 );
      dataTOnim_checks2m( m, p, tp, 1, 0 );
      dataTOnim_checks2m( m, p, tp, 1, 1 );
      dataTOnim_checks2m( m, p, tp, 1, 2 );
      p = soma::Point3df( 0, 0, nim->nz - 1 );
      tp = m.transform( soma::Point3df( 0, 0, 1 ) ) - m.transform( p0 );
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

    m2s = m.inverse();

    // total volume size, in disk orientation
//     soma::Point3df vtsize = m2s.transform(
//       soma::Point3df( sizes[0][0], sizes[0][1], sizes[0][2] ) )
//       - m2s.transform( p0 );
//     int  sx = int( rint( fabs( vtsize[ 0 ] ) ) );
//     int  sy = int( rint( fabs( vtsize[ 1 ] ) ) );
//     int  sz = int( rint( fabs( vtsize[ 2 ] ) ) );
//     int  st = sizes[ 0 ][ 3 ];

    // region size, in disk orientation
    soma::Point3df vsize = m2s.transform(
      soma::Point3df( size[0], size[1], size[2] ) )
      - m2s.transform( p0 );
    std::vector<int> tr_size = size;
    tr_size[0] = int( rint( fabs( vsize[ 0 ] ) ) );
    tr_size[1] = int( rint( fabs( vsize[ 1 ] ) ) );
    tr_size[2] = int( rint( fabs( vsize[ 2 ] ) ) );

    // region position, in disk orientation
    soma::Point3df orig = m2s.transform(
      soma::Point3df( pos[0], pos[1], pos[2] ) );
    soma::Point3df pend = m2s.transform(
      soma::Point3df( pos[0] + size[0] - 1, pos[1] + size[1] - 1,
                      pos[2] + size[2] - 1 ) );
    std::vector<int> tr_pos = pos;
    if( tr_pos.size() < tr_size.size() )
      tr_pos.resize( tr_size.size(), 0 );
    tr_pos[0] = std::min( int( rint( fabs( orig[ 0 ] ) ) ),
                          int( rint( fabs( pend[ 0 ] ) ) ) );
    tr_pos[1] = std::min( int( rint( fabs( orig[ 1 ] ) ) ),
                          int( rint( fabs( pend[ 1 ] ) ) ) );
    tr_pos[2] = std::min( int( rint( fabs( orig[ 2 ] ) ) ),
                          int( rint( fabs( pend[ 2 ] ) ) ) );
    // TODO: still need a s2m matrix for the memory object
    // memory volume-size s2m
    mems2m = m;
    p = soma::Point3df( tr_size[0] - 1, 0, 0 );
    tp = mems2m.transform( soma::Point3df( 1, 0, 0 ) ) - mems2m.transform( p0 );
    dataTOnim_checks2m( mems2m, p, tp, 0, 0 );
    dataTOnim_checks2m( mems2m, p, tp, 0, 1 );
    dataTOnim_checks2m( mems2m, p, tp, 0, 2 );
    p = soma::Point3df( 0, tr_size[1] - 1, 0 );
    tp = mems2m.transform( soma::Point3df( 0, 1, 0 ) )
      - mems2m.transform( p0 );
    dataTOnim_checks2m( mems2m, p, tp, 1, 0 );
    dataTOnim_checks2m( mems2m, p, tp, 1, 1 );
    dataTOnim_checks2m( mems2m, p, tp, 1, 2 );
    p = soma::Point3df( 0, 0, tr_size[2] - 1 );
    tp = mems2m.transform( soma::Point3df( 0, 0, 1 ) )
      - mems2m.transform( p0 );
    dataTOnim_checks2m( mems2m, p, tp, 2, 0 );
    dataTOnim_checks2m( mems2m, p, tp, 2, 1 );
    dataTOnim_checks2m( mems2m, p, tp, 2, 2 );

    size_t dim, ndim = sizes[0].size();
    if( ndim > 7 )
    {
      ndim = 7; // NIFTI is limited to 7D
      tr_size.resize( ndim );
      tr_pos.resize( ndim );
    }

    int tmin, tmax;
    if( tt >= 0 )
    {
      for( dim=4; dim<ndim; ++dim )
        tr_size[dim] = 1;
      tr_pos[3] =  tt;
    }

    if( !source ||
        !expandNiftiScaleFactor( hdr, nim, mems2m, source,
          zfp, strides, tr_size, tr_pos ) )
      writeWithoutScaleFactor( hdr, nim, mems2m, source,
        zfp, strides, tr_size, tr_pos );
  }

} // namespace {}


namespace soma
{
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  NiftiImageWriter<T>::NiftiImageWriter() :
    ImageWriter<T>(), api( 0 )
  {
  }

  template <typename T>
  NiftiImageWriter<T>::~NiftiImageWriter()
  {
    delete api;
  }

  //==========================================================================
  //   I M A G E W R I T E R   M E T H O D S
  //==========================================================================
  template <typename T>
  znzFile NiftiImageWriter<T>::writeNiftiHeader( carto::Object options ) const
  {
    nifti_image *nim = _nim->nim;
    // check file extension
    std::string fname = nim->fname;
    std::string::size_type len = fname.length();
    if( ( len >= 4
          && ( fname.substr( len - 4, 4 ) == ".nii"
            || fname.substr( len - 4, 4 ) == ".hdr" ) )
        || ( len >= 7 && ( fname.substr( len-7, 7 ) == ".nii.gz"
          || fname.substr( len - 7, 7 ) == ".hdr.gz" ) ) )
      // regular extension: use the nifticlib function
      return api->nifti_image_write_hdr_img( _nim->nim, 2, "wb" );

    // non-standard extension check options
    bool compressed = false, override_extension = false;
    try
    {
      carto::Object oext = options->getProperty( "override_extension" );
      override_extension = bool( oext->getScalar() );
    }
    catch( ... )
    {
    }
    if( !override_extension )
      // don't force ext: let nifticlib manage it
      return api->nifti_image_write_hdr_img( _nim->nim, 2, "wb" );

    try
    {
      carto::Object ocompressed = options->getProperty( "compressed" );
      compressed = bool( ocompressed->getScalar() );
    }
    catch( ... )
    {
    }
    znzFile zfp;
    if( !compressed && len >= 3 && fname.substr( len - 3, 3 ) == ".gz" )
      // extension is .gz: force compression
      compressed = true;
    zfp = znzopen( nim->fname , "wb", compressed );
    // use lower_level nifti function
    nifti_image_write_hdr_img2( nim, 2, "wb", zfp, 0 );
    return zfp;
  }


  template <typename T>
  void NiftiImageWriter<T>::write( const T * source, DataSourceInfo & dsi,
                                    const std::vector<int> & pos,
                                    const std::vector<int> & size,
                                    const std::vector<long> & strides,
                                    carto::Object options )
  {
    // std::cout << "NiftiImageWriter<T>::write\n";

    if( _sizes.empty() || _nim.isNull() )
      updateParams( dsi );

    unsigned  st = static_cast<unsigned>( _sizes[ 0 ][ 3 ] );
    unsigned  vt = static_cast<unsigned>( size[ 3 ] );
    unsigned  ot = static_cast<unsigned>( pos[ 3 ] );

    bool  write4d = dsi.list().size( "nii" ) == 1;

    if( !checkDimsCompatibility() )
      throw carto::invalid_format_error( formatName()
        + " cannot handle volume "
        "dimensions exceeding 32737", dsi.url() );

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

    nifti_image *nim = _nim->nim;

    bool ok = true;
    carto::fdinhibitor fdi( 2 );
    fdi.close(); // disable output on stderr

    if( write4d || st == 1 )
    {
      // write Nifti data
      znzFile zfp;

      if( _znzfiles.size() != 0 )
        zfp = _znzfiles[0]->znzfile;
      else
      {
        if( partial )
        {
          // partial writing needs:
          // opening the "img" stream read-write and position correctly in it
          //   (skip the header, if any)
          // seek the correct location, in disk space orientation, before
          //   writing each line
          zfp = znzopen( dsi.url().c_str(), "r+b",
                         nifti_is_gzfile( dsi.url().c_str() ) );
          znzseek( zfp, _nim->nim->iname_offset, SEEK_CUR );
        }
        else
        {
          // header has not been written. do it.
          zfp = writeNiftiHeader( options );
//           zfp = api->nifti_image_write_hdr_img( _nim->nim, 2, "wb" );
//           _znzfiles.push_back( carto::rc_ptr<NiftiFileWrapper>(
//             new NiftiFileWrapper( zfp ) ) );
        }
      }

      if( znz_isnull( zfp ) )
        ok = false;

      if( ok )
      {
        dataTOnim( nim, hdr, source, -1, zfp, strides, pos, size, _sizes );

        if( znz_isnull( zfp ) )
          ok = false;
      }

      if( znz_isnull( zfp ) )
        ok = false;
      else if( _znzfiles.empty() ) // close it if not in _znzfiles
        znzclose( zfp );

      // unload data in the nifti_image struct
      nifti_image_unload( nim );
    }
    else // write4d == false
    {
      unsigned f;
//       hdr.setProperty( "series_filenames", fnames );
//       std::vector<int> vdim(4);
//       vdim[0] = sx;
//       vdim[1] = sy;
//       vdim[2] = sz;
//       vdim[3] = st;
//       hdr.setProperty( "volume_dimension", vdim );

      const DataSourceList & dsl = dsi.list();

      for( f=ot; f<ot+vt; ++f )
      {
//         hdr.setName( dname + fnames[f] );
//         hdr.fillNim( false ); // allow4D set to false
//         nifti_image *nim = hdr.niftiNim();

        nifti_set_filenames( nim, dsl.dataSource( "nii", f )->url().c_str(),
          0, 1 );

        znzFile zfp;
        if( _znzfiles.size() > f )
          // file 0 is already open
          zfp = _znzfiles[f]->znzfile;
        else // others are not.
        {
          if( partial )
          {
            // partial writing needs:
            // opening the "img" stream read-write and position correctly in it
            //   (skip the header, if any)
            // seek the correct location, in disk space orientation, before
            //   writing each line
            zfp = znzopen( dsl.dataSource( "nii", f )->url().c_str(), "r+b",
                           nifti_is_gzfile( dsi.url().c_str() ) );
            znzseek( zfp, _nim->nim->iname_offset, SEEK_CUR );
          }
          else
          {
            // header has not been written. do it.
            zfp = api->nifti_image_write_hdr_img( nim, 2, "wb" );
          }
        }
        if( znz_isnull( zfp ) )
          ok = false;
        else
        {
          dataTOnim( nim, hdr, source, f, zfp, strides, pos, size, _sizes );
          if( znz_isnull( zfp ) )
            ok = false;
          else if( _znzfiles.size() <= f )
            znzclose( zfp );
        }
        nifti_image_unload( nim );
        if( !ok )
          break;
      }
    }

    fdi.open(); // enable stderr
    if( !ok )
      carto::io_error::launchErrnoExcept();
  }


  template <typename T>
  DataSourceInfo NiftiImageWriter<T>::writeHeader(
    DataSourceInfo & dsi,
    const T * source,
    const std::vector<int> & pos,
    const std::vector<int> & size,
    const std::vector<long> & strides,
    carto::Object options )
  {
    // check if it will be a sequence of 3D volumes
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

    Object hdr = dsi.header();
    int dimt;
    hdr->getProperty( "sizeT", dimt );
    if( write4d && !canWrite4d( hdr ) )
      write4d = false;

    //--- build datasourcelist -----------------------------------------------
    bool dolist = dsi.list().typecount() == 1;
    if( dolist )
    {
      localMsg( "building DataSourceList..." );
      buildDSList( dsi.list(), options, write4d, dimt, dsi.header() );
    }
    //--- set header ---------------------------------------------------------
    localMsg( "setting Header..." );
    if( options->hasProperty( "partial_writing" ) )
    {
      DataSourceInfoLoader  dsil;
      carto::rc_ptr<DataSource> hdrds;
      try
      {
        hdrds = dsi.list().dataSource( "hdr" );
      }
      catch( ... )
      {
        hdrds = dsi.list().dataSource();
      }
      if( hdrds.isNull() )
        hdrds = dsi.list().dataSource( "nii" );
      DataSourceInfo hdrdsi( hdrds );
      hdrdsi = dsil.check( hdrdsi );
      dsi.header() = hdrdsi.header();
      dsi.privateIOData()->copyProperties( hdrdsi.privateIOData() );
      return dsi;
    }

    checkDiskDataType( hdr, source, strides, size, options );

    //--- write header -------------------------------------------------------
    localMsg( "building Header..." );
    fillNiftiHeader( dsi, options, write4d );

    updateParams( dsi );
    _znzfiles.clear();
//     // FIXME: handle multiple files
//     znzFile zfp = api->nifti_image_write_hdr_img( _nim->nim, 2, "wb" );
//     // don't close file because we have no other way to know if it failed
//     if( znz_isnull( zfp ) )
//     {
//       znzclose(zfp);
//       carto::io_error::launchErrnoExcept();
//     }
//     _znzfiles.push_back( carto::rc_ptr<NiftiFileWrapper>(
//       new NiftiFileWrapper( zfp ) ) );

    // diffusion bvec / bval files
    try
    {
      DataSource* bvalfile = dsi.list().dataSource( "bval" ).get();
      DataSource* bvecfile = dsi.list().dataSource( "bvec" ).get();
      if( bvalfile && bvecfile )
        _writeDiffusionVectors( bvalfile, bvecfile, dsi.header() );
    }
    catch( ... )
    {
    }

    //--- write minf ---------------------------------------------------------
    localMsg( "writing Minf..." );
    carto::Object minf = dsi.header();
    if( minf->hasProperty( "file_type" ) )
      minf->removeProperty( "file_type" );
    minf->setProperty( "format", formatName() );
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
    minf->setProperty( "voxel_size",
                       minf->getProperty( "voxel_size" ) );

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
  void NiftiImageWriter<T>::buildDSList( DataSourceList & dsl,
                                         carto::Object options,
                                         bool write4d, int dimt,
                                         carto::Object header ) const
  {
    DataSource* pds = dsl.dataSource().get();
    std::string niiname, hdrname, minfname, basename;
    enum format_shape
    {
      NII,
      NII_GZ,
      IMG,
    };
    format_shape shape = NII_GZ;

    niiname = pds->url();
    if( niiname.empty() )
    {
      throw carto::file_not_found_error( "needs a filename", niiname );
    }
    else
    {
      std::string ext = carto::FileUtil::extension( niiname );
      basename = carto::FileUtil::removeExtension( niiname );

      if( ext == "gz" )
      {
        ext = ext = carto::FileUtil::extension( basename );
        basename = carto::FileUtil::removeExtension( basename );
        if( ext != "nii" )
          ext = "";
        else
          ext = "nii.gz";
        shape = NII_GZ;
      }
      if( ext == "nii" || ext == "nii.gz" || ext == "img" )
      {
        niiname = basename + "." + ext;
        if( ext == "img" )
        {
          hdrname = basename + ".hdr";
          shape = IMG;
        }
        else if( ext == "nii" )
          shape = NII;
      }
      else if( ext == "hdr" )
      {
        niiname = basename + ".img";
        hdrname = basename + ".hdr";
        shape = IMG;
      }
      else
        ext.clear();

      if( ext.empty() )
      {
        bool compressed = false;
        bool override_ext = false;
        try
        {
          Object ocomp = options->getProperty( "compressed" );
          compressed = bool( ocomp->getScalar() );
        }
        catch( ... )
        {
        }
        try
        {
          Object oovext = options->getProperty( "override_extension" );
          override_ext = bool( oovext->getScalar() );
        }
        catch( ... )
        {
        }
        basename = niiname;
        hdrname = "";
        if( compressed )
        {
          niiname = basename + ".nii.gz";
          ext = "nii.gz";
          shape = NII_GZ;
        }
        else
        {
          niiname = basename + ".nii";
          ext = "nii";
          shape = NII;
        }
        if( override_ext )
        {
          niiname = basename;
          ext = carto::FileUtil::extension( niiname );
        }
      }

      if( !write4d && dimt > 1 )
      {
        if( !ext.empty() )
          ext = std::string( "." ) + ext;
        std::vector<char> name( basename.length() + 7, 0 );
        minfname = basename + "_0000" + ext + ".minf";
        for( int t=0; t<dimt; ++t )
        {
          sprintf( &name[0], "%s_%04d", basename.c_str(), t );
          if( shape == IMG )
            dsl.addDataSource( "hdr",
              carto::rc_ptr<DataSource>(
                new FileDataSource(
                  std::string( &name[0] ) + "hdr" ) ) );
          dsl.addDataSource( "nii", carto::rc_ptr<DataSource>(
              new FileDataSource(
                std::string( &name[0] ) + ext ) ) );
        }
        basename += "_0000";
      }
      else
      {
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
    }

    dsl.addDataSource( "minf",
      carto::rc_ptr<DataSource>( new FileDataSource( minfname ) ) );

    // if there are diffusion B values / directions information, add 2 files
    if( header->hasProperty( "diffusion_directions" )
        && header->hasProperty( "b_values" ) )
    {
      dsl.addDataSource( "bval",
        carto::rc_ptr<DataSource>( new FileDataSource( basename + ".bval" ) ) );
      dsl.addDataSource( "bvec",
        carto::rc_ptr<DataSource>( new FileDataSource( basename + ".bvec" ) ) );
    }

  }


  template <typename T>
  void NiftiImageWriter<T>::_writeDiffusionVectors( DataSource* bvalfile,
                                                    DataSource* bvecfile,
                                                    carto::Object header )
  {
    std::vector<float> bval;
    std::vector<std::vector<float> > bvec, dbvec;
    if( header->getProperty( "b_values", bval )
      && header->getProperty( "diffusion_directions", bvec ) )
    {
      AffineTransformation3d s2m, m2s;
      Object storage_to_memory;
      storage_to_memory = header->getProperty( "storage_to_memory" );
      s2m = storage_to_memory;
      m2s = s2m.inverse();

      if( bval.size() != bvec.size() )
      {
        std::cerr << "Inconsistency between diffusion_directions and b_values.\n";
        return;
      }
      size_t i, j, n = bval.size();
      bvalfile->open( DataSource::Write );
      for( i=0; i<n; ++i )
      {
        if( i != 0 )
          bvalfile->putch( ' ' );
        AsciiDataSourceTraits<float>::write( *bvalfile, bval[i] );
      }
      bvalfile->putch( '\n' );
      bvalfile->close();

      // handle bvec coordinates system
      for( i=0; i<n; ++i )
      {
        Point3df vec1( bvec[i][0],  bvec[i][1],  bvec[i][2] );
        Point3df vec2 = m2s.transform( vec1 )
          - m2s.transform( Point3df( 0, 0, 0 ) );
        std::vector<float> vvec( 3, 0. );
        vvec[0] = vec2[0];
        vvec[1] = vec2[1];
        vvec[2] = vec2[2];
        dbvec.push_back( vvec );
      }

      bvecfile->open( DataSource::Write );
      for( j=0; j<3; ++j )
      {
        for( i=0; i<n; ++i )
        {
          if( i != 0 )
            bvecfile->putch( ' ' );
          AsciiDataSourceTraits<float>::write( *bvecfile, dbvec[i][j] );
        }
        bvecfile->putch( '\n' );
      }
      bvecfile->close();
    }
  }


  template <typename T>
  void NiftiImageWriter<T>::fillNiftiHeader( DataSourceInfo & dsi,
                                             carto::Object options,
                                             bool allow4d )
  {
    /* initialization of a nifti image struct */
    nifti_image *nim = nifti_simple_init_nim();

    Object hdr = dsi.header();

    // filenames

//     bool compressed = false;
    bool override_ext = false;
//     try
//     {
//       Object ocomp = options->getProperty( "compressed" );
//       compressed = bool( ocomp->getScalar() );
//     }
//     catch( ... )
//     {
//     }
    try
    {
      Object oovext = options->getProperty( "override_extension" );
      override_ext = bool( oovext->getScalar() );
    }
    catch( ... )
    {
    }
    std::string niiname = dsi.url();
//     if( compressed )
//     {
//       niiname = basename + ".nii.gz";
//       ext = "nii.gz";
//       shape = NII_GZ;
//     }
//     else
//     {
//       niiname = basename + ".nii";
//       ext = "nii";
//       shape = NII;
//     }

    // cf nifti_convert_nim2nhdr() to know which fields need to be filled

    /***********************/
    /* DATA DIMENSIONALITY */
    /***********************/

    /* Dimensions of grid array */
    std::vector<int> dims(4, 1);
    if( !hdr->getProperty( "volume_dimension", dims ) )
    {
      hdr->getProperty( "sizeX", dims[0] );
      hdr->getProperty( "sizeY", dims[1] );
      hdr->getProperty( "sizeZ", dims[2] );
      hdr->getProperty( "sizeT", dims[3] );
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
    std::vector<int> tdims = dims;
    tdims[0] = short( rint( fabs( df[0] ) ) );
    tdims[1] = short( rint( fabs( df[1] ) ) );
    tdims[2] = short( rint( fabs( df[2] ) ) );
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

    if( dims.size() > 7 )
      dims.resize( 7 );
    if( dims.size() == 4 && dims[3] == 1 )
    {
      dims.erase( dims.begin() + 3 );
      if( dims[2] == 1 )
      {
        dims.erase( dims.begin() + 2 );
        if( dims[1] == 1 )
          dims.erase( dims.begin() + 1 );
      }
    }

    size_t dim, ndim = dims.size();
    nim->ndim = nim->dim[0] = dims.size();
    for( dim=0; dim<ndim; ++dim )
      nim->dim[dim + 1] = tdims[dim];
    for( dim=ndim+1; dim<8; ++dim )
      nim->dim[dim] = 1;
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
    Object vso;
    std::vector<float> vs( ndim, 1.F );
    try
    {
      vso = hdr->getProperty( "voxel_size" );
      if( vso )
      {
        Object vso_it = vso->objectIterator();
        for( dim=0; dim<ndim && vso_it->isValid(); vso_it->next(), ++dim )
        {
          vs[dim] = float( vso_it->currentValue()->getScalar() );
        }
      }
    }
    catch( ... )
    {
    }
    if( hdr->hasProperty( "tr" ) )
      vs[3] = hdr->getProperty( "tr" )->getScalar();

    df = m2s.transform( Point3df( vs[0], vs[1], vs[2] ) )
        - m2s.transform( Point3df( 0, 0, 0 ) );
    std::vector<float> tvs = vs;
    tvs[0] = fabs( df[0] );
    tvs[1] = fabs( df[1] );
    tvs[2] = fabs( df[2] );

    nim->pixdim[0] = 0;
    for( dim=0; dim<ndim; ++dim )
      nim->pixdim[dim + 1] = tvs[dim];
    for( dim=ndim+1; dim<8; ++dim )
      nim->pixdim[dim] = 1.0;
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
    else if( type == "CFLOAT" )
      nim->datatype = DT_COMPLEX64;
    else if( type == "CDOUBLE" )
      nim->datatype = DT_COMPLEX128;
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
    int i;
    for( i=0; i<4; ++i )
      for( int j=0; j<4; ++j )
        S2M_m44.m[i][j] = mvec[j+4*i];
    int icod, jcod, kcod;
    nifti_mat44_to_orientation( S2M_m44, &icod, &jcod, &kcod );

    bool s2moriented = false, s2morientedbis;
    bool qform_is_sform = false;

    for( i=0; i<nt; ++i )
    {
      mv[i] = AffineTransformation3d( transformations->getArrayItem(i) );
      skip = false;
      for( j=0; j<i; ++j )
        if( mv[j] == mv[i] )
        {
          skip = true;
          break;
        }
      if( !skip )
      {
        AffineTransformation3d mot( mv[i] );
        std::string ref = referentials->getArrayItem(i)->getString();
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
        }

        for( int x=0; x<4; ++x )
          for( int j=0; j<4; ++j )
            R.m[x][j] = m[j+4*x];

        bool ok = false;
        int xform_code = NiftiReferential( ref );
        if( ( ( xform_code == NIFTI_XFORM_SCANNER_ANAT )
              || ( nim->sform_code != NIFTI_XFORM_UNKNOWN ) )
              && ( nim->qform_code == NIFTI_XFORM_UNKNOWN ) && s2morientedbis )
        {
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
                ok = false;
                nim->qform_code = NIFTI_XFORM_UNKNOWN;
                break;
              }
        }
        /* The NIFTI format specs are somewhat ambiguous on the question
           whether to fill or not the sform matrix if the qform is already
           filled with the same transform. Many software packages actually do
           so (typically duplicating the scanner transform in both qform and
           sform) so that there is always a sform. AIMS/Soma-IO did not do so
           before version 4.5 because we chose not du duplicate information,
           but it appears that some software might use sform unconditionally
           and disregard qform, so it may be safer to duplicate the scanner
           matrix.
           Thus we now fill sform if it has not been yet, and overwrite it if
           we have a different transform information than the one currently in
           the qform.
        */
        if( nim->sform_code == NIFTI_XFORM_UNKNOWN
          || ( !ok && qform_is_sform ) )
        {
          if( ok )
            qform_is_sform = true;
          else
          {
            qform_is_sform = false;
            ok = true;
          }
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
      AffineTransformation3d NIs2m_aims;
      NIs2m_aims.matrix( 0, 0 ) = -1; // invert all axes
      NIs2m_aims.matrix( 1, 1 ) = -1;
      NIs2m_aims.matrix( 2, 2 ) = -1;
      AffineTransformation3d NIs2m = s2m * NIs2m_aims;
      mvec = NIs2m.toVector();
      mat44 P;
      for( int x=0; x<4; ++x )
        for( int j=0; j<4; ++j )
          P.m[x][j] = mvec[j+4*x];
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

    strcpy( nim->descrip, (formatName() + " (writer: Soma IO "
      + carto::cartobaseShortVersion() + ").\0").c_str() );
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
      unsigned next = extcode.size();
      std::vector<std::vector<char> > extdata( next );
      hdr->getProperty( "extdata", extdata );
      if( extdata.size() < next )
        next = extdata.size();
      for( i=0; i<next; ++i )
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
    if( override_ext )
    {
      free( nim->iname );
      nim->iname = strdup( mainfile->url().c_str() );
      free( nim->fname );
      nim->fname = strdup( mainfile->url().c_str() );
    }
    /* set nifti_type from  */
    //nifti_set_type_from_names( nim );
    /* set iname_offset from nifti_type */
    //nifti_set_iname_offset( nim );


    // store nifti structure in DataSourceInfo

    carto::rc_ptr<NiftiStructWrapper> nimw( new NiftiStructWrapper( nim ) );
    dsi.privateIOData()->setProperty( "nifti_structure", nimw );
  }


  template <typename T>
  void NiftiImageWriter<T>::checkDiskDataType(
    carto::Object hdr,
    const T * source,
    const std::vector<long> & strides,
    const std::vector<int> & size,
    carto::Object options ) const
  {
    // Can float data be saved as integer without loss?

    if( hdr->hasProperty( "scale_factor" ) )
      hdr->removeProperty( "scale_factor" );
    if( hdr->hasProperty( "scale_offset" ) )
      hdr->removeProperty( "scale_offset" );
    if( hdr->hasProperty( "disk_data_type" ) )
      hdr->removeProperty( "disk_data_type" );
    if( hdr->hasProperty( "scale_factor_applied" ) )
      hdr->removeProperty( "scale_factor_applied" );

    std::string code = carto::DataTypeCode<T>().dataType();
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
        bool shen = canEncodeAsScaledS16( source, scale,
                                          offset, strides, size, true,
                                          0 /* &maxm */ );
        if( shen )
        {
          hdr->setProperty( "disk_data_type",
                          carto::DataTypeCode<int16_t>().dataType() );
          hdr->setProperty( "scale_factor_applied", true );
          hdr->setProperty( "scale_factor", scale );
          hdr->setProperty( "scale_offset", offset );
        }
      }
    }
  }

  //==========================================================================
  //   N I F T I 1 I M A G E W R I T E R   M E T H O D S
  //==========================================================================
  template <typename T>
  Nifti1ImageWriter<T>::Nifti1ImageWriter()
    : NiftiImageWriter<T>()
  {
    this->setApi( new Nifti1ApiHelpers );
  }


  template <typename T>
  Nifti1ImageWriter<T>::~Nifti1ImageWriter()
  {
  }


  template <typename T>
  bool Nifti1ImageWriter<T>::checkDimsCompatibility() const
  {
    if( this->_sizes[ 0 ][ 0 ] >= 0x8000 || this->_sizes[ 0 ][ 1 ] >= 0x8000
        || this->_sizes[ 0 ][ 2 ] >= 0x8000 )
      return false;
    return true;
  }


  template <typename T>
  bool Nifti1ImageWriter<T>::canWrite4d( carto::Object hdr ) const
  {
    int sizeT;
    if( hdr->getProperty( "sizeT", sizeT ) )
      return sizeT < 0x8000;
    return false;
  }


  //==========================================================================
  //   N I F T I 2 I M A G E W R I T E R   M E T H O D S
  //==========================================================================
  template <typename T>
  Nifti2ImageWriter<T>::Nifti2ImageWriter()
    : NiftiImageWriter<T>()
  {
    this->setApi( new Nifti2ApiHelpers );
  }


  template <typename T>
  Nifti2ImageWriter<T>::~Nifti2ImageWriter()
  {
  }

}

#undef localMsg
#endif
