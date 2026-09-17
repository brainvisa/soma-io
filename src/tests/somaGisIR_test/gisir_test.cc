#include <soma-io/plugingis/gisformatchecker.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <cartobase/smart/rcptr.h>
#include <soma-io/plugingis/gisimagereader.h>
#include <soma-io/plugingis/gisimagereader_d.h>

#include <cartobase/exception/assert.h>
#include <cartobase/getopt/getopt.h>
#include <cartobase/config/verbose.h>
#include <string>
#include <iostream>
#include <vector>

using namespace soma;
using namespace carto;
using namespace std;

int main( int argc, const char** argv )
{
  try {
    string  fname;
    vector<int> origin( 4, 0 );
    vector<int> frame( 4, 0 );
    int     test = 1;
    CartoApplication  app( argc, argv, "Test for GIS format checking" );
    app.addOption( fname, "-i", "input filename to be read\n" );
    app.addOption( origin[ 0 ], "-ox", "frame origin (x comp)\n", true );
    app.addOption( origin[ 1 ], "-oy", "frame origin (x comp)\n", true );
    app.addOption( origin[ 2 ], "-oz", "frame origin (x comp)\n", true );
    app.addOption( origin[ 3 ], "-ot", "frame origin (x comp)\n", true );
    app.addOption( frame[ 0 ], "-sx", "frame size (x comp)\n", true );
    app.addOption( frame[ 1 ], "-sy", "frame size (x comp)\n", true );
    app.addOption( frame[ 2 ], "-sz", "frame size (x comp)\n", true );
    app.addOption( frame[ 3 ], "-st", "frame size (x comp)\n", true );
    app.alias( "-v", "--verbose" );
    app.initialize();
    
    /*
    cout << "//---------------------------------------------------------" << endl;
    cout << "//   A R G U M E N T S                                     " << endl;
    cout << "//---------------------------------------------------------" << endl;
    cout << "Origin : " << origin[ 0 ] << ", "
                        << origin[ 1 ] << ", "
                        << origin[ 2 ] << ", "
                        << origin[ 3 ] << endl;
    cout << "Frame : " << frame[ 0 ] << ", "
                       << frame[ 1 ] << ", "
                       << frame[ 2 ] << ", "
                       << frame[ 3 ] << endl;
    */
    
    rc_ptr<DataSource> ds ( new FileDataSource( fname ) );
    DataSourceInfo dsi( ds );
    GisFormatChecker gfc;
    DataSourceInfoLoader dsil;
    dsi = gfc.check( dsi, dsil );
    
    vector<int> size( 4, 0 );
    dsi.header()->getProperty( "sizeX", size[ 0 ] );
    dsi.header()->getProperty( "sizeY", size[ 1 ] );
    dsi.header()->getProperty( "sizeZ", size[ 2 ] );
    dsi.header()->getProperty( "sizeT", size[ 3 ] );
    if( frame[ 0 ] == 0 )
      frame = size;
    
    /*
    cout << "//---------------------------------------------------------" << endl;
    cout << "//   H E A D E R                                           " << endl;
    cout << "//---------------------------------------------------------" << endl;
    cout << "Size : " << size[ 0 ] << ", "
                      << size[ 1 ] << ", "
                      << size[ 2 ] << ", "
                      << size[ 3 ] << endl;
    cout << "Frame : " << frame[ 0 ] << ", "
                       << frame[ 1 ] << ", "
                       << frame[ 2 ] << ", "
                       << frame[ 3 ] << endl;
    */
    
    int16_t *buffer = (int16_t*) malloc( sizeof(int16_t) * frame[ 0 ] * frame[ 1 ] * frame[ 2 ] * frame[ 3 ] );
    GisImageReader<int16_t> gir;
    gir.read( buffer, dsi, origin, frame );
    
  } catch( user_interruption & ) {
    return EXIT_SUCCESS;
  } catch( exception & e ) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
  
}
