//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/writer_d.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasource/streamdatasource.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
//----------------------------------------------------------------------------

using namespace carto;
using namespace std;

namespace soma {

GenericWriter::GenericWriter()
  : _datasourceinfo( new DataSourceInfo )
{
}


GenericWriter::GenericWriter( rc_ptr<DataSource> ds )
  : _datasourceinfo( new DataSourceInfo( ds ) )
{
}


GenericWriter::GenericWriter( const string & filename )
  : _datasourceinfo ( new DataSourceInfo
                      ( rc_ptr<DataSource>
                        ( new FileDataSource( filename ) )
                      )
                    )
{
}


GenericWriter::GenericWriter( ostream & stream )
  : _datasourceinfo ( new DataSourceInfo
                      ( rc_ptr<DataSource>
                        ( new OStreamDataSource( stream ) )
                      )
                    )
{
}


GenericWriter::~GenericWriter()
{
}


void GenericWriter::flush()
{
  if( _datasourceinfo->list().dataSource() )
    _datasourceinfo->list().dataSource()->flush();
}


void GenericWriter::close()
{
  if( _datasourceinfo->list().dataSource() )
    _datasourceinfo->list().dataSource( )->close();
}


const rc_ptr<DataSource> GenericWriter::dataSource() const
{
  return _datasourceinfo->list().dataSource();
}


rc_ptr<DataSource> GenericWriter::dataSource()
{
  return _datasourceinfo->list().dataSource();
}


void GenericWriter::attach( rc_ptr<DataSource> ds )
{
  _datasourceinfo.reset( new DataSourceInfo( ds ) );
}


void GenericWriter::attach( const std::string & filename )
{
  _datasourceinfo.reset ( new DataSourceInfo
                          ( rc_ptr<DataSource>( new FileDataSource( filename ) ) )
                        );
}


void GenericWriter::attach( ostream & stream )
{
  _datasourceinfo.reset ( new DataSourceInfo
                          ( rc_ptr<DataSource>( new OStreamDataSource( stream ) ) )
                        );
}


template class Writer<GenericObject>;
template class Writer<Object>;

} // namespace soma
