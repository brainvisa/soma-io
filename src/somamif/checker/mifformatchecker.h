#ifndef SOMAIO_CHECKER_MIFFORMATCHECKER_H
#define SOMAIO_CHECKER_MIFFORMATCHECKER_H
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

  /// FormatChecker for MIF files.
  class MifFormatChecker : public FormatChecker
  {
  public:
    MifFormatChecker();
    DataSourceInfo check( DataSourceInfo dsi,
                          DataSourceInfoLoader & f,
                          carto::Object options = carto::none() )
      const override;
    virtual ~MifFormatChecker();

  protected:
    void _buildDSList( DataSourceList & dsl ) const;
    carto::Object _buildHeader( DataSource & hds ) const;
    std::string formatName() const { return "MIF"; }
  };

}

#endif
