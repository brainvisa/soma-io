#ifndef SOMAIO_DATASOURCE_CHAINDATASOURCE_H
#define SOMAIO_DATASOURCE_CHAINDATASOURCE_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>                       // base class
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                          // member: _source
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

namespace soma
{

  /// Abstract base DataSource class to operate on another DataSource
  /// (to transform it)
  class ChainDataSource : public DataSource
  {
  public:
    ChainDataSource( carto::rc_ptr<DataSource> ds, 
                     const std::string & url = std::string() );
    virtual ~ChainDataSource();

    void setSource( carto::rc_ptr<DataSource> ds, 
                    const std::string & url = std::string() );
          carto::rc_ptr<DataSource> source();
    const carto::rc_ptr<DataSource> source() const;
    virtual bool isOpen() const;
    virtual bool open( int mode );
    virtual void close();
    virtual std::string url() const;

  protected:
    carto::rc_ptr<DataSource>	_source;

  private:
    std::string	_url;
  };

}

#endif
