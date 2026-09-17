#ifndef SOMAIO_UTILITIES_MINFUTIL_H
#define SOMAIO_UTILITIES_MINFUTIL_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{

  /// Meta information utilities
  ///
  class MinfUtil
  {
  public:
    virtual ~MinfUtil() {}

    /// \c header is filtered by checking existing properties.
    /// Properties 'uuid' and 'resolutions_dimension' are removed
    /// unless options specify to keep them. \n
    /// \n
    /// Available options: \n
    /// "remove_uuid", if set to false, "uuid" property is kept (default is true) \n
    /// "remove_resolutions_dimension", if set to false, "resolutions_dimension" 
    ///                             property is kept (default is true) 
    static void filter(carto::Object header,
                       carto::Object options = carto::none());
    

    /// \c header is updated from a data source.
    /// Properties 'uuid' is updated unless options specify to skip it. \n
    /// \n
    /// Available options: \n
    /// "update_uuid", if set to false, "uuid" property update from \c source is skipped (default is true) \n
    static void updateFromSource(soma::DataSource * source,
                                 carto::Object header,
                                 carto::Object options = carto::none());
  };

}

#endif

