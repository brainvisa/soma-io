//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/xmlformatchecker.h>             // class declaration
#include <soma-io/datasourceinfo/datasourceinfoloader.h>       // function arg
#include <soma-io/datasourceinfo/datasourceinfo.h>    // function return value
#include <soma-io/datasourceinfo/datasourcecapabilities.h>     // to build dsi
#include <soma-io/datasource/datasourcelist.h>                 // function arg
#include <soma-io/datasource/datasource.h>                   // actual reading
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                           // function arg
#include <cartobase/object/property.h>      // to manipulate header properties
//--- system -----------------------------------------------------------------
#include <vector>
#include <string>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "XMLFORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

namespace
{
  bool initMe()
  {
    vector<string>	exts;
    exts.push_back( "minf" );
    exts.push_back( "xml" );
    DataSourceInfoLoader::registerFormat( "XML", new XMLFormatChecker, exts );

    return true;
  }

  bool	_dummy __attribute__((unused)) = initMe();
}


DataSourceInfo XMLFormatChecker::check( DataSourceInfo dsi, 
                                        DataSourceInfoLoader &,
                                        Object /* options */ ) const
{
  DataSource *ds = dsi.list().dataSource().get();
  ds->open( DataSource::Read );
  static const string	sign = "<?xml";
  char		c = 0;
  int		i, n = sign.length();
  
  localMsg( "Reading minf header... " + ds->url() );
  for( i=0; i<n && ds->isOpen() && sign[i] == (c=static_cast<char>(ds->getch())); ++i ) {}
  if( ds->isOpen() )
  {
    // rewind
    int	j = i - 1;
    if( i != n )
      ds->ungetch( c );
    for( ; j>=0; --j )
      ds->ungetch( sign[j] );
  }
  if( i != n )
  {
    if( !ds->isOpen() )
      io_error::launchErrnoExcept( ds->url() );
    throw wrong_format_error( "not a XML MINF file", ds->url() );
  }

  Object hdr = Object::value( PropertySet() );
  hdr->setProperty( "format", string( "XML" ) );
  hdr->setProperty( "object_type", string( "genericobject" ) );
  hdr->setProperty( "data_type", string( "any" ) );
  
  // add header to datasourceinfo
  dsi.header() = hdr;
  
  localMsg( "Building list... " + ds->url() );
  dsi.list().addDataSource( "minf", rc_ptr<DataSource>( ds ) );
  
  localMsg( "Writing capabilities... " + ds->url() );
  dsi.capabilities().setMemoryMapping( false );
  dsi.capabilities().setThreadSafe( false ); /* TODO */
  dsi.capabilities().setOrdered( false );
  dsi.capabilities().setSeekVoxel( false );
  dsi.capabilities().setSeekLine( false );
  dsi.capabilities().setSeekSlice( false );
  dsi.capabilities().setSeekVolume( false );
  
  localMsg( "Checking done " + ds->url() );
  return dsi;
}

#undef localMsg
