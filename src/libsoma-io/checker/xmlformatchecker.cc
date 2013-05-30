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
#include <soma-io/checker/xmlformatchecker.h>               // class declaration
#include <soma-io/datasourceinfo/datasourceinfoloader.h>         // function arg
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function return value
#include <soma-io/datasourceinfo/datasourcecapabilities.h>       // to build dsi
#include <soma-io/datasource/datasourcelist.h>                   // function arg
#include <soma-io/datasource/datasource.h>                     // actual reading
//--- cartobase ----------------------------------------------------------------
#include <cartobase/object/object.h>                             // function arg
#include <cartobase/object/property.h>        // to manipulate header properties
//--- system -------------------------------------------------------------------
// #define SOMA_IO_DEBUG
#ifdef SOMA_IO_DEBUG
  #include <iostream>
#endif
#include <vector>
#include <string>
//------------------------------------------------------------------------------

// TODO
// build datasourcelist (check if ok)
// build capabilities

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

  bool	_dummy = initMe();
}


DataSourceInfo XMLFormatChecker::check( DataSourceInfo dsi, 
                                        DataSourceInfoLoader &,
                                        Object /* options */ ) const
{
  DataSource *ds = dsi.list().dataSource( "default", 0 ).get();
  ds->open( DataSource::Read );
  static const string	sign = "<?xml";
  char		c;
  int		i, n = sign.length();
  
  #ifdef SOMA_IO_DEBUG
    cout << "XMLFORMATCHECKER:: Reading minf header..." << ds->url() << endl;
  #endif
  for( i=0; i<n && ds->isOpen() && sign[i] == (c=(char)ds->getch()); ++i ) {}
  if( ds->isOpen() )
    {
      // rewind
      int	j = i;
      if( i == n )
        --j;
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
  
  #ifdef SOMA_IO_DEBUG
    cout << "XMLFORMATCHECKER:: Building list..." << ds->url() << endl;
  #endif
  dsi.list().addDataSource( "minf", rc_ptr<DataSource>( ds ) );
  
  #ifdef SOMA_IO_DEBUG
    cout << "XMLFORMATCHECKER:: Writing capabilities..." << ds->url() << endl;
  #endif
  dsi.capabilities().setMemoryMapping( false );
  dsi.capabilities().setThreadSafe( false ); /* TODO */
  dsi.capabilities().setOrdered( false );
  dsi.capabilities().setSeekVoxel( false );
  dsi.capabilities().setSeekLine( false );
  dsi.capabilities().setSeekSlice( false );
  dsi.capabilities().setSeekVolume( false );
  
  #ifdef SOMA_IO_DEBUG
    cout << "XMLFORMATCHECKER:: Checking done " << ds->url() << endl;
  #endif
  return dsi;
}

