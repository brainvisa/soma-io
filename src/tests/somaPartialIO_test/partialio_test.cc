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
#include <soma-io/io/reader.h>
#include <soma-io/io/writer.h>
#include <soma-io/io/formatdictionary.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/writer/pythonwriter.h>
//--- cartodata ----------------------------------------------------------------
#include <soma-io/cartodata/volumeview.h>
#include <soma-io/cartodata/volumeformatwriter.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/object/object.h>
#include <cartobase/getopt/getopt.h>
#include <cartobase/config/verbose.h>
//--- system -------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
//------------------------------------------------------------------------------

using namespace moncartodata;
using namespace soma;
using namespace carto;
using namespace std;

void printheader( Object hdr )
{
  PythonWriter  pw;
  pw.attach( cout );
  pw.write( hdr );
}

int main( int argc, const char** argv )
{
  string  fname;
  try
  {
    //=== APPLICATION ==========================================================
    VolumeView<int16_t>::Position4Di origin;
    VolumeView<int16_t>::Position4Di frame;
    string ofname;
    bool partial_writing;

    CartoApplication  app( argc, argv, "Test for soma partial reading" );
    app.addOption( fname, "-i", "input filename to be read\n" );
    app.addOption( ofname, "-o", "output filename to be written\n", true );
    app.addOption( origin[ 0 ], "-ox", "frame origin (x comp)\n", true );
    app.addOption( origin[ 1 ], "-oy", "frame origin (y comp)\n", true );
    app.addOption( origin[ 2 ], "-oz", "frame origin (z comp)\n", true );
    app.addOption( origin[ 3 ], "-ot", "frame origin (t comp)\n", true );
    app.addOption( frame[ 0 ], "-sx", "frame size (x comp)\n", true );
    app.addOption( frame[ 1 ], "-sy", "frame size (y comp)\n", true );
    app.addOption( frame[ 2 ], "-sz", "frame size (z comp)\n", true );
    app.addOption( frame[ 3 ], "-st", "frame size (t comp)\n", true );
    app.addOption( partial_writing, "-pw", "partial writing of volume\n", true );
    app.alias( "-v", "--verbose" );

    app.initialize();
    
    //=== SHOW PARAMETERS ======================================================
    cout << "=== PARAMETERS ==========================================" << endl;
    cout << "filename: " << fname << endl;
    cout << "origin: " << origin[0] << ", " << origin[1] << ", "
                       << origin[2] << ", " << origin[3] << endl;
    cout << "frame: " << frame[0] << ", " << frame[1] << ", "
                      << frame[2] << ", " << frame[3] << endl;
    cout << "=========================================================" << endl;
    
    //=== DECLARE VARIABLES ====================================================
    Object options;

    //=== SET UNALLOCATED VOLUME ===============================================
    Reader<Volume<int16_t> > rVol( fname );
    cout << "=== VOLUME READER =======================================" << endl;
    cout << "filename: " << rVol.dataSource()->url() << endl;
    cout << "=========================================================" << endl;
    options = Object::value( PropertySet() );
    options->setProperty( "unallocated", true );
    options->setProperty( "resolution_level", 0 );
    rVol.setOptions( options );
    cout << "reading unallocated volume..." << endl;
    VolumeRef<int16_t>  vol( rVol.read() );
    cout << "done" << endl;
    
    //=== SET FRAME IF NOT INITIALIZED =========================================
    cout << "reading size from header..." << endl;
    VolumeView<int16_t>::Position4Di size;
    rVol.dataSourceInfo()->header()->getProperty( "sizeX", size[ 0 ] );
    rVol.dataSourceInfo()->header()->getProperty( "sizeY", size[ 1 ] );
    rVol.dataSourceInfo()->header()->getProperty( "sizeZ", size[ 2 ] );
    rVol.dataSourceInfo()->header()->getProperty( "sizeT", size[ 3 ] );
    if( frame[ 0 ] == 0 || frame[ 1 ] == 0 || 
        frame[ 2 ] == 0 || frame[ 3 ] == 0 )
      frame = size;
    cout << "done" << endl;
    cout << "=== HEADER ==============================================" << endl;
    printheader( rVol.dataSourceInfo()->header() );
    cout << "=========================================================" << endl;
    
    //=== SET ALLOCATED VOLUME VIEW ============================================
    Reader<Volume<int16_t> > rView( rVol.dataSourceInfo() );
    options = Object::value( PropertySet() );
    options->setProperty( "partial_reading", true );
    options->setProperty( "resolution_level", 0 );
    rView.setOptions( options );
    VolumeRef<int16_t> view( new VolumeView<int16_t>( vol, origin, frame ) );
    
    cout << "reading partial volume..." << endl;
    rView.read( *view );
    cout << "done" << endl;
    
    //=== SHOW RESULTS =========================================================
    cout << "=== VOLUME SIZES ========================================" << endl;
    cout << "unallocated volume: size(" 
         << vol->getSizeX() << ", "
         << vol->getSizeY() << ", "
         << vol->getSizeZ() << ", "
         << vol->getSizeT() << ")"
         << endl;
    cout << "allocated view: size(" 
         << view->getSizeX() << ", "
         << view->getSizeY() << ", "
         << view->getSizeZ() << ", "
         << view->getSizeT() << ")"
         << endl;
    cout << "=========================================================" << endl;
    cout << "=== WRITE VOLUME ========================================" << endl;
    /*
    int x, y, z, t;
    for( t=0; t<view->getSizeT(); ++t )
      for( z=0; z<view->getSizeZ(); ++z )
        for( y=0; y<view->getSizeY(); ++y ) {
          for( x=0; x<view->getSizeX(); ++x )
            cout << view( x, y, z, t ) << " ";
          cout << endl;
        }
    */
    if( !ofname.empty() ) {
      Writer<VolumeRef<int16_t> > vfw( ofname );
      options = Object::value( PropertySet() );
      if( partial_writing ) {
        vfw.write( vol, options );
        options = Object::value( PropertySet() );
        options->setProperty( "partial_writing", true );
        vfw.attach( ofname );
      }
     vfw.write( view, options );
    }
    cout << "=========================================================" << endl;
    
    //=== WRITE ================================================================
    
  }
  catch( user_interruption & )
  {
  }
  catch( datatype_format_error & e )
  {
    DataSourceInfoLoader  f;
    DataSourceInfo dsi( rc_ptr<DataSource>( new FileDataSource( fname ) ) );
    dsi = f.check( dsi );
    if( !dsi.header().isNone() )
      printheader( dsi.header() );
  }
  catch( exception & e )
  {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

