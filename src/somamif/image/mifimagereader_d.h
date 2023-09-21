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
#include <cartobase/type/byte_order.h>
#include <cartobase/containers/nditerator.h>
//--- system -----------------------------------------------------------------
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <memory>
#include <numeric>
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
       || !dsi.privateIOData()->getProperty( "_mif_data_layout", _data_layout )
       || !dsi.privateIOData()->getProperty( "_mif_data_offset", _data_offset )
       || !dsi.privateIOData()->getProperty( "_mif_byteswap", _byteswap )) {
      localMsg("missing/incorrect fields that should have been set by MifFormatChecker");
      throw std::logic_error("missing/incorrect fields that should have been set by MifFormatChecker");
    }
    if(dsi.header()->hasProperty( "scale_factor" )
       && dsi.header()->hasProperty( "scale_offset" )) {
      _scaling = true;
      if(!dsi.header()->getProperty( "scale_factor", _scale_factor)
         || !dsi.header()->getProperty( "scale_offset", _scale_offset)) {
      localMsg("incorrect scaling fields");
        throw std::logic_error("missing/incorrect fields that should have been set by MifFormatChecker");
      }
    } else {
      _scaling = false;
    }
  }

  template <typename T>
  void MifImageReader<T>::resetParams()
  {
    _dims.clear();
    _data_layout.clear();
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

    if( dt == DataTypeCode<T>::name() )
      readType<T>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }

  // Specialized case for float and double: may be used to read any scalar disk
  // data type when scaling is used
  template <>
  void MifImageReader<float>::read( float * dest, DataSourceInfo & dsi,
                                    std::vector<int> & pos,
                                    std::vector<int> & size,
                                    std::vector<long> & stride,
                                    carto::Object      options )
  {
    if( _dims.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "FLOAT" )
      readType<float>( dest, dsi, pos, size, stride, options );
    else if( dt == "S8" )
      readType<int8_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U8" )
      readType<uint8_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "S16" )
      readType<int16_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U16" )
      readType<uint16_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "S32" )
      readType<int32_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U32" )
      readType<uint32_t>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }

  template <>
  void MifImageReader<double>::read( double * dest, DataSourceInfo & dsi,
                                     std::vector<int> & pos,
                                     std::vector<int> & size,
                                     std::vector<long> & stride,
                                     carto::Object      options )
  {
    if( _dims.empty() )
      updateParams( dsi );

    std::string dt;
    dsi.header()->getProperty( "disk_data_type", dt );

    if( dt == "DOUBLE" )
      readType<double>( dest, dsi, pos, size, stride, options );
    else if( dt == "FLOAT" )
      readType<float>( dest, dsi, pos, size, stride, options );
    else if( dt == "S8" )
      readType<int8_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U8" )
      readType<uint8_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "S16" )
      readType<int16_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U16" )
      readType<uint16_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "S32" )
      readType<int32_t>( dest, dsi, pos, size, stride, options );
    else if( dt == "U32" )
      readType<uint32_t>( dest, dsi, pos, size, stride, options );
    else
      throw carto::datatype_format_error( dsi.url() );
  }

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


namespace std
{
  // For debugging
  template <typename T>
  ostream& operator << (ostream& os, std::vector<T> const & vec) {
    os << "[";
    auto it = begin(vec);
    if(it != end(vec)) {
      os << *it;
      ++it;
    }
    for(; it != end(vec); ++it) {
      os << ", ";
      os << *it;
    }
    os << "]";
    return os;
  }
}

namespace { // for file-local helper functions

template <typename SequenceT>
std::vector<size_t> argsort_abs(SequenceT const & values) {
  std::vector<size_t> ret(values.size());
  std::iota(begin(ret), end(ret), 0);
  std::sort(begin(ret), end(ret), [values](auto a, auto b) -> bool {
    return std::abs(values[a]) < std::abs(values[b]);
  });
  return ret;
}

template <typename T, typename DiskT>
T apply_scaling(DiskT raw_value, float _scale_factor, float _scale_offset)
{
  return raw_value * _scale_factor + _scale_offset;
}

template <>
std::complex<double> apply_scaling(std::complex<double> raw_value,
                                   float _scale_factor, float _scale_offset)
{
  return raw_value * static_cast<double>(_scale_factor) + static_cast<double>(_scale_offset);
}


} // end of anonymous namespace for file-local helper functions


namespace soma
{

  template <typename T>
  template <typename DiskT>
  void MifImageReader<T>::readType( T *const dest, DataSourceInfo & dsi,
                                    std::vector<int> & pos,
                                    std::vector<int> & size,
                                    std::vector<long> & strides_to_lpi,
                                    carto::Object /* options */ )
  {
    // strides are given in LPI orientation
    localMsg(__func__ + std::string(" called with:")
             + "\npos = " + toString(pos)
             + "\nsize = " + toString(size)
             + "\nstride = " + toString(strides_to_lpi)
             + "\n_mif_data_offset = " + toString(_data_offset)
             + "\n_mif_data_layout = " + toString(_data_layout)
             + "\n_mif_byteswap = " + toString(_byteswap)
             + "\nvolume_dimension = " + toString(_dims) + "\n");

    if( _dims.empty() )
      updateParams( dsi );

    const size_t ndims = _data_layout.size();
    assert(_dims == size);
    assert(strides_to_lpi.size() >= ndims);

    auto data_ds = dsi.list().dataSource("data");
    if(!data_ds->isOpen()) {
      data_ds->open(DataSource::Read);
    }
    // Seek to the beginning of the data
    if(!data_ds->at(_data_offset)) {
      throw read_write_error("MIF reader cannot seek to the data offset");
    }

    if(_scaling) {
      dsi.header()->setProperty( "scale_factor_applied", true );
    }

    std::vector<std::ptrdiff_t> composite_strides;
    std::vector<int> storage_dims;

    std::vector<size_t> storage_order = argsort_abs(_data_layout);
    localMsg(__func__ + std::string(" derived:")
             + "\nstorage_order = " + toString(storage_order));
    std::size_t total_size = 1;
    std::size_t origin_offset = 0;
    for(size_t i = 0; i < ndims; ++i) {
      size_t axis_index = storage_order[i];
      assert(std::abs(_data_layout[axis_index]) == i + 1);
      assert(axis_index < ndims);
      storage_dims.push_back(_dims[axis_index]);
      bool invert_axis;
      if(axis_index < 3) {
        // MIF data layout points to RAS+, whereas AIMS uses LPI+, so the
        // orientation of the first 3 (spatial) axes is inverted
        invert_axis = (_data_layout[axis_index] > 0);
      } else {
        invert_axis = (_data_layout[axis_index] < 0);
      }
      if(invert_axis) {
        origin_offset += strides_to_lpi[axis_index] * (_dims[axis_index] - 1);
        composite_strides.push_back(-strides_to_lpi[axis_index]);
      } else {
        composite_strides.push_back(strides_to_lpi[axis_index]);
      }
      total_size *= _dims[axis_index];
    }

    localMsg(__func__ + std::string(" derived:")
             + "\ntotal_size = " + toString(total_size)
             + "\nstorage_dims = " + toString(storage_dims)
             + "\ncomposite_strides = "+ toString(composite_strides)
             + "\norigin_offset = " + toString(origin_offset) + "\n");

    // The file is read by blocks of a fixed size
    const std::size_t buffer_size = 1048576; // 1 million elements (1024^2)
    std::unique_ptr<DiskT, std::default_delete<DiskT[]>> buffer(new DiskT[buffer_size]);
    std::size_t elements_remaining = total_size;
    carto::NDIterator<T> dest_it(dest + origin_offset, storage_dims,
                                 composite_strides);
    ByteSwapper swapper;
    swapper.setSwapped(_byteswap);

    std::size_t dest_size = std::accumulate(begin(size), end(size), 1, [](auto a, auto b) { return a*b; });
    while(!dest_it.ended()) {
      long bytes_avail = data_ds->readBlock(reinterpret_cast<char*>(buffer.get()),
                                            std::min(buffer_size * sizeof(DiskT),
                                                     elements_remaining * sizeof(DiskT)));
      if(bytes_avail <= 0) {
        throw read_write_error("cannot read enough data (premature end of file?)");
      } else if(bytes_avail % sizeof(DiskT) != 0) {
        throw std::runtime_error("unhandled corner case: readBlock returned a "
                                 "non-multiple of dtype size");
      }
      const DiskT * const buffer_end = reinterpret_cast<DiskT*>(reinterpret_cast<char*>(buffer.get()) + bytes_avail);

      for(DiskT * buf_ptr = buffer.get() ; buf_ptr < buffer_end; ++buf_ptr) {
        T * dest_ptr = &(*dest_it);
        assert(dest_ptr >= dest);
        assert(dest_ptr < dest + dest_size);
        DiskT raw_value = *buf_ptr;
        swapper.reorder(raw_value);
        if(_scaling) {
          *dest_it = apply_scaling<T>(raw_value, _scale_factor, _scale_offset);
        } else {
          *dest_it = raw_value;
        }
        ++dest_it;
      }
      elements_remaining -= (buffer_end - buffer.get());
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
