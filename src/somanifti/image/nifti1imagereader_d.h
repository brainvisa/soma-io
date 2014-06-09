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

#ifndef SOMAIO_IMAGE_NIFTI1IMAGEREADER_D_H
#define SOMAIO_IMAGE_NIFTI1IMAGEREADER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/nifti1imagereader.h> // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                  // inheritance
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>          // used by clone()
#include <soma-io/datasource/datasource.h>
#include <soma-io/checker/transformation.h>
#include <soma-io/nifticlib/niftilib/nifti1_io.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                    // header, options
#include <cartobase/type/voxelrgb.h>
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "NIFTI1IMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void Nifti1ImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );

    dsi.privateIOData()->getProperty( "nifti_structure", _nim );
  }

  template <typename T>
  void Nifti1ImageReader<T>::resetParams()
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
  Nifti1ImageReader<T>::Nifti1ImageReader() :
    ImageReader<T>()
  {
  }

  template <typename T>
  Nifti1ImageReader<T>::~Nifti1ImageReader()
  {
  }

  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void Nifti1ImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<int> & stride,
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

  // specialize RGB and RGBA

  template <>
  void Nifti1ImageReader<carto::VoxelRGB>::read( carto::VoxelRGB * dest,
                                DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<int> & stride,
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
  void Nifti1ImageReader<carto::VoxelRGBA>::read( carto::VoxelRGBA * dest,
                                DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<int> & stride,
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


  template <typename T>
  template <typename U>
  void Nifti1ImageReader<T>::readType( T * dest, DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<int> & /* stride */,
                                carto::Object      /* options */ )
  {
    // dest is supposed to be allocated

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
    offset_t offset;

//     std::cout << "readType, size: " << vx << ", " << vy << ", " << vz << std::endl;
    nifti_image *nim = _nim->nim;

    // fix erroneous null dimT
    if( nim->dim[4] == 0 )
      nim->dim[4] = 1;

    carto::Object hdr = dsi.header();
    std::vector< float > storage_to_memory;
    hdr->getProperty( "storage_to_memory", storage_to_memory );

    AffineTransformation3d s2m( storage_to_memory );
    AffineTransformation3d m2s = s2m.inverse();

    std::vector<float> s(2);
    hdr->getProperty( "scale_factor", s[0] );
    hdr->getProperty( "scale_offset", s[1] );
    std::string dt;
    hdr->getProperty( "disk_data_type", dt );

    Point3df pdims = m2s.transform( Point3df( vx, vy, vz ) )
        - m2s.transform( Point3df( 0, 0, 0 ) );
//     Point3df posf = m2s.transform( Point3df( ox, oy, oz ) )
//         - m2s.transform( Point3df( 0, 0, 0 ) );
    Point3df posf1 = m2s.transform( Point3df( ox, oy, oz ) );
    Point3df posf2 = m2s.transform( Point3df( ox+vx-1, oy+vy-1, oz+vz-1 ) );
    int idims[3];
    idims[0] = (int) rint( fabs( pdims[0] ) );
    idims[1] = (int) rint( fabs( pdims[1] ) );
    idims[2] = (int) rint( fabs( pdims[2] ) );
    int ipos[3];
//     ipos[0] = (int) rint( fabs( posf[0] ) );
//     ipos[1] = (int) rint( fabs( posf[1] ) );
//     ipos[2] = (int) rint( fabs( posf[2] ) );
    ipos[0] = (int) rint( std::min( posf1[0], posf2[0] ) );
    ipos[1] = (int) rint( std::min( posf1[1], posf2[1] ) );
    ipos[2] = (int) rint( std::min( posf1[2], posf2[2] ) );
    Point3df incf = m2s.transform( Point3df( 1, 0, 0 ) )
        - m2s.transform( Point3df( 0, 0, 0 ) );
    int inc[3];
    inc[0] = int( rint( incf[0] ) );
    inc[1] = int( rint( incf[1] ) );
    inc[2] = int( rint( incf[2] ) );
    Point3df d0f;
    int d0[3];

    std::vector<U> src( ((size_t)sx) * sy * sz );
    T *target = 0;
    void *buf = &src[0];
    size_t ntot = ((size_t)vx) * vy * vz * sizeof(U);
    long ii;
    size_t yoff = idims[0];
    size_t zoff = yoff * idims[1];
    long minc;
    U* psrc, *pmax = &src[src.size()], *pmin = &src[0];
    bool fail = false;
    int t2;
    int subbb0[7] = { 0, 0, 0, 0, 0, 0, 0 },
    subbb1[7] = { 0, 0, 0, 1, 1, 1, 1 };
    subbb0[0] = ipos[0];
    subbb0[1] = ipos[1];
    subbb0[2] = ipos[2];
    subbb1[0] = idims[0];
    subbb1[1] = idims[1];
    subbb1[2] = idims[2];

    if (((carto::DataTypeCode<T>::name() == "FLOAT")
      || (carto::DataTypeCode<T>::name() == "DOUBLE"))&& (s[0] != 0.0))
    {
      hdr->setProperty( "scale_factor_applied", true );
      for( int t=0; t<vt; ++t )
      {
        t2 = t + ot;

        subbb0[3] = t2;
        ii = nifti_read_subregion_image( nim, subbb0, subbb1, &buf );
        if( ii < 0 || (size_t) ii < ntot )
          throw eof_error( dsi.url() );

        for( int z=0; z<vz; ++z )
          for( int y=0; y<vy; ++y )
          {
            d0f = m2s.transform( Point3df( ox, oy+y, oz+z ) );
            d0[0] = int( rint( d0f[0] ) ) - ipos[0];
            d0[1] = int( rint( d0f[1] ) ) - ipos[1];
            d0[2] = int( rint( d0f[2] ) ) - ipos[2];
            // increment as pointer
            minc = zoff * inc[2] + yoff * inc[1] + inc[0];
            psrc = pmin + zoff * d0[2] + yoff * d0[1] + d0[0];
            // we move in the buffer
            offset = ((offset_t)t2) * vz + z;
            offset = offset * vy + y;
            offset = offset * vx;
            target = dest + offset;

            for( int x=0; x<vx; ++x, psrc += minc )
            {
              if( psrc >= pmin && psrc < pmax )
                *target++ = (T) ( ((T) *psrc ) * s[0] + s[1] );
              else
              {
                *target++ = 0;
                fail = true;
              }
            }
          }
      }
    }
    else
    {
      for( int t=0; t<vt; ++t )
      {
        t2 = t + ot;

        subbb0[3] = t2;
//         std::cout << "subbb0: " << subbb0[0] << ", " << subbb0[1] << ", " << subbb0[2] << ", " << subbb0[3] << std::endl;
//         std::cout << "subbb1: " << subbb1[0] << ", " << subbb1[1] << ", " << subbb1[2] << ", " << subbb1[3] << std::endl;
//         std::cout << "nim dim: " << nim->dim[1] << ", " << nim->dim[2] << ", " << nim->dim[3] << ", " << nim->dim[4] << std::endl;
        ii = nifti_read_subregion_image( nim, subbb0, subbb1, &buf );
        if( ii < 0 || (size_t) ii < ntot )
          throw eof_error( dsi.url() );

        for( int z=0; z<vz; ++z )
          for( int y=0; y<vy; ++y )
          {
            d0f = m2s.transform( Point3df( ox, oy+y, oz+z ) );
            d0[0] = int( rint( d0f[0] ) ) - ipos[0];
            d0[1] = int( rint( d0f[1] ) ) - ipos[1];
            d0[2] = int( rint( d0f[2] ) ) - ipos[2];
            // increment as pointer
            minc = zoff * inc[2] + yoff * inc[1] + inc[0];
            psrc = pmin + zoff * d0[2] + yoff * d0[1] + d0[0];
            // we move in the buffer
            offset = ((offset_t)t2) * vz + z;
            offset = offset * vy + y;
            offset = offset * vx;
            target = dest + offset;
//             if( y == 0 )
//               std::cout << "d0: " << d0[0] << ", " << d0[1] << ", " << d0[2] << std::endl;

            for( int x=0; x<vx; ++x, psrc += minc )
            {
              if( psrc >= pmin && psrc < pmax )
                *target++ = (T) *psrc;
              else
              {
                *target++ = 0;
                fail = true;
              }
            }
          }
      }
    }
    if( fail )
      std::cerr << "Warning: storage_to_memory transformation in NIFTI1 file "
          "seems to be wrong" << std::endl;
//     throw carto::corrupt_stream_error( dsi.url() );
  }


  template <typename T>
  ImageReader<T>* Nifti1ImageReader<T>::cloneReader() const
  {
    return new Nifti1ImageReader;
  }

}

#undef localMsg
#endif
