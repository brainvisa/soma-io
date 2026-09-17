//--- soma io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasourcelist.h>             // class definition
#include <soma-io/datasource/datasource.h>             // to declare rc_ptr of
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                      // to declare vecor of
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "DATASOURCELIST" )
//--- system -----------------------------------------------------------------
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

typedef vector<rc_ptr<DataSource> > vectorDSL;
typedef map<string,vectorDSL>        mapDSL;

//============================================================================
//   C O N S T R U C T O R S
//============================================================================

DataSourceList::DataSourceList()
: _dslist()
{
  _dslist[ "default" ] 
    = vectorDSL( 1, rc_ptr<DataSource>() );
}

DataSourceList::DataSourceList( const rc_ptr<DataSource> & ds, 
                                const string & type )
: _dslist()
{
  _dslist[ type ] = vectorDSL( 1, ds );
  if( type != "default" )
    _dslist[ "default" ] = vectorDSL( 1, ds );
}

DataSourceList::DataSourceList( const DataSourceList & other )
: _dslist( other._dslist )
{
}

DataSourceList::~DataSourceList()
{
}

DataSourceList & DataSourceList::operator = ( const DataSourceList & other )
{
  _dslist = other._dslist;
  
  return *this;
}

//============================================================================
//   A C C E S S O R S
//============================================================================

bool DataSourceList::operator == ( const DataSourceList & other ) const
{
  return ( _dslist == other._dslist );
}

bool DataSourceList::operator != ( const DataSourceList & other ) const
{
  return !( *this == other );
}

set<string> DataSourceList::types() const
{
  set<string> s;
  for( mapDSL::const_iterator it = _dslist.begin();
       it != _dslist.end(); ++it )
  {
    s.insert(it->first);
  }
  return s;
}

bool DataSourceList::empty() const 
{
  return _dslist.empty();
}

int DataSourceList::typecount() const
{
  return _dslist.size();
}

bool DataSourceList::exists( const string & type ) const
{
  return _dslist.count( type ) != 0;
}

bool DataSourceList::empty( const string & type ) const
{
  if( exists( type ) ) {
    return _dslist.find( type )->second.empty();
  } else {
    return true;
  }
}

int DataSourceList::size( const string & type ) const
{
  if( !exists( type ) )
    return 0;
  else
    return _dslist.find( type )->second.size();
}

//============================================================================
//   A C C E S S O R S [ ]
//============================================================================

const rc_ptr<DataSource> & DataSourceList::dataSource ( const string & type,
                                                        int i ) const
{
  if( exists( type )
      && static_cast<size_t>(i) < _dslist.find( type )->second.size() )
    return _dslist.find( type )->second[ i ];
  else
    // launch exception
    throw runtime_error( "datasource type not found" );
}

rc_ptr<DataSource> & DataSourceList::dataSource ( const string & type, int i )
{
  if( exists( type )
      && static_cast<size_t>(i) < _dslist.find( type )->second.size() )
      return _dslist.find( type )->second[ i ];
  else
    // launch exception
    throw runtime_error( "datasource type not found" );

}

//============================================================================
//   M U T A T O R S
//============================================================================

void DataSourceList::addDataSource( const std::string & type, 
                                    const rc_ptr<DataSource> & ds)
{
  if( exists( type ) )
    _dslist[ type ].push_back( ds );
  else
    _dslist[ type ] = vectorDSL( 1, ds );
}

void DataSourceList::reset()
{
  _dslist.clear();
}

#undef localMsg
