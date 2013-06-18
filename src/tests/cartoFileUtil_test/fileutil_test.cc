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

#include <cstdlib>
#include <cartobase/stream/fileutil.h>
#include <cartobase/object/object.h>
#include <cartobase/object/property.h>
#include <cartobase/exception/assert.h>

using namespace carto;
using namespace std;


int main( int argc, char *argv[] )
{
  string uri1 = "/this/is/my/path/file.ext?option1=this&option2=that";
  string uri2 = "this/is/my/path/?option1=1";
  string uri3 = "file.ext";
  string uri4 = "file.ext?";
  string uri5 = "file.ext?option1=this&&option2&";
  string filename;
  Object options;
  
  cout << "parsing uri : " << uri1 << endl;
  filename = FileUtil::uriFilename( uri1 );
  cout << " -> " << filename << endl;
  ASSERT( filename == "/this/is/my/path/file.ext" );
  options = FileUtil::uriOptions( uri1 );
  ASSERT( options.get() != 0 );
  try {
    cout << " -> option1 : " << options->getProperty( "option1" )->getString() << endl;
  } catch( ... ) {
    cout << "!!! option1 failed" << endl;
  }
  ASSERT( options->getProperty( "option1" )->getString() == "this" );
  try {
    cout << " -> option2 : " << options->getProperty( "option2" )->getString() << endl;
  } catch( ... ) {
    cout << "!!! option2 failed" << endl;
  }
  ASSERT( options->getProperty( "option2" )->getString() == "that" );
  cout << endl;
  
  
  cout << "parsing uri : " << uri2 << endl;
  filename = FileUtil::uriFilename( uri2 );
  cout << " -> " << filename << endl;
  ASSERT( filename == "this/is/my/path/" );
  options = FileUtil::uriOptions( uri2 );
  ASSERT( options.get() != 0 );
  try {
    cout << " -> option1 : " << options->getProperty( "option1" )->getString() << endl;
  } catch( ... ) {
    cout << "!!! option1 failed" << endl;
  }
  ASSERT( options->getProperty( "option1" )->getString() == "1" );
  cout << endl;
  
  
  cout << "parsing uri : " << uri3 << endl;
  filename = FileUtil::uriFilename( uri3 );
  cout << " -> " << filename << endl;
  ASSERT( filename == "file.ext" );
  options = FileUtil::uriOptions( uri3 );
  ASSERT( options.get() == 0 );
  cout << endl;
  
  
  cout << "parsing uri : " << uri4 << endl;
  filename = FileUtil::uriFilename( uri4 );
  cout << " -> " << filename << endl;
  ASSERT( filename == "file.ext" );
  options = FileUtil::uriOptions( uri4 );
  ASSERT( options.get() == 0 );
  cout << endl;
  
  
  cout << "parsing uri : " << uri5 << endl;
  filename = FileUtil::uriFilename( uri5 );
  cout << " -> " << filename << endl;
  ASSERT( filename == "file.ext" );
  options = FileUtil::uriOptions( uri5 );
  ASSERT( options.get() != 0 );
  try {
    cout << " -> option1 : " << options->getProperty( "option1" )->getString() << endl;
  } catch( ... ) {
    cout << "!!! option1 failed" << endl;
  }
  ASSERT( options->getProperty( "option1" )->getString() == "this" );
  try {
    cout << " -> option2 : " << options->getProperty( "option2" )->getString() << endl;
  } catch( ... ) {
    cout << "!!! option2 failed" << endl;
  }
  ASSERT( options->getProperty( "option2" )->getString() == "true" );
  cout << endl;
}
