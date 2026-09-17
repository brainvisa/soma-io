#ifndef SOMAIO_CHECKER_DICOMFORMATCHECKER_H
#define SOMAIO_CHECKER_DICOMFORMATCHECKER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/formatchecker.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;
  class DataSourceInfoLoader;
  class DataSourceInfo;

  /// FormatChecker for DICOM files.
  class DicomFormatChecker : public FormatChecker
  {
    public:
      virtual DataSourceInfo check( DataSourceInfo dsi, 
                                    DataSourceInfoLoader & f,
                                    carto::Object options = carto::none() )
                                    const;
      virtual ~DicomFormatChecker();

    protected:
      carto::Object  _buildDSList( DataSourceInfo & dsi ) const;
  };

}

#endif

