#ifndef SOMAIO_CHECKER_GISFORMATCHECKER_H
#define SOMAIO_CHECKER_GISFORMATCHECKER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/formatchecker.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;
  class DataSourceList;
  class DataSourceInfoLoader;
  class DataSourceInfo;

  /// FormatChecker for GIS files.
  class GisFormatChecker : public FormatChecker
  {
    public:
      virtual DataSourceInfo check( DataSourceInfo dsi, 
                                    DataSourceInfoLoader & f,
                                    carto::Object options = carto::none() )
                                    const;
      virtual ~GisFormatChecker();

    protected:
      void  _buildDSList( DataSourceList & dsl ) const;
      carto::Object   _buildHeader( DataSource * hds ) const;
  };

}

#endif

