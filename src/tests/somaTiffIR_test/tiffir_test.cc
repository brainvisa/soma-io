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

#include <soma-io/config/soma_config.h>
//#include <soma-io/checker/formatchecker.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
//#include <soma-io/io/formatdictionary_d.h>
//#include <soma-io/io/reader_d.h>
#include <soma-io/checker/tiffformatchecker.h>
#include <soma-io/image/tiffimagereader.h>
#include <soma-io/image/tiffimagereader_d.h>

//--- cartobase --------------------------------------------------------------
#include <cartobase/object/property.h>
#include <cartobase/smart/rcptr.h>
#include <cartobase/type/voxelrgb.h>

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
    vector<long> stride;
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
    
    // Get tiff FormatChecker
    FormatChecker * fc = DataSourceInfoLoader::formatInfo( "TIFF" );
    ASSERT(fc);
    
    DataSourceInfoLoader dsil;
    dsi = fc->check( dsi, dsil );
    
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

    typedef carto::VoxelRGB VoxelType;
    VoxelType *buffer = (VoxelType*) malloc( sizeof(VoxelType) * frame[ 0 ] * frame[ 1 ] * frame[ 2 ] * frame[ 3 ] );
    TiffImageReader<VoxelType> tiffir;
    tiffir.read( buffer, dsi, origin, frame, stride );
    
  } catch( user_interruption & ) {
    return EXIT_SUCCESS;
  } catch( exception & e ) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
  
}
