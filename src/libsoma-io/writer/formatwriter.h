#ifndef SOMAIO_WRITER_FORMATWRITER_H
#define SOMAIO_WRITER_FORMATWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
#include <cartobase/smart/rcptr.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// Low-level object IO writer specialized for a specific format
  template<typename T>
  class FormatWriter
  {
  public:
    virtual ~FormatWriter() {}
    
    /// This method filters properties for a FormatWriter.
    virtual bool filterProperties(carto::Object properties,
                                  carto::Object options = carto::none()) = 0;
      
    virtual bool write( const T & obj, 
                        carto::rc_ptr<DataSourceInfo> dsi,
                        carto::Object options ) = 0;
  };

}

#endif
