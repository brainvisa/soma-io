//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasourcelist.h>
#include <soma-io/datasource/filedatasource.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/smart/rcptr.h>
#include <cartobase/exception/assert.h>
//--- system -------------------------------------------------------------------
#include <set>
#include <string>
//------------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

int main()
{
  DataSourceList dsl;
  dsl = DataSourceList( rc_ptr<DataSource>( new FileDataSource( "filename.dim" ) ) );
  ASSERT( dsl.dataSource( "default", 0 ).get() );
  dsl.addDataSource( "newkey", DataSource::none() );
  ASSERT( !dsl.empty() );
  ASSERT( dsl.typecount() == 2 );
  ASSERT( dsl.exists( "default" ) );
  ASSERT( dsl.exists( "newkey" ) );
  ASSERT( !dsl.exists( "" ) );
  ASSERT( !dsl.empty( "default" ) );
  ASSERT( !dsl.empty( "newkey" ) );
  ASSERT( dsl.size( "default" ) == 1 );
  ASSERT( dsl.size( "newkey" ) == 1);
  dsl.addDataSource( "newkey", rc_ptr<DataSource>( new FileDataSource( "otherfilename.dim" ) ) );
  ASSERT( dsl.size( "newkey") == 2 );
  DataSourceList dsl2( dsl );
  ASSERT( dsl2 == dsl );
  dsl2.reset();
  ASSERT( dsl2 != dsl );
  ASSERT( dsl2.empty() );
  set<string> types = dsl.types();
  ASSERT( types.size() == 2 );
  ASSERT( *types.begin() == "default" );
  ASSERT( *(++types.begin()) == "newkey" );
}
