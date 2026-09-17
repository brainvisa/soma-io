#ifndef SOMAIO_READER_PYTHONFORMATREADER_H
#define SOMAIO_READER_PYTHONFORMATREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/reader/formatreader.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace carto {
  class GenericObject;
}

namespace soma
{
  
  class PythonFormatReader : public FormatReader<carto::GenericObject>
  {
  public:
    //========================================================================
    //   N E W   M E T H O D S
    //========================================================================
    virtual carto::GenericObject* 
    createAndRead( carto::rc_ptr<DataSourceInfo> dsi,
                   const AllocatorContext & context, 
                   carto::Object options );
    virtual void read( carto::GenericObject & obj, 
                       carto::rc_ptr<DataSourceInfo> dsi, 
                       const AllocatorContext & context, 
                       carto::Object options );
    virtual FormatReader<carto::GenericObject>* clone() const;
    virtual std::string formatID() const { return "PYTHON"; }

    //========================================================================
    //   O L D   M E T H O D S
    //========================================================================
    virtual carto::GenericObject* 
    createAndRead( carto::Object header, carto::rc_ptr<DataSource> ds,
                   const AllocatorContext & context, carto::Object options );
    virtual void read( carto::GenericObject & obj,
                       carto::Object header,
                       const AllocatorContext & context,
                       carto::Object options );
  };

}

#endif
