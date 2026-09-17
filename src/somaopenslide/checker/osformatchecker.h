#ifndef SOMAIO_CHECKER_OSFORMATCHECKER_H
#define SOMAIO_CHECKER_OSFORMATCHECKER_H
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

  /// FormatChecker for virtual microscopy images using OpenSlide
  class OSFormatChecker : public FormatChecker
  {
    public:
      virtual DataSourceInfo check( DataSourceInfo dsi, 
                                    DataSourceInfoLoader & f,
                                    carto::Object options = carto::none() )
                                    const;
      virtual ~OSFormatChecker();

    protected:
      void            _buildDSList( DataSourceList & dsl ) const;
      carto::Object   _buildHeader( DataSource * hds, carto::Object options ) const;
  };

}

#endif

