#ifndef SOMAIO_CHECKER_MINCFORMATCHECKER_H
#define SOMAIO_CHECKER_MINCFORMATCHECKER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/formatchecker.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------


namespace carto
{
  class Mutex;
}


namespace soma
{
  class DataSource;
  class DataSourceList;
  class DataSourceInfoLoader;
  class DataSourceInfo;

  /// FormatChecker for MINC files.
  class MincFormatChecker : public FormatChecker
  {
    public:
      virtual DataSourceInfo check( DataSourceInfo dsi, 
                                    DataSourceInfoLoader & f,
                                    carto::Object options = carto::none() )
                                    const;
      virtual ~MincFormatChecker();

      /// mute output function
      static void my_empty_print_error(char *message);
      /// lock to avoid non-thread-safe operations
      static carto::Mutex & mincMutex();

  protected:
      void  _buildDSList( DataSourceList & dsl ) const;
      carto::Object _buildHeader( DataSource * hds ) const;
      carto::Object _buildMinc2Header( DataSource * hds,
                                       void *handle ) const;

      int readMincAttribute( carto::Object hdr,
                             carto::Syntax &sx, int mincid,
                             std::string minc_var, std::string minc_att,
                             std::string aims_att_name ) const;
      int readMincHistory( carto::Object hdr, const std::string & fname,
                           int mincid ) const;
  };

}

#endif

