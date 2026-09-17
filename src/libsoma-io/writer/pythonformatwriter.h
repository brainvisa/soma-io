#ifndef SOMAIO_WRITER_PYTHONFORMATWRITER_H
#define SOMAIO_WRITER_PYTHONFORMATWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/writer/formatwriter.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{

  class PythonFormatWriter : public FormatWriter<carto::GenericObject>
  {
  public:
    virtual bool filterProperties(carto::Object properties,
                                  carto::Object options = carto::none());
    
    virtual bool write( const carto::GenericObject & obj, 
                        carto::rc_ptr<DataSourceInfo> dsi,
                        carto::Object options );
  };

}

#endif

