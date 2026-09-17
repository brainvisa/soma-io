#ifndef SOMAIO_IMAGE_IMAGEREADER_D_H
#define SOMAIO_IMAGE_IMAGEREADER_D_H
//--- soma io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                    // class declaration
#include <soma-io/datasourceinfo/datasourceinfo.h>                   // member
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>       // updateParams(): to access header
#include <cartobase/object/property.h>       // updateParams(): to read header
//--- system -----------------------------------------------------------------
#include <vector>
//----------------------------------------------------------------------------

namespace soma {
  
  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T> 
  void ImageReader<T>::updateParams( DataSourceInfo & /* dsi */ )
  {
  }
  
  template <typename T>
  void ImageReader<T>::resetParams()
  {
  }
  
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  ImageReader<T>::ImageReader()
  {
  }
  
  template <typename T>
  ImageReader<T>::~ImageReader()
  {
  }
  
  //==========================================================================
  //   M E T H O D S
  //==========================================================================
  //--- read(...) ------------------------------------------------------------
  /* This is an abstract method that is defined in format-specific readers
   * (like GisImageReader)
   */
  template <typename T>
  void ImageReader<T>::read( T * /* dest */,
                             DataSourceInfo & dsi,
                             std::vector<int> &  /* pos */,
                             std::vector<int> &  /*size */,
                             std::vector<long> & /* stride */,
                             carto::Object       /* options */ )
  {
    carto::rc_ptr<DataSource> ds = dsi.list().dataSource();
    throw carto::invalid_format_error( "format reader not implemented yet...", 
                                       ds ? ds->url() : "" );
  }


  template <typename T>
  bool ImageReader<T>::open( DataSourceInfo & /* dsi */ )
  {
    return true;
  }


  template <typename T>
  void ImageReader<T>::close( DataSourceInfo & /* dsi */ )
  {
  }


  template <typename T>
  bool ImageReader<T>::isOpen( const DataSourceInfo & /* dsi */ ) const
  {
    return false;
  }

}

#endif
