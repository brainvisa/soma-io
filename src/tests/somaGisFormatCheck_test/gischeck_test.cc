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

#include <soma-io/checker/gisformatchecker.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <cartobase/smart/rcptr.h>
#include <cartobase/exception/assert.h>
#include <cartobase/getopt/getopt.h>
#include <cartobase/config/verbose.h>
#include <string>
#include <iostream>
#include <set>

using namespace soma;
using namespace carto;
using namespace std;

int main( int argc, const char** argv )
{
  try {
    string  fname, hname;
    int     test = 1;
    CartoApplication  app( argc, argv, "Test for GIS format checking" );
    app.addOption( fname, "-i", "input filename to be read\n" );
    app.addOption( hname, "-j", "input filename to be read (if 2 passes)\n", true );
    app.addOption( test, "-t", "test to perform. 1) normal reading. "
                              "2) two pass reading with complete dsi. "
                              "3) two pass reading with juste header\n", true );
    app.alias( "-v", "--verbose" );
    app.initialize();
    
    // Get gis FormatChecker
    FormatChecker * fc = DataSourceInfoLoader::formatInfo( "GIS" );
    ASSERT(fc);
    
    if( hname.empty() )
      hname = fname;
    
    rc_ptr<DataSource> ds ( new FileDataSource( fname ) );
    DataSourceInfo dsi( ds );
    DataSourceInfoLoader dsil;
    dsi = fc->check( dsi, dsil );
    
    if( test == 2) {
      dsi = fc->check( dsi, dsil );
    } else if( test == 3 ) {
      dsi.list().reset();
      dsi.list().addDataSource( "default", 
                                rc_ptr<DataSource>( new FileDataSource( hname ) ) 
                              );
      dsi.capabilities().reset();
      //dsi.header()->setProperty( "format" , "JPEG" );
      dsi = fc->check( dsi, dsil );
    }
    
    //--- Write Header -----------------------------------------------------------
    cout << "//---------------------------------------------------------" << endl;
    cout << "//   H E A D E R                                           " << endl;
    cout << "//---------------------------------------------------------" << endl;
    if( dsi.header()->hasProperty( "sizeX" ) )
      cout << "sizeX: \t" << dsi.header()->getProperty( "sizeX" )->getString() << endl;
    if( dsi.header()->hasProperty( "sizeY" ) )
      cout << "sizeY: \t" << dsi.header()->getProperty( "sizeY" )->getString() << endl;
    if( dsi.header()->hasProperty( "sizeZ" ) )
      cout << "sizeZ: \t" << dsi.header()->getProperty( "sizeZ" )->getString() << endl;
    if( dsi.header()->hasProperty( "sizeT" ) )
      cout << "sizeT: \t" << dsi.header()->getProperty( "sizeT" )->getString() << endl;
    if( dsi.header()->hasProperty( "format" ) )
      cout << "format: \t" << dsi.header()->getProperty( "format" )->getString() << endl;
    if( dsi.header()->hasProperty( "voxel_size" ) )
      cout << "voxel_size: \t( " << dsi.header()->getProperty( "voxel_size" )->getArrayItem(0)->getScalar() << ", "
                                << dsi.header()->getProperty( "voxel_size" )->getArrayItem(1)->getScalar() << ", "
                                << dsi.header()->getProperty( "voxel_size" )->getArrayItem(2)->getScalar() << " )"
                                << endl;
    if( dsi.header()->hasProperty( "object_type" ) )
      cout << "object_type: \t" << dsi.header()->getProperty( "object_type" )->getString() << endl;
    if( dsi.header()->hasProperty( "ascii" ) )
      cout << "ascii: \t" << dsi.header()->getProperty( "ascii" )->getString() << endl;
    if( dsi.header()->hasProperty( "byte_swapping" ) )
      cout << "byte_swapping: \t" << dsi.header()->getProperty( "byte_swapping" )->getString() << endl;
    //--- Write Capabilities -----------------------------------------------------
    cout << "//---------------------------------------------------------" << endl;
    cout << "//   C A P A B I L I T I E S                               " << endl;
    cout << "//---------------------------------------------------------" << endl;
    cout << "isInit: \t"              << dsi.capabilities().isInit()              << endl;
    cout << "allowsMemoryMapping: \t" << dsi.capabilities().allowsMemoryMapping() << endl;
    cout << "isThreadSafe: \t"        << dsi.capabilities().isThreadSafe()        << endl;
    cout << "isOrdered: \t"           << dsi.capabilities().isOrdered()           << endl;
    cout << "canSeekVoxel: \t"        << dsi.capabilities().canSeekVoxel()        << endl;
    cout << "canSeekLine: \t"         << dsi.capabilities().canSeekLine()         << endl;
    cout << "canSeekSlice: \t"        << dsi.capabilities().canSeekSlice()        << endl;
    cout << "canSeekVolume: \t"       << dsi.capabilities().canSeekVolume()       << endl;
    //--- Write DSList -----------------------------------------------------------
    cout << "//---------------------------------------------------------" << endl;
    cout << "//   D A T A   S O U R C E   L I S T                       " << endl;
    cout << "//---------------------------------------------------------" << endl;
    set<string> types = dsi.list().types();
    set<string>::iterator it = types.begin();
    for( ; it != types.end(); ++it ) {
      int size = dsi.list().size( *it );
      int i;
      for ( i = 0; i< size; ++i ) {
        cout << "DS " << *it << " : \t" << dsi.list().dataSource( *it, i )->url() << endl;
      }
    }
  } catch( user_interruption & ) {
    return EXIT_SUCCESS;
  } catch( exception & e ) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
  
}
