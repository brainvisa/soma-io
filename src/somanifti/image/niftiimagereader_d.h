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

#ifndef SOMAIO_IMAGE_NIFTIIMAGEREADER_D_H
#define SOMAIO_IMAGE_NIFTIIMAGEREADER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/niftiimagereader.h> // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                  // inheritance
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>          // used by clone()
#include <soma-io/datasource/datasource.h>
#include <soma-io/transformation/affinetransformation3d_base.h>
#include <soma-io/nifticlib/niftilib/nifti2_io.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                    // header, options
#include <cartobase/type/datatypetraits.h>              // datatypetraits
#include <cartobase/type/voxelrgb.h>
#include <cartobase/containers/nditerator.h>
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "NIFTIIMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void NiftiImageReader<T>::updateParams( DataSourceInfo & dsi )
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
  void NiftiImageReader<T>::resetParams()
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
  NiftiImageReader<T>::NiftiImageReader( const std::string & format_id ) :
    ImageReader<T>(), _format_id( format_id )
  {
  }

  template <typename T>
  NiftiImageReader<T>::~NiftiImageReader()
  {
  }

  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void NiftiImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                   std::vector<int> & pos,
                                   std::vector<int> & size,
                                   std::vector<long> & stride,
                                   carto::Object      options )
  {
    if( _sizes.empty() || _nim.isNull() )
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

  // specialize RGB, RGBA and complex

  template <>
  void NiftiImageReader<carto::VoxelRGB>::read( carto::VoxelRGB * dest,
                                DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<long> & stride,
                                carto::Object      options )
  {
    if( _sizes.empty() || _nim.isNull() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "RGB" )
      readType<carto::VoxelRGB>( dest, dsi, pos, size, stride, options );
    else if( dt == "RGBA" )
      readType<carto::VoxelRGBA>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }


  template <>
  void NiftiImageReader<carto::VoxelRGBA>::read( carto::VoxelRGBA * dest,
                                DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<long> & stride,
                                carto::Object      options )
  {
    if( _sizes.empty() || _nim.isNull() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "RGB" )
      readType<carto::VoxelRGB>( dest, dsi, pos, size, stride, options );
    else if( dt == "RGBA" )
      readType<carto::VoxelRGBA>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }


  template <>
  void NiftiImageReader<cfloat>::read( cfloat * dest,
                                        DataSourceInfo & dsi,
                                        std::vector<int> & pos,
                                        std::vector<int> & size,
                                        std::vector<long> & stride,
                                        carto::Object options )
  {
    if( _sizes.empty() || _nim.isNull() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "CFLOAT" )
      readType<cfloat>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }


  template <>
  void NiftiImageReader<cdouble>::read( cdouble * dest,
                                         DataSourceInfo & dsi,
                                         std::vector<int> & pos,
                                         std::vector<int> & size,
                                         std::vector<long> & stride,
                                         carto::Object options )
  {
    if( _sizes.empty() || _nim.isNull() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "CDOUBLE" )
      readType<cdouble>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }

}


namespace
{

  template <typename T> inline T _scaledValue(
    const T & value, double scale, double offset)
  {
    return (T) ( value * scale 
              + (typename carto::DataTypeTraits<T>::ChannelType)offset );
  }


  template <> inline cfloat _scaledValue(
    const cfloat & value, double /* scale */, double /* offset */)
  {
    return value;
  }


  template <> inline cdouble _scaledValue(
    const cdouble & value, double /* scale */, double /* offset */)
  {
    return value;
  }

}


namespace soma
{

  template <typename T>
  template <typename U>
  void NiftiImageReader<T>::readType( T * dest, DataSourceInfo & dsi,
                                      std::vector<int> & pos,
                                      std::vector<int> & size,
                                      std::vector<long> & stride,
                                      carto::Object /* options */ )
  {
    // dest is supposed to be allocated

    size_t dim, ndim = size.size();
    if( ndim > 7 )
      ndim = 7; // NIFTI is limited to 7 dimensions.

    // region size
    int  vx = size[ 0 ];
    int  vy = size[ 1 ];
    int  vz = size[ 2 ];
    // region position
    int  ox = pos[ 0 ];
    int  oy = pos[ 1 ];
    int  oz = pos[ 2 ];

    offset_t offset;

    nifti_image *nim = _nim->nim;

    // fix erroneous null dimT
    if( nim->dim[4] == 0 )
      nim->dim[4] = 1;

    carto::Object hdr = dsi.header();
    std::vector< float > storage_to_memory;
    hdr->getProperty( "storage_to_memory", storage_to_memory );

    AffineTransformation3dBase s2m( storage_to_memory );
    std::unique_ptr<AffineTransformation3dBase> m2s = s2m.inverse();

    std::vector<float> s(2);
    s[0] = 1.;
    s[1] = 0.;
    hdr->getProperty( "scale_factor", s[0] );
    hdr->getProperty( "scale_offset", s[1] );
    std::string dt;
    hdr->getProperty( "disk_data_type", dt );

    std::vector<int> idims = m2s->transformVector( size );
    Point3df posf1 = m2s->transform( Point3df( pos[0], pos[1], pos[2] ) );
    Point3df posf2 = m2s->transform( Point3df( pos[0] + vx - 1,
                                               pos[1] + vy - 1,
                                               pos[2] + vz - 1 ) );
    for( dim=0; dim<ndim; ++dim )
      idims[dim] = std::abs( idims[dim] );
    std::vector<int> ipos = pos;
    ipos[0] = (int) rint( std::min( posf1[0], posf2[0] ) );
    ipos[1] = (int) rint( std::min( posf1[1], posf2[1] ) );
    ipos[2] = (int) rint( std::min( posf1[2], posf2[2] ) );
    Point3df incf = m2s->transformVector( Point3df( 1, 0, 0 ) );
    int inc[3];
    inc[0] = int( rint( incf[0] ) );
    inc[1] = int( rint( incf[1] ) );
    inc[2] = int( rint( incf[2] ) );
    Point3df d0f;
    std::vector<int> d0;

    int subbb0[7] = { 0, 0, 0, 0, 0, 0, 0 },
    subbb1[7] = { 0, 0, 0, 1, 1, 1, 1 };
    subbb0[0] = ipos[0];
    subbb0[1] = ipos[1];
    subbb0[2] = ipos[2];
    subbb1[0] = idims[0];
    subbb1[1] = idims[1];
    subbb1[2] = idims[2];

    // check if it is a compressed stream to optimize reading
    char *tmpimgname = nifti_findimgname(nim->iname , nim->nifti_type);
    if( !tmpimgname )
    {
      tmpimgname = nifti_findimgname( dsi.url().c_str(), nim->nifti_type );
      // replace nifti structure filename by the correct one
      free( nim->iname );
      nim->iname = strdup( tmpimgname );
    }
    bool gzipped = nifti_is_gzfile( tmpimgname );

    free( tmpimgname );
    size_t len = 1;
    if( gzipped )
    {
      // compressed: read all in one call, but fully duplicate the buffer:
      // needs twice as much memory as the volume
      for( dim=3; dim<ndim; ++dim )
      {
        subbb0[dim] = pos[dim];
        subbb1[dim] = size[dim];
        len *= size[dim];
      }
    }

    std::vector<U> src( ((size_t)vx) * vy * vz * len );
    T *target = 0;
    void *buf = &src[0];
    size_t ntot = ((size_t)vx) * vy * vz * sizeof(U);
    long ii;
    size_t yoff = idims[0];
    size_t zoff = yoff * idims[1];
    size_t toff = 0;
    long minc;
    U* psrc, *pmax = &src[src.size()], *pmin = &src[0];
    bool fail = false;
    // strides in dest data
    if( stride.size() < ndim )
      stride.resize( ndim, 0 );
    if( stride[0] == 0 )
      stride[0] = 1;

    std::vector<long> istride( ndim, 1 );
    for( dim=1; dim<ndim; ++dim )
    {
      if( stride[dim] == 0 )
        stride[dim] = size[ dim - 1 ] * stride[ dim - 1 ];
      istride[dim] = idims[ dim - 1 ] * istride[ dim - 1 ];
    }
    long dstinc = stride[0]; // just for faster access

    if( gzipped )
    {
      // compressed stream are read just once, completely
      ii = nifti_read_subregion_image( nim, subbb0, subbb1, &buf );
      if( ii < 0 || (size_t) ii < ntot * len )
        throw eof_error( dsi.url() );
      toff = zoff * idims[2];
    }

    if( s[0] != 0. && ( s[0] != 1. || s[1] != 0. ) )
    {
      hdr->setProperty( "scale_factor_applied", true );
      std::vector<int> tsize( size.begin() + 3, size.end() );
      std::vector<int> tstrides( stride.begin() + 3, stride.end() );
      std::vector<int> tpos( ndim - 3, 0 );
      std::vector<int> volpos( ndim, 0 );

      NDIterator_base it( tsize, tstrides );
      for( ; !it.ended(); ++it )
      {
        toff = 0;
        for( dim=3; dim<ndim; ++dim )
        {
          volpos[dim] = it.position()[ dim - 3 ] + pos[ dim ];
          if( gzipped )
            toff += istride[dim] * it.position()[ dim - 3 ];
          else
            subbb0[dim] = volpos[dim];
        }

        if( !gzipped )
        {
          ii = nifti_read_subregion_image( nim, subbb0, subbb1, &buf );
          if( ii < 0 || (size_t) ii < ntot )
            throw eof_error( dsi.url() );
        }

        for( int z=0; z<vz; ++z )
          for( int y=0; y<vy; ++y )
          {
            d0 = volpos;
            d0f = m2s->transform( Point3df( ox, oy+y, oz+z ) );
            d0[0] = int( rint( d0f[0] ) ) - ipos[0];
            d0[1] = int( rint( d0f[1] ) ) - ipos[1];
            d0[2] = int( rint( d0f[2] ) ) - ipos[2];
            // increment as pointer
            minc = zoff * inc[2] + yoff * inc[1] + inc[0];
            psrc = pmin + toff + zoff * d0[2] + yoff * d0[1] + d0[0];
            // we move in the buffer
            offset = it.offset() + z * stride[2] + y * stride[1];
            target = dest + offset;

            for( int x=0; x<vx; ++x, psrc += minc )
            {
              if( psrc >= pmin && psrc < pmax )
                *target = _scaledValue( (T) *psrc, s[0], s[1] );
              else
              {
                *target = 0;
                fail = true;
              }
              target += dstinc;
            }
          }
      }
    }
    else
    {
      std::vector<int> tsize( size.begin() + 3, size.end() );
      std::vector<int> tstrides( stride.begin() + 3, stride.end() );
      std::vector<int> tpos( ndim - 3, 0 );
      std::vector<int> volpos( ndim, 0 );

      NDIterator_base it( tsize, tstrides );
      for( ; !it.ended(); ++it )
      {
        toff = 0;
        for( dim=3; dim<ndim; ++dim )
        {
          volpos[dim] = it.position()[ dim - 3 ] + pos[ dim ];
          if( gzipped )
            toff += istride[dim] * it.position()[ dim - 3 ];
          else
            subbb0[dim] = volpos[dim];
        }

        if( !gzipped )
        {
          ii = nifti_read_subregion_image( nim, subbb0, subbb1, &buf );
          if( ii < 0 || (size_t) ii < ntot )
            throw eof_error( dsi.url() );
        }

        for( int z=0; z<vz; ++z )
          for( int y=0; y<vy; ++y )
          {
            d0 = volpos;
            d0f = m2s->transform( Point3df( ox, oy+y, oz+z ) );
            d0[0] = int( rint( d0f[0] ) ) - ipos[0];
            d0[1] = int( rint( d0f[1] ) ) - ipos[1];
            d0[2] = int( rint( d0f[2] ) ) - ipos[2];
            // increment as pointer
            minc = zoff * inc[2] + yoff * inc[1] + inc[0];
            psrc = pmin + toff + zoff * d0[2] + yoff * d0[1] + d0[0];
            // we move in the buffer
            offset = it.offset() + z * stride[2] + y * stride[1];
            target = dest + offset;

            for( int x=0; x<vx; ++x, psrc += minc )
            {
              if( psrc >= pmin && psrc < pmax )
                *target = (T) *psrc;
              else
              {
                *target = 0;
                fail = true;
              }

              target += dstinc;
            }
          }
      }
    }
    if( fail )
      std::cerr << "Warning: storage_to_memory transformation in NIFTI1 file "
          "seems to be wrong" << std::endl;
  }


  template <typename T>
  ImageReader<T>* NiftiImageReader<T>::cloneReader() const
  {
    return new NiftiImageReader( formatID() );
  }

}

#undef localMsg
#endif
