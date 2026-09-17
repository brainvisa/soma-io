#include <cstdlib>
#include <soma-io/utilities/minfXML2.h>
#include <soma-io/writer/pythonwriter.h>

using namespace carto;
using namespace std;


int main( int argc, char *argv[] )
{
  if( argc < 2 || string( argv[1] ) == "-h" || string( argv[1] ) == "--help" )
  {
    cerr << "Usage: " << argv[0] << " minfXML_file" << endl;
    return EXIT_SUCCESS;
  }
  try {
    Object minf = readDictionaryMinfXML( argv[ 1 ] );
    PythonWriter pw;
    pw.attach( cout );
    pw.write( minf );
  }
  catch( exception &e ) {
    cerr << e.what() << endl;
    exit( 1 );
  }
  catch( ... ) {
    cerr << "Unexpected exception" << endl;
    exit( 1 );
  }
}
