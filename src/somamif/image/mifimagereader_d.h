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

#ifndef SOMAIO_IMAGE_MIFIMAGEREADER_D_H
#define SOMAIO_IMAGE_MIFIMAGEREADER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/mifimagereader.h> // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                  // inheritance
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>          // used by clone()
#include <soma-io/datasource/datasource.h>
#include <soma-io/transformation/affinetransformation3d_base.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                    // header, options
#include <cartobase/type/datatypetraits.h>              // datatypetraits
#include <cartobase/type/voxelrgb.h>
#include <cartobase/containers/nditerator.h>
//--- system -----------------------------------------------------------------
#include <stdexcept>
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "MIFIMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------


namespace soma
{

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void MifImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    if(!dsi.header()->getProperty( "volume_dimension", _dims )
       || dsi.privateIOData()->getProperty( "_mif_storage_strides", _strides )
       || dsi.privateIOData()->getProperty( "_mif_data_offset", _data_offset )
       || dsi.privateIOData()->getProperty( "_mif_byteswap", _byteswap )) {
      throw std::logic_error("missing fields that should have been set by MifFormatChecker");
    }
  }

  template <typename T>
  void MifImageReader<T>::resetParams()
  {
    _dims.clear();
    _strides.clear();
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
  MifImageReader<T>::MifImageReader() :
    ImageReader<T>()
  {
  }

  template <typename T>
  MifImageReader<T>::~MifImageReader()
  {
  }

  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void MifImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<long> & stride,
                                carto::Object      options )
  {
    if( _dims.empty() )
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

#if 0  // TODO remove if superfluous
  // specialize complex
  template <>
  void MifImageReader<cfloat>::read( cfloat * dest,
                                     DataSourceInfo & dsi,
                                     std::vector<int> & pos,
                                     std::vector<int> & size,
                                     std::vector<long> & stride,
                                     carto::Object options )
  {
    if( _dims.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "CFLOAT" )
      readType<cfloat>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }


  template <>
  void MifImageReader<cdouble>::read( cdouble * dest,
                                      DataSourceInfo & dsi,
                                      std::vector<int> & pos,
                                      std::vector<int> & size,
                                      std::vector<long> & stride,
                                      carto::Object options )
  {
    if( _dims.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "CDOUBLE" )
      readType<cdouble>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }
#endif
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
  void MifImageReader<T>::readType( T *const dest, DataSourceInfo & dsi,
                                    std::vector<int> & pos,
                                    std::vector<int> & size,
                                    std::vector<long> & stride,
                                    carto::Object /* options */ )
  {
    if( _dims.empty() )
      updateParams( dsi );

    auto data_ds = dsi.list().dataSource("data");
    if(!data_ds->isOpen()) {
      data_ds->open(DataSource::Read);
    }
    // Seek to the beginning of the data
    if(!data_ds->at(_data_offset)) {
      throw read_write_error("MIF reader cannot seek to the data offset");
    }

    size_t total_size = sizeof(T);
    for(auto it = begin(_dims); it != end(_dims); ++it)
      total_size *= *it;

    // FIXME: obviously this does not handle any shape, strides,
    // byteswapping, or scaling whatsoever
    std::clog << "MIF byteswap: " << _byteswap << std::endl;
    char * const dest_begin = reinterpret_cast<char*>(dest);
    char * dest_cur = reinterpret_cast<char*>(dest);
    while(dest_cur < dest_begin + total_size && !!(*data_ds)) {
      std::size_t bytes_read = data_ds->readBlock(dest_cur, total_size);
      localMsg("read " + toString(bytes_read) + " bytes");
      dest_cur += bytes_read;
    }
  }


  template <typename T>
  ImageReader<T>* MifImageReader<T>::cloneReader() const
  {
    return new MifImageReader();
  }

}

#undef localMsg
#endif
