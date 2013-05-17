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
#include <soma-io/plugingis/gisimagewriter.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
//--- cartodata ----------------------------------------------------------------
//--- cartobase ----------------------------------------------------------------
#include <cartobase/object/object.h>
#include <cartobase/object/property.h>
#include <cartobase/getopt/getopt.h>
#include <cartobase/config/verbose.h>
//--- system -------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
//------------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

void printheader( Object hdr )
{
  
}

int main( int argc, const char** argv )
{
  string  ofname;
  try
  {
    //=== APPLICATION ==========================================================
    CartoApplication  app( argc, argv, "Test for soma partial writing" );
    app.addOption( ofname, "-o", "output filename to be written\n" );
    app.alias( "-v", "--verbose" );

    app.initialize();
    
    //=== DECLARE VARIABLES ====================================================
    cout << "declaring variables..." << endl;
    DataSourceInfo dsi( rc_ptr<DataSource>( new FileDataSource( ofname ) ) );
    dsi.header() = Object::value( PropertySet() );
    dsi.header()->setProperty( "sizeX", 230 );
    dsi.header()->setProperty( "sizeY", 175 );
    dsi.header()->setProperty( "sizeZ", 213 );
    dsi.header()->setProperty( "sizeT", 1 );
    vector<int> vs( 4, .1 );
    dsi.header()->setProperty( "voxel_size", vs );
    dsi.header()->setProperty( "ascii", false );
    dsi.header()->setProperty( "byte_swapping", false );
    Object options = Object::value( PropertySet() );
    options->setProperty( "unallocated", true );
    cout << "done: declaring variables" << endl;

    //=== WRITER ===============================================================
    cout << "writing header..." << endl;
    GisImageWriter<int16_t> giw;
    dsi = giw.writeHeader( dsi, options );
    cout << "done: writing header" << endl;
    
    //=== RESULTS ==============================================================
    cout << "=== RESULTS =============================================" << endl;
    cout << "fname: " << ofname << endl;
    cout << "=========================================================" << endl;
    
  }
  catch( user_interruption & )
  {
  }
  catch( exception & e )
  {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

