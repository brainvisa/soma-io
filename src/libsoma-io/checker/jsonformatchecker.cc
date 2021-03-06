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

//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/jsonformatchecker.h>          // class declaration
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
#define localMsg( message ) cartoCondMsg( 4, message, "JSONFORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

namespace
{
  bool initMe()
  {
    vector<string>      exts;
    exts.push_back( "json" );
    DataSourceInfoLoader::registerFormat( "JSON",
                                          new JsonFormatChecker,
                                          exts );

    return true;
  }

  bool  _dummy __attribute__((unused)) = initMe();
}


DataSourceInfo JsonFormatChecker::check( DataSourceInfo dsi,
                                         DataSourceInfoLoader &,
                                         Object /* options */ ) const
{

  // FIXME: very very slight checking...
  static const string   sign1 = "{";
  static const string   sign2 = "[";
  const string* sign = &sign1;
  DataSource *ds = dsi.list().dataSource().get();
  ds->open( DataSource::Read );
  char  c;
  int   i, n = sign->length();
  string readsign;

  localMsg( "Reading json beginning... " + ds->url() );
  for( i=0; i<n && ds->isOpen(); ++i )
  {
    c = static_cast<char>(ds->getch());
    readsign += c;
    if( c != ' ' && c != '\t' && c != '\n' )
    {
      if( (*sign)[i] != c )
      {
        if( i == 0 && sign == &sign1 && sign2[i] == c )
        {
          sign = &sign2;
          n = sign->length();
        }
        else
          break;
      }
    }
  }
  if( ds->isOpen() )
  {
    // rewind
    for( int j=readsign.length()-1; j>=0; --j )
      ds->ungetch( readsign[j] );
  }
  if( i != n )
  {
    if( !ds->isOpen() )
      io_error::launchErrnoExcept( ds->url() );
    throw wrong_format_error( "not a JSON file", ds->url() );
  }

  Object hdr = Object::value( PropertySet() );
  hdr->setProperty( "format", string( "JSON" ) );
  hdr->setProperty( "object_type", string( "genericobject" ) );
  hdr->setProperty( "data_type", string( "any" ) );

  // add header to datasourceinfo
  dsi.header() = hdr;

  localMsg( "Building list... " + ds->url() );
  dsi.list().addDataSource( "minf", rc_ptr<DataSource>( ds ) );

  localMsg( "Writing capabilities... " + ds->url() );
  dsi.capabilities().setMemoryMapping( false );
  dsi.capabilities().setThreadSafe( false );  // TODO
  dsi.capabilities().setOrdered( false );
  dsi.capabilities().setSeekVoxel( false );
  dsi.capabilities().setSeekLine( false );
  dsi.capabilities().setSeekSlice( false );
  dsi.capabilities().setSeekVolume( false );

  localMsg( "Checking done " + ds->url() );
  return dsi;
}

#undef localMsg
