/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

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

