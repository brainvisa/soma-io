#ifndef SOMAIO_CHECKER_TIFFFORMATCHECKER_H
#define SOMAIO_CHECKER_TIFFFORMATCHECKER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/formatchecker.h>
#include <soma-io/utilities/multifileformat.h>     // to read datasource
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;
  class DataSourceList;
  class DataSourceInfoLoader;
  class DataSourceInfo;

  /// FormatChecker for TIFF files.
  class TiffFormatChecker : public FormatChecker
  {
    public:
      virtual DataSourceInfo check( DataSourceInfo dsi, 
                                    DataSourceInfoLoader & f,
                                    carto::Object options = carto::none() )
                                    const;
      virtual ~TiffFormatChecker();

    protected:
      std::string _getTiffFileName(const std::string filename) const;
      std::string _getMinfFileName(const std::string filename) const;
      void _buildMultiFileFormatInfo( DataSourceInfo & dsi ) const;
      void  _buildDSList( DataSourceList & dsl, const soma::MultiFileFormatInfo & info ) const;
      carto::Object   _buildHeader( DataSource * hds, const soma::MultiFileFormatInfo & info ) const;
  };
  
}

#endif

