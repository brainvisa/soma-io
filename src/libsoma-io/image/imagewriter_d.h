#ifndef SOMAIO_IMAGE_IMAGEWRITER_D_H
#define SOMAIO_IMAGE_IMAGEWRITER_D_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                    // class declaration
#include <soma-io/datasourceinfo/datasourceinfo.h>                   // member
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>
#include <cartobase/object/object.h>
//--- system -----------------------------------------------------------------
//----------------------------------------------------------------------------

namespace soma
{
  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T> 
  void ImageWriter<T>::updateParams( DataSourceInfo & /* dsi */ )
  {
  }
  
  template <typename T>
  void ImageWriter<T>::resetParams()
  {
  }
  
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  ImageWriter<T>::ImageWriter()
  {
  }
  
  template <typename T>
  ImageWriter<T>::~ImageWriter()
  {
  }
  
  //==========================================================================
  //   M E T H O D S
  //==========================================================================
  /*** write(...) ************************************************************
   * This is an abstract method that is defined in format-specific writers
   * (like GisImageWriter)
   **************************************************************************/
  template <typename T>
  void ImageWriter<T>::write( const T * /*source*/, 
                              DataSourceInfo & dsi,
                              const std::vector<int> & /*pos*/,
                              const std::vector<int> & /*size*/,
                              const std::vector<long> & /*strides*/,
                              carto::Object /*options*/ )
  {
    carto::rc_ptr<DataSource> ds = dsi.list().dataSource();
    throw carto::invalid_format_error( "format reader not implemented yet...", 
                                       ds ? ds->url() : "" );
  }

  template <typename T>
  DataSourceInfo ImageWriter<T>::writeHeader(
    DataSourceInfo & dsi,
    const T * /* source */,
    const std::vector<int> & /* pos */,
    const std::vector<int> & /* size */,
    const std::vector<long> & /* strides */,
    carto::Object /*options*/ )
  {
    carto::rc_ptr<DataSource> ds = dsi.list().dataSource();
    throw carto::invalid_format_error( "format reader not implemented yet...", 
                                       ds ? ds->url() : "" );
  }
}

#endif
