#include <soma-io/plugingis/gisformatchecker.h>
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
    string  fname;
    int     test = 1;
    CartoApplication  app( argc, argv, "Test for GIS format checking" );
    app.addOption( fname, "-i", "input filename to be read\n" );
    app.alias( "-v", "--verbose" );
    app.initialize();
    
    rc_ptr<DataSource> ds ( new FileDataSource( fname ) );
    DataSourceInfo dsi( ds );
    DataSourceInfoLoader dsil;
    dsi = dsil.check( dsi );
    
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
    // tert minf
    if (dsi.header()->hasProperty( "patient_id" ) )
      cout << "patient_id: \t" << dsi.header()->getProperty( "patient_id" )->getString() << endl;
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
    for( it; it != types.end(); ++it ) {
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
