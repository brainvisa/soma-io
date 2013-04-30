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
  string fname;
  CartoApplication  app( argc, argv, "Test for GIS format checking" );
  app.addOption( fname, "-i", "input filename to be read\n" );
  app.alias( "-v", "--verbose" );
  app.initialize();
  
  DataSourceInfo dsi, newdsi;
  GisFormatChecker gfc;
  DataSourceInfoLoader dsil;
  dsi.list().addDataSource( "default", 
                            rc_ptr<DataSource>( new FileDataSource( fname ) ) 
                          );
  newdsi = gfc.check( dsi, dsil );
  
  //--- Write Header -----------------------------------------------------------
  cout << "//---------------------------------------------------------" << endl;
  cout << "//   H E A D E R                                           " << endl;
  cout << "//---------------------------------------------------------" << endl;
  if( newdsi.header()->hasProperty( "sizeX" ) )
    cout << "sizeX: \t" << newdsi.header()->getProperty( "sizeX" )->getString() << endl;
  if( newdsi.header()->hasProperty( "sizeY" ) )
    cout << "sizeY: \t" << newdsi.header()->getProperty( "sizeY" )->getString() << endl;
  if( newdsi.header()->hasProperty( "sizeZ" ) )
    cout << "sizeZ: \t" << newdsi.header()->getProperty( "sizeZ" )->getString() << endl;
  if( newdsi.header()->hasProperty( "sizeT" ) )
    cout << "sizeT: \t" << newdsi.header()->getProperty( "sizeT" )->getString() << endl;
  if( newdsi.header()->hasProperty( "format" ) )
    cout << "format: \t" << newdsi.header()->getProperty( "format" )->getString() << endl;
  if( newdsi.header()->hasProperty( "voxel_size" ) )
    cout << "voxel_size: \t( " << newdsi.header()->getProperty( "voxel_size" )->getArrayItem(0)->getScalar() << ", "
                               << newdsi.header()->getProperty( "voxel_size" )->getArrayItem(1)->getScalar() << ", "
                               << newdsi.header()->getProperty( "voxel_size" )->getArrayItem(2)->getScalar() << " )"
                               << endl;
  if( newdsi.header()->hasProperty( "object_type" ) )
    cout << "object_type: \t" << newdsi.header()->getProperty( "object_type" )->getString() << endl;
  if( newdsi.header()->hasProperty( "ascii" ) )
    cout << "ascii: \t" << newdsi.header()->getProperty( "ascii" )->getString() << endl;
  if( newdsi.header()->hasProperty( "byte_swapping" ) )
    cout << "byte_swapping: \t" << newdsi.header()->getProperty( "byte_swapping" )->getString() << endl;
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
  set<string> types = newdsi.list().types();
  set<string>::iterator it = types.begin();
  for( it; it != types.end(); ++it ) {
    int size = newdsi.list().size( *it );
    int i;
    for ( i = 0; i< size; ++i ) {
      cout << "DS " << *it << " : \t" << newdsi.list().dataSource( *it, i )->url() << endl;
    }
  }
  
}