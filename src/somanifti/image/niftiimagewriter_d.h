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
#include <soma-io/transformation/affinetransformation3d_base.h>
#include <soma-io/io/scaledcoding.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
#include <soma-io/utilities/minfutil.h>               // used by writeHeader()
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
#include <cstdio>
#include <memory>
#include <stdexcept>
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
  void dataTOnim_checks2m( soma::AffineTransformationBase & m,
                           const std::vector<int> & p,
                           const std::vector<int> & tp, int ip, int il )
  {
    if( tp[il] != 0 )
    {
      if( tp[il] < 0 )
        m.matrix()(il, 3) = p[ip];
      else
        m.matrix()(il, 3) = 0;
    }
  }


  template <typename T>
  bool expandNiftiScaleFactor( const carto::Object & hdr,
                               nifti_image *nim, 
                               const soma::AffineTransformationBase & m,
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
      size_t dim, ndim = vsz.size();
      if( ndim > 7 )
        ndim = 7; // NIFTI is limited to 7D
      std::vector<int> d0( ndim, 0  );
      std::vector<int> p1( ndim, 0 );
      p1[0] = 1;
      std::vector<int> inc = m.transformVector( p1 );
      long pinc = inc[2] * strides[2]
        + inc[1] * strides[1]
        + inc[0] * strides[0];
      const T *p0;

      // region line size
      long offset, cur_offset = 0, offset2;
      std::vector<long> dstrides( ndim, 0 );
      dstrides[0] = sizeof(int16_t);
      for( dim=0; dim<ndim - 1; ++dim )
        dstrides[ dim + 1 ] = nim->dim[dim + 1] * dstrides[dim];

      size_t vx = vsz[0];
      size_t numbytes = vx * sizeof( int16_t ), ss;
      std::vector<int16_t> buf( vx );
      int16_t *d = 0;

      carto::line_NDIterator_base it( vsz, strides );
      for( ; !it.ended(); ++it )
      {
        const std::vector<int> & volpos = it.position();
        std::vector<int> vp = volpos;
        vp[0] = 0;
        d0 = m.transform( vp );
        d = &buf[0];
        p0 = data;
        offset = 0;
        for( dim=0; dim<ndim; ++dim )
        {
          // memory pointer/offset
          p0 += d0[dim] * strides[dim];
          // disk offset
          offset += ( volpos[dim] + opos[dim] ) * dstrides[dim];
        }
        for( size_t x=0; x<vx; ++x, p0 += pinc )
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
                               const soma::AffineTransformationBase &,
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
                               const soma::AffineTransformationBase &,
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
                               const soma::AffineTransformationBase &,
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
                               const soma::AffineTransformationBase &,
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
                               const soma::AffineTransformationBase &,
                               const cdouble*,
                               znzFile,
                               const std::vector<long> &,
                               const std::vector<int> &,
                               const std::vector<int> & )
  {
    return false;
  }


  template <typename T>
  void writeWithoutScaleFactor( const carto::Object & /* hdr */,
                                nifti_image *nim,
                                const soma::AffineTransformationBase & m,
                                const T* data,
                                znzFile zfp,
                                const std::vector<long> & strides,
                                const std::vector<int> & vsz,
                                const std::vector<int> & opos )
  {
    size_t dim, ndim = vsz.size();
    if( ndim > 7 )
      ndim = 7; // NIFTI is limited to 7D
    std::vector<int> d0( ndim, 0 );
    std::vector<int> p1( ndim, 0 );
    p1[0] = 1;
    std::vector<int> inc = m.transformVector( p1 );
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
    std::vector<T> buf( vx, 0 );
    T *d = 0;

    carto::line_NDIterator_base it( vsz, strides );
    for( ; !it.ended(); ++it )
    {
      const std::vector<int> & volpos = it.position();
      std::vector<int> vp = volpos;
      vp[0] = 0;
      d0 = m.transform( vp );
      d = &buf[0];
      p0 = data;
      offset = 0;
      for( dim=0; dim<ndim; ++dim )
      {
        // memory pointer/offset
        p0 += d0[dim] * strides[dim];
        // disk offset
        offset += ( volpos[dim] + opos[dim] ) * dstrides[dim];
      }
      if( data != 0 )
        for( size_t x=0; x<vx; ++x, p0 += pinc )
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
    soma::AffineTransformationBase m, mems2m;
    std::unique_ptr<soma::AffineTransformationBase> m2s;
    std::vector<int> p, p1, tp;

    try
    {
      carto::Object storage_to_memory;
      storage_to_memory = hdr->getProperty( "storage_to_memory" );
      m = storage_to_memory;
      /* adjust translations so that they fit (in case the vol size has
         changed) */
      unsigned n = m.order();
      p1 = std::vector<int>( n, 0 );
      p = std::vector<int>( n, 0 );
      p[0] = nim->nx - 1;
      p1[0] = 1;
      tp = m.transformVector( p1 );
      dataTOnim_checks2m( m, p, tp, 0, 0 );
      dataTOnim_checks2m( m, p, tp, 0, 1 );
      dataTOnim_checks2m( m, p, tp, 0, 2 );
      p[0] = 0;
      p[1] = nim->ny - 1;
      p1[0] = 0;
      p1[1] = 1;
      tp = m.transformVector( p1 );
      dataTOnim_checks2m( m, p, tp, 1, 0 );
      dataTOnim_checks2m( m, p, tp, 1, 1 );
      dataTOnim_checks2m( m, p, tp, 1, 2 );
      p[1] = 0;
      p[2] = nim->nz - 1;
      p1[1] = 0;
      p1[2] = 1;
      tp = m.transformVector( p1 );
      dataTOnim_checks2m( m, p, tp, 2, 0 );
      dataTOnim_checks2m( m, p, tp, 2, 1 );
      dataTOnim_checks2m( m, p, tp, 2, 2 );
    }
    catch( ... )
    {
      m.matrix()(0, 3) = nim->nx - 1;
      m.matrix()(1, 3) = nim->ny - 1;
      m.matrix()(2, 3) = nim->nz - 1;
      m.matrix()(0,0) = -1.0;
      m.matrix()(1,1) = -1.0;
      m.matrix()(2,2) = -1.0;
    }
    hdr->setProperty( "storage_to_memory", m.toVector() );

    m2s = m.inverse();

    // total volume size, in disk orientation
//     Point3df vtsize = m2s->transform(
//       Point3df( sizes[0][0], sizes[0][1], sizes[0][2] ) )
//       - m2s->transform( p0 );
//     int  sx = int( rint( fabs( vtsize[ 0 ] ) ) );
//     int  sy = int( rint( fabs( vtsize[ 1 ] ) ) );
//     int  sz = int( rint( fabs( vtsize[ 2 ] ) ) );
//     int  st = sizes[ 0 ][ 3 ];

    // region size, in disk orientation
    std::vector<int> tr_size = m2s->transformVector( size );
    size_t dim, n = tr_size.size();
    for( dim=0; dim<n; ++dim )
      tr_size[dim] = std::abs(tr_size[dim] );

    // region position, in disk orientation
    std::vector<int> orig = m2s->transform( pos );
    std::vector<int> posend
      = soma::Transformation::vsub( soma::Transformation::vadd( pos, size ),
                                    1 );
    std::vector<int> pend = m2s->transform( posend );
    std::vector<int> tr_pos = pos;
    if( tr_pos.size() < tr_size.size() )
      tr_pos.resize( tr_size.size(), 0 );
    tr_pos[0] = std::min( std::abs( orig[ 0 ] ), std::abs( pend[ 0 ] ) );
    tr_pos[1] = std::min( std::abs( orig[ 1 ] ), std::abs( pend[ 1 ] ) );
    tr_pos[2] = std::min( std::abs( orig[ 2 ] ), std::abs( pend[ 2 ] ) );
    
    // TODO: still need a s2m matrix for the memory object
    // memory volume-size s2m
    mems2m = m;
    p[2] = 0;
    p[0] = tr_size[0] - 1;
    p1[2] = 0;
    p1[0] = 1;
    tp = mems2m.transformVector( p1 );
    dataTOnim_checks2m( mems2m, p, tp, 0, 0 );
    dataTOnim_checks2m( mems2m, p, tp, 0, 1 );
    dataTOnim_checks2m( mems2m, p, tp, 0, 2 );
    p[0] = 0;
    p[1] = tr_size[1] - 1;
    p1[0] = 0;
    p1[1] = 1;
    tp = mems2m.transformVector( p1 );
    dataTOnim_checks2m( mems2m, p, tp, 1, 0 );
    dataTOnim_checks2m( mems2m, p, tp, 1, 1 );
    dataTOnim_checks2m( mems2m, p, tp, 1, 2 );
    p[1] = 0;
    p[2] = tr_size[2] - 1;
    p1[1] = 0;
    p1[2] = 1;
    tp = mems2m.transformVector( p1 );
    dataTOnim_checks2m( mems2m, p, tp, 2, 0 );
    dataTOnim_checks2m( mems2m, p, tp, 2, 1 );
    dataTOnim_checks2m( mems2m, p, tp, 2, 2 );

    size_t ndim = sizes[0].size();
    if( ndim > 7 )
    {
      ndim = 7; // NIFTI is limited to 7D
      tr_size.resize( ndim );
      tr_pos.resize( ndim );
    }

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


struct QformParams
{
  float quatern_b, quatern_c, quatern_d;
  float qoffset_x, qoffset_y, qoffset_z;
  float qfac;

  void from_mat44(const mat44& R) {
    nifti_mat44_to_quatern( R, &quatern_b, &quatern_c,
                            &quatern_d, &qoffset_x,
                            &qoffset_y, &qoffset_z,
                            NULL, NULL, NULL, &qfac );
  }

  mat44 to_mat44(const std::vector<float>& voxel_size) const {
    return nifti_quatern_to_mat44( quatern_b, quatern_c, quatern_d,
                                   qoffset_x, qoffset_y, qoffset_z,
                                   voxel_size[0], voxel_size[1], voxel_size[2],
                                   qfac );
  }

  bool matches_mat44(const mat44 &R, const std::vector<float>& voxel_size,
                     float epsilon = 1e-4f) const {
    mat44 P = to_mat44(voxel_size);
    for( int i=0; i<3 ; ++i ) {
      for( int j=0; j<3; ++j ) {
        // Test if the relative error (for values > 1) or the absolute error
        // (for values < 1) is smaller than epsilon.
        float amax = std::max( fabs( P.m[i][j] ), fabs( R.m[i][j] ) );
        amax = std::max(amax, 1.f);
        if( !( fabs( P.m[i][j] - R.m[i][j] ) <= epsilon * amax ) )
        {
          return false;
        }
      }
    }
    return true;
  }

  void store_to_nifti_image(nifti_image* nim) {
    nim->quatern_b = quatern_b;
    nim->quatern_c = quatern_c;
    nim->quatern_d = quatern_d;
    nim->qoffset_x = qoffset_x;
    nim->qoffset_y = qoffset_y;
    nim->qoffset_z = qoffset_z;
    nim->qfac = qfac;
  }
};


void store_transformations_as_qform_and_sform(
   const carto::Object& referentials,
   const carto::Object& transformations,
   nifti_image *nim,
   const soma::AffineTransformationBase& voxsz,
   const soma::AffineTransformationBase& s2m,
   const std::vector<float>& tvs,
   const bool debug_transforms)
{
  using soma::AffineTransformationBase;
  using soma::NiftiReferential;

  nim->qform_code = NIFTI_XFORM_UNKNOWN;
  nim->sform_code = NIFTI_XFORM_UNKNOWN;

  size_t nt = std::min(referentials->size(), transformations->size());
  std::vector<int> nifti_referentials( nt );
  if( debug_transforms )
    std::clog << "somanifti: store_transformations_as_qform_and_sform, n: "
      << nt << std::endl;

  std::vector<AffineTransformationBase> mv( nt );
  if( nt > 0 )
    mv[0] = AffineTransformationBase( transformations->getArrayItem(0) );

  bool sform_is_a_copy_of_qform = false;
  for( size_t i=0; i<nt; ++i )
  {
    // Skip duplicate transformations
    mv[i] = AffineTransformationBase( transformations->getArrayItem(i) );
    nifti_referentials[i] = NiftiReferential( referentials->getArrayItem(i)->getString() );
    if( debug_transforms )
      std::clog << "somanifti: process trans " << i + 1 << " to ref: "
        << nifti_referentials[i] << ":\n" << mv[i] << std::endl;
    bool skip = false;
    for( size_t j=0; j<i; ++j )
    {
      if( mv[j] == mv[i] && nifti_referentials[j] == nifti_referentials[i] )
      {
        skip = true;
        if( debug_transforms )
          std::clog << "somanifti: skip trans " << i << std::endl;
        break;
      }
    }
    if( !skip )
    {
      AffineTransformationBase mot( mv[i] );
      std::string ref = referentials->getArrayItem(i)->getString();

      int xform_code = NiftiReferential( ref );
      if( debug_transforms )
        std::clog << "xform_code: " << xform_code << std::endl;
      bool skip_qform = false;
      if( xform_code == NIFTI_XFORM_ALIGNED_ANAT
          && ( nim->sform_code == NIFTI_XFORM_UNKNOWN
               || sform_is_a_copy_of_qform ) )
      {
        // There is no NIfTI code for this referential, ignore this
        // transformation.
        if( debug_transforms )
          std::clog << "xform_code is unknown: skipping qform." << std::endl;
        skip_qform = true;
      }

      std::vector<float> m = (mot * voxsz * s2m).toVector();
      mat44 R;

      for( int x=0; x<4; ++x )
        for( int j=0; j<4; ++j )
          R.m[x][j] = m[j+4*x];

      bool stored_as_qform = false;

      if( !skip_qform && nim->qform_code == NIFTI_XFORM_UNKNOWN )
      {
        QformParams qform_params;
        qform_params.from_mat44(R);
        // check if the matrix can actually be stored as a quaternion
        bool can_be_stored_as_qform = qform_params.matches_mat44(R, tvs);
        if( can_be_stored_as_qform )
        {
          stored_as_qform = true;
          qform_params.store_to_nifti_image(nim);
          nim->qform_code = xform_code;
          if(debug_transforms) {
            std::clog << "somanifti: storing transform "
                      << i + 1 << " / " << nt << " as qform." << std::endl;
          }
        }
      }

      /* The NIFTI format specs are somewhat ambiguous on the question
         whether to fill or not the sform matrix if the qform is already
         filled with the same transform. Many software packages actually do
         so (typically duplicating the scanner transform in both qform and
         sform) so that there is always a sform. AIMS/Soma-IO did not do so
         before version 4.5 because we chose not to duplicate information,
         but it appears that some software might use sform unconditionally
         and disregard qform, so it may be safer to duplicate the scanner
         matrix.
      */
      if( nim->sform_code == NIFTI_XFORM_UNKNOWN || sform_is_a_copy_of_qform ) {
        sform_is_a_copy_of_qform = stored_as_qform;

        if(debug_transforms) {
          std::clog << "somanifti: storing transform "
                    << i + 1 << " / " << nt << " as sform." << std::endl;
          std::clog << "will be a copy: " << sform_is_a_copy_of_qform
                    << std::endl;
        }
        nim->sform_code = xform_code;
        for( int x=0; x<4; ++x )
          for( int j=0; j<4; ++j )
            nim->sto_xyz.m[x][j] = m[j+x*4];
      } else if( !stored_as_qform ) {
        // This warning, even though it could be useful even for people who do
        // not want to debug the transformations, often confuses people because
        // it pops up in red in the middle of BrainVISA processes. We could
        // make it optional, but there is not really a good way of passing the
        // option (Writer options, carto::verbose, header field?).
        if(debug_transforms) {
          std::clog << "somanifti warning: could not save transformation "
                    << i + 1 << " / " << nt << "." << std::endl;
        }
      }
      else if( debug_transforms )
        std::clog << "somanifti warning: could not save transformation "
                  << i + 1 << " / " << nt << "." << std::endl;
      if( debug_transforms )
        std::clog << "stored_as_qform: " << stored_as_qform << ", sform_code: " << nim->sform_code << ", qform_code: " << nim->qform_code << ", sform_is_a_copy_of_qform: " << sform_is_a_copy_of_qform << std::endl;
    }
    else if( debug_transforms )
      std::clog << "skipped.\n";
  }
}

bool nifti_is_s2m_oriented(const soma::AffineTransformationBase& s2m,
                           nifti_image* nim,
                           bool nifti_strong_orientation)
{
  // Calculate the orientation codes that correspond to the storage_to_memory
  // header attribute
  int icod, jcod, kcod;
  {
    // The Nifti convention (RAS+) is the opposite of soma-io (LIP+).
    soma::AffineTransformationBase mi;
    mi.matrix()(0, 0) = mi.matrix()(1, 1) = mi.matrix()(2, 2) = -1;
    std::vector<float> mvec = (s2m * mi).toVector();
    mat44 S2M_m44;
    for( int i=0; i<4; ++i )
      for( int j=0; j<4; ++j )
        S2M_m44.m[i][j] = mvec[j+4*i];
    nifti_mat44_to_orientation( S2M_m44, &icod, &jcod, &kcod );
  }

  int idir = 0, jdir = 0, kdir = 0;

  // nim->qto_xyz is needed for the following test, we fill it like in
  // nifti_convert_nhdr2nim (see nifti1_io.c)
  if( nim->qform_code != NIFTI_XFORM_UNKNOWN )
  {
    nim->qto_xyz = nifti_quatern_to_mat44(nim->quatern_b, nim->quatern_c,
                                          nim->quatern_d,
                                          nim->qoffset_x, nim->qoffset_y,
                                          nim->qoffset_z,
                                          nim->dx, nim->dy, nim->dz,
                                          nim->qfac );
  }

  // Here we apply the same rules as the somanifti reader (see
  // niftiformatchecker.cc). These first rules are non-ambiguous: these 3
  // referentials can be trusted to be in RAS+ orientation.
  if ( nim->qform_code == NIFTI_XFORM_SCANNER_ANAT
       || nim->qform_code == NIFTI_XFORM_MNI_152
       || nim->qform_code == NIFTI_XFORM_TALAIRACH )
  {
    nifti_mat44_to_orientation( nim->qto_xyz, &idir, &jdir, &kdir );
  }
  else if ( nim->sform_code == NIFTI_XFORM_SCANNER_ANAT
              || nim->sform_code == NIFTI_XFORM_MNI_152
              || nim->sform_code == NIFTI_XFORM_TALAIRACH )
  {
    nifti_mat44_to_orientation( nim->sto_xyz, &idir, &jdir, &kdir );
  }

  // Applying the fallback rules allows better control over the stored
  // transformations, but is inherently ambiguous, because the orientation
  // of the ALIGNED_ANAT referential is not well defined by the Nifti
  // standard, nor is the orientation of a Nifti file without any
  // transformation.
  if( !nifti_strong_orientation ) {
    if ( nim->qform_code == NIFTI_XFORM_ALIGNED_ANAT ) {
      nifti_mat44_to_orientation( nim->qto_xyz, &idir, &jdir, &kdir );
    } else if ( nim->sform_code == NIFTI_XFORM_ALIGNED_ANAT ) {
      nifti_mat44_to_orientation( nim->sto_xyz, &idir, &jdir, &kdir );
    } else {
      idir = NIFTI_L2R;
      jdir = NIFTI_P2A;
      kdir = NIFTI_I2S;
    }
  }

  return icod == idir && jcod == jdir && kcod == kdir;
}

void copy_referentials_and_transformations(const carto::Object& referentials,
                                           const carto::Object& transformations,
                                           carto::Object& referentials2,
                                           carto::Object& transformations2)
{
  // Initialize new objects with empty vectors that they own
  transformations2 = carto::Object::value<std::vector<std::vector<float> > >();
  referentials2 = carto::Object::value<std::vector<std::string> >();

  // Obtain a reference to the new vectors owned by the objects
  std::vector<std::vector<float> > & t2
    = transformations2->value<std::vector<std::vector<float> > >();
  std::vector<std::string> & r2
    = referentials2->value<std::vector<std::string> >();

  // Copy the elements one by one
  size_t nt = std::min(referentials->size(), transformations->size());
  r2.reserve(nt);
  t2.reserve(nt);
  for( size_t i=0; i<nt; ++i ) {
    soma::AffineTransformationBase mat(transformations->getArrayItem(i));
    t2.push_back(mat.toVector());
    r2.push_back(referentials->getArrayItem(i)->getString());
  }
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
    carto::fdinhibitor fdi( stderr );
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
    const std::vector<int> & /* pos */,
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

    // Filter minf to remove irrelevant properties
    // and update some property (uuid) from existing minf
    soma::MinfUtil::filter(minf, options);
    soma::MinfUtil::updateFromSource(
        dsi.list().dataSource( "minf" ).get(),
        minf,
        options
    );

    try
    {
      Writer<carto::GenericObject> minfw( dsi.list().dataSource( "minf" ) );
      minfw.write( *minf );
    }
    catch( std::exception & e )
    {
      std::cerr << "exception in writing "
                << dsi.list().dataSource( "minf" )->url() << ": "
                << e.what() << std::endl;
    }
    // close the stream to ensure nothing will be appended later.
    dsi.list().dataSource( "minf" )->close();

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
      IMG_GZ
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
        if(( ext != "nii" ) && (ext != "img") && (ext != "hdr"))
          ext = "";
        if ((ext == "img") || (ext == "hdr")) {
          ext = "img.gz";
          shape = IMG_GZ;
        }
        else {
          ext = "nii.gz";
          shape = NII_GZ;
        }
      }
      if( ext == "nii" || ext == "nii.gz" || ext == "img" || ext == "img.gz" )
      {
        niiname = basename + "." + ext;
        if( (ext == "img") || (ext == "img.gz") )
        {
          hdrname = basename + ".hdr";
          
          if(ext == "img")
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
          if(( shape == IMG ) || (shape == IMG_GZ))
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
      AffineTransformationBase s2m;
      std::unique_ptr<AffineTransformationBase> m2s;
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
        std::vector<float> vec2 = m2s->transformVector( bvec[i] );
        vec2.resize( 3, 0.f );
        dbvec.push_back( vec2 );
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

    AffineTransformationBase s2m;
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
      s2m = AffineTransformationBase( 3 );
      s2m.matrix()(0,3) = dims[0] - 1;
      s2m.matrix()(1,3) = dims[1] - 1;
      s2m.matrix()(2,3) = dims[2] - 1;
      s2m.matrix()(0,0) = -1.0;
      s2m.matrix()(1,1) = -1.0;
      s2m.matrix()(2,2) = -1.0;
    }

    std::unique_ptr<AffineTransformationBase> m2s = s2m.inverse();
    std::vector<int> tdims = m2s->transformVector( dims );
    size_t dim, ndim = dims.size();
    for( dim=0; dim<ndim; ++dim )
      tdims[dim] = std::abs( tdims[dim] );
    // fix s2m dims if needed (happens in case image dims have changed)
    unsigned so = s2m.order();
    for( dim=0; dim<so; ++dim )
      s2m.matrix()( dim, so ) = 0; // erase initial translation
    std::vector<int> p = s2m.transform( tdims );

    for( dim=0; dim<ndim; ++dim )
      if( p[dim] < 0 )
        s2m.matrix()( dim, so ) = -p[dim] - 1;

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
      
    if((nim->dim[dims.size()] == 1) && (nim->dim[0] > 1))
      nim->ndim = nim->dim[0] -= 1;

    if (!allow4d)
    {
      nim->nt = nim->nu = nim->nv = nim->nw = 1;
      nim->dim[4] = nim->dim[5] = nim->dim[6] = nim->dim[7] = 1;
      nim->ndim = nim->dim[0] = (dims.size() >= 4)? 3 : dims.size();
    }
    
    nim->nvox =  nim->nx * nim->ny * nim->nz
              * nim->nt * nim->nu * nim->nv * nim->nw ;

    /* Grid spacings. Code below relies on the voxel size containing at least
       3 values. */
    Object vso;
    std::vector<float> vs( std::max(ndim, static_cast<size_t>(3)), 1.F );
    try
    {
      vso = hdr->getProperty( "voxel_size" );
      if( vso )
      {
        Object vso_it = vso->objectIterator();
        for( dim=0; dim<ndim && vso_it->isValid(); vso_it->next(), ++dim )
        {
          vs[dim] = float( vso_it->currentValue()->getScalar() );
          if(vs[dim] == 0.f) {
            // Nifti transforms do not play well with null voxel sizes
            vs[dim] = 1.f;
          }
        }
      }
    }
    catch( ... )
    {
    }
    if( hdr->hasProperty( "tr" ) )
      vs[3] = hdr->getProperty( "tr" )->getScalar();

    std::vector<float> tvs = m2s->transformVector( vs );
    for( dim=0; dim<tvs.size(); ++dim )
      tvs[dim] = fabs( tvs[dim] );

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

    localMsg("disk data type read is "
           + (type.size() > 0 ? type : "not specified"));
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
    nim->scl_slope = 0.0;
    try {
      Object o = hdr->getProperty( "scale_factor" );
      if( !o.isNull() )
        nim->scl_slope = o->getScalar();
    } catch(...) {
    }
    nim->scl_inter = 0.0;
    try {
      Object o = hdr->getProperty( "scale_offset" );
      if( !o.isNull() )
        nim->scl_inter = o->getScalar();
    } catch(...) {
    }

    /* Calibration parameter: min, max */
    nim->cal_min = 0.0;
    try{
      Object o = hdr->getProperty( "cal_min" );
      if( !o.isNull() )
        nim->cal_min = o->getScalar();
    } catch(...) {
    }
    nim->cal_max = 0.0;
    try {
      Object o = hdr->getProperty( "cal_max" );
      if( !o.isNull() )
        nim->cal_max = o->getScalar();
    } catch(...) {
    }

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

    nim->slice_duration = 0.0; /* TODO */ // should be tr?
    try {
      Object o = hdr->getProperty( "slice_duration" );
      if( !o.isNull() )
        nim->slice_duration = o->getScalar();
    } catch(...) {
    }

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
    AffineTransformationBase voxsz;
    voxsz.matrix()(0,0) = vs[0];
    voxsz.matrix()(1,1) = vs[1];
    voxsz.matrix()(2,2) = vs[2];

    const bool debug_transforms = false;

    /* soma-io has a fixed in-memory layout with respect to anatomical axes
       (LIP+), but Nifti does not have a fixed on-disk data orientation.
       Therefore, the somanifti reader uses the qform and sform to determine
       the correct axis flips/swaps when loading data from disk. Here we
       determine (in the s2moriented variable) if the transformations written
       so far in the Nifti file will allow a correct round-trip when it is read
       with somanifti.
     */
    bool nifti_strong_orientation = true;
    try
    {
      if( !options.isNull() )
      {
        carto::Object aso = options->getProperty( "nifti_strong_orientation" );
        if( !aso.isNull() )
          nifti_strong_orientation = static_cast<bool>( aso->getScalar() );
      }
    }
    catch( ... )
    {
    }

    /* If the transformations written so far do not allow the somanifti writer
       to preserve the in-memory orientation (see s2moriented below), then we
       must set a new dummy qform, unless the user explicitly requests the
       writer to allow the orientation change.
     */
    bool allow_orientation_change = false;
    try
    {
      if( !options.isNull() )
      {
        carto::Object aso = options->getProperty( "allow_orientation_change" );
        if( !aso.isNull() )
          allow_orientation_change = static_cast<bool>( aso->getScalar() );
      }
    }
    catch( ... )
    {
    }

    // Copy transformations/referentials into transformations2/referentials2 so
    // that we can modify them without affecting the original values.
    carto::Object referentials2, transformations2;
    copy_referentials_and_transformations(referentials, transformations,
                                          referentials2, transformations2);
    std::vector<std::vector<float> > & t2
      = transformations2->value<std::vector<std::vector<float> > >();
    std::vector<std::string> & r2
      = referentials2->value<std::vector<std::string> >();

    store_transformations_as_qform_and_sform(referentials2,
                                             transformations2,
                                             nim, voxsz, s2m, tvs,
                                             debug_transforms);
    bool s2moriented = nifti_is_s2m_oriented(s2m, nim,
                                             nifti_strong_orientation);
    if( !s2moriented && !allow_orientation_change )
    {
      // FIXME It would be better to search the list of transformations for a
      // transformation that has the correct orientation and put it first, only
      // generating a dummy transformation as a fallback.
      if(nim->qform_code != NIFTI_XFORM_UNKNOWN || debug_transforms) {
        std::clog << "somanifti: inserting a dummy scanner-based qform "
                     "to fix data orientation" << std::endl;
      }

      AffineTransformationBase NIs2m_aims;
      NIs2m_aims.matrix()( 0, 0 ) = -1; // invert all axes
      NIs2m_aims.matrix()( 1, 1 ) = -1;
      NIs2m_aims.matrix()( 2, 2 ) = -1;

      // /* TODO */ // use 'origin' field
      NIs2m_aims.matrix()(0, 3) = dims[0] * vs[0] / 2;
      NIs2m_aims.matrix()(1, 3) = ( dims[1] - 2 ) * vs[1] / 2;
      NIs2m_aims.matrix()(2, 3) = ( dims[2] - 2 ) * vs[2] / 2;

      t2.insert( t2.begin(), NIs2m_aims.toVector() );
      r2.insert( r2.begin(), NiftiReferential( NIFTI_XFORM_SCANNER_ANAT ) );

      store_transformations_as_qform_and_sform(referentials2,
                                               transformations2,
                                               nim, voxsz, s2m, tvs,
                                               debug_transforms);
      s2moriented = nifti_is_s2m_oriented(s2m, nim,
                                          nifti_strong_orientation);
    }

    if(!s2moriented) {
      std::clog << "somanifti warning: the data of the saved Nifti file "
                   "will be loaded in a different in-memory order "
                   "when opened by somanifti." << std::endl;
      if(!allow_orientation_change) {
        // this should never happen
        throw std::logic_error("somanifti failed to preserve s2m orientation");
      }
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

    nim->toffset = 0.0;
    try {
      Object o = hdr->getProperty( "toffset" );
      if( !o.isNull() )
        nim->toffset = o->getScalar();
    } catch(...) {
    }

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

    nim->intent_p1 = 0.0;
    try {
      Object o = hdr->getProperty( "intent_p1" );
      if( !o.isNull() )
        nim->intent_p1 = o->getScalar();
    } catch(...) {
    }
    nim->intent_p2 = 0.0;
    try {
      Object o = hdr->getProperty( "intent_p2" );
      if( !o.isNull() )
        nim->intent_p2 = o->getScalar();
    } catch(...) {
    }
    nim->intent_p3 = 0.0;
    try {
      Object o = hdr->getProperty( "intent_p3" );
      if( !o.isNull() )
        nim->intent_p3 = o->getScalar();
    } catch(...) {
    }

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
      size_t next = extcode.size();
      std::vector<std::vector<char> > extdata( next );
      hdr->getProperty( "extdata", extdata );
      if( extdata.size() < next )
        next = extdata.size();
      for( size_t i=0; i<next; ++i )
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
      if( !forcedDT && source )
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
