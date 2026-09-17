//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/readeralgorithm.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasource/filedatasource.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/exception/format.h>
//--- system -------------------------------------------------------------------
#include <iostream>
//------------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;


ReaderAlgorithm::ReaderAlgorithm( const string & name ) : Algorithm( name )
{
}


ReaderAlgorithm::~ReaderAlgorithm()
{
}


void ReaderAlgorithm::registerAlgorithmType( const string & objType, 
                                             ProcFunc procFunc )
{
  _execs[ objType ] = procFunc;
}


bool ReaderAlgorithm::execute( const string & filename )
{
  return execute( rc_ptr<DataSourceInfo>
                  ( new DataSourceInfo( rc_ptr<DataSource>
                    ( new FileDataSource( filename ) ) 
                  ) )
                );
}


bool ReaderAlgorithm::execute( rc_ptr<DataSource> source )
{
  rc_ptr<DataSourceInfo> dsi( new DataSourceInfo( source ) );
  return execute( dsi );
}


bool ReaderAlgorithm::execute( Object header, rc_ptr<DataSource> source )
{
  rc_ptr<DataSourceInfo> dsi( new DataSourceInfo( source, header ) );
  return execute( dsi );
}


bool ReaderAlgorithm::execute( rc_ptr<DataSourceInfo> dsi )
{
  // If no header
  if( dsi->header().isNone() ) {
    DataSourceInfoLoader  f;
    DataSourceInfo ndsi =  f.check( *dsi );
    if( ndsi.header().isNone() || ndsi.list().empty() ) {
      f.launchException();
      // throw format_error( source->url() );
      return false;
    }
    *dsi = ndsi;
    return execute( dsi );
  }
  
  string  otype;

  dsi->header()->getProperty( "object_type", otype );

  map<string, ProcFunc>::const_iterator ip = _execs.find( otype );
  if( ip == _execs.end() )
  {
    // Try alternate data types
    vector<string>  posstypes;
    try
    {
      dsi->header()->getProperty( "possible_types", posstypes );
    }
    catch( ... )
    {
    }

    // try AIMS style object_type/data_type and possible_data_types
    string dtype;
    if( dsi->header()->getProperty( "data_type", dtype ) )
      posstypes.push_back( otype + " of " + dtype );

    vector<string> possdtypes;
    if( dsi->header()->getProperty( "possible_data_types", possdtypes ) )
    {
      vector<string>::const_iterator idt, edt = possdtypes.end();
      for( idt=possdtypes.begin(); idt!=edt; ++idt )
        posstypes.push_back( otype + " of " + *idt );
    }

    unsigned    i, n = posstypes.size();

    for( i=0; i<n; ++i )
      if( posstypes[i] != otype )
      {
        ip = _execs.find( posstypes[i] );
        if( ip != _execs.end() )
        {
          // force new datatype into header
          dsi->header()->setProperty( "object_type", posstypes[i] );
          if( dsi->header()->hasProperty( "data_type" ) )
            dsi->header()->removeProperty( "data_type" );
          break;
        }
      }
      if( i == n )
      {
      throw datatype_format_error( string( "unsupported object type " )
                                     + otype, dsi->list().dataSource()->url()
                                   );
      return false;
    }
  }

  //  execute algo function
  return ip->second( *this, dsi->header(), dsi->list().dataSource() );
}

