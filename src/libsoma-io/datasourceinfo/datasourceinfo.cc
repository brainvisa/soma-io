//--- soma io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>        // class declaration
#include <soma-io/datasourceinfo/datasourcecapabilities.h>           // member
#include <soma-io/datasource/datasourcelist.h>                       // member
#include <soma-io/datasource/datasource.h>           // constructor's argument
#include <soma-io/datasource/filedatasource.h> // used with string constructor
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // member
#include <cartobase/object/property.h>          // to manage header properties
#include <cartobase/smart/rcptr.h>                      // used by constructor
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   C O N S T R U C T O R S
//============================================================================
DataSourceInfo::DataSourceInfo( const Object & header, 
                                const DataSourceCapabilities & cap,
                                const DataSourceList & dsl )
: _header( header ), _capabilities( cap ), _datasourcelist( dsl ),
  _privateiodata( Object::value( PropertySet() ) )
{
}

DataSourceInfo::DataSourceInfo( const rc_ptr<DataSource> & ds,
                                const std::vector<int> & dim )
: _header( none() ), _capabilities(), _datasourcelist( ds ),
  _privateiodata( Object::value( PropertySet() ) )
{
  if( !dim.empty() ) {
    _header = Object::value( PropertySet() );
    _header->setProperty( "sizeX", dim[ 0 ] );
    _header->setProperty( "sizeY", dim[ 1 ] );
    _header->setProperty( "sizeZ", dim[ 2 ] );
    _header->setProperty( "sizeT", dim[ 3 ] );
  }
}

DataSourceInfo::DataSourceInfo( const rc_ptr<DataSource> & ds, 
                                Object header )
: _header( header ), _capabilities(), _datasourcelist( ds ),
  _privateiodata( Object::value( PropertySet() ) )
{
}

DataSourceInfo::DataSourceInfo( const DataSourceInfo & other )
: _header( other._header ), 
  _capabilities( other._capabilities ), 
  _datasourcelist( other._datasourcelist ),
  _privateiodata( other._privateiodata ),
  _identifiedFormat( other._identifiedFormat ),
  _identifiedFileExtension( other._identifiedFileExtension )
{
}

DataSourceInfo::DataSourceInfo( const string & fname )
: _header( none() ), _capabilities(),
  _datasourcelist( carto::rc_ptr<DataSource>( new FileDataSource( fname ) ) ),
  _privateiodata( Object::value( PropertySet() ) )
{
}

DataSourceInfo::~DataSourceInfo()
{
}

//============================================================================
//   A C C E S S   T O   M E M B E R S
//============================================================================

// already defined in header
