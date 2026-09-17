#include <cstdlib>
#include <cartobase/stream/fileutil.h>
#include <cartobase/stream/stringutil.h>
#include <cartobase/object/object.h>
#include <cartobase/object/property.h>
#include <cartobase/exception/assert.h>
#include <cartobase/getopt/getopt.h>
#include <cartobase/config/verbose.h>

#define verbMsg( message ) if( verbose ) std::cout << message << std::endl;

using namespace carto;
using namespace std;


int main( int argc, const char **argv )
{
  //=== APPLICATION ==========================================================
  CartoApplication  app( argc, argv, "Test for uri parser" );
  app.alias( "-v", "--verbose" );
  app.alias( "-d", "--debugLevel" );

  app.initialize();

  //=== STRINGS TO TEST ======================================================
  string uri1 = "/this/is/my/path/file.ext?option1=this&option2=that";
  string uri2 = "this/is/my/path/?option1=1.2";
  string uri3 = "file.ext";
  string uri4 = "file.ext?";
  string uri5 = "file.ext?option1=this&&option2&";
  string uri6 = "/home/Petri/base/TD201210_Aperio/3399.svs?bx=5&by=5&resolution_level=2";
  string uri7 = "?option1=1.2";
  string filename;
  Object options;

  //=== PROCESSING ===========================================================
  //--- test #1 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri1 );
  filename = FileUtil::uriFilename( uri1 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "/this/is/my/path/file.ext" );
  options = FileUtil::uriOptions( uri1 );
  ASSERT( options.get() != 0 );
  try {
    verbMsg( " -> option1 : \t" + options->getProperty( "option1" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! option1 failed" );
  }
  ASSERT( options->getProperty( "option1" )->getString() == "this" );
  try {
    verbMsg( " -> option2 : \t" + options->getProperty( "option2" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! option2 failed" );
  }
  ASSERT( options->getProperty( "option2" )->getString() == "that" );
  verbMsg( "" );

  //--- test #2 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri2 );
  filename = FileUtil::uriFilename( uri2 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "this/is/my/path/" );
  options = FileUtil::uriOptions( uri2 );
  ASSERT( options.get() != 0 );
  try {
    verbMsg( " -> option1 : \t" + options->getProperty( "option1" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! option1 failed" );
  }
  ASSERT( options->getProperty( "option1" )->getScalar() == (float)1.2 );
  verbMsg( "" );

  //--- test #3 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri3 );
  filename = FileUtil::uriFilename( uri3 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "file.ext" );
  options = FileUtil::uriOptions( uri3 );
  ASSERT( options.get() == 0 );
  verbMsg( "" );

  //--- test #4 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri4 );
  filename = FileUtil::uriFilename( uri4 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "file.ext" );
  options = FileUtil::uriOptions( uri4 );
  ASSERT( options.get() == 0 );
  verbMsg( "" );

  //--- test #5 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri5 );
  filename = FileUtil::uriFilename( uri5 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "file.ext" );
  options = FileUtil::uriOptions( uri5 );
  ASSERT( options.get() != 0 );
  try {
    verbMsg( " -> option1 : \t" + options->getProperty( "option1" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! option1 failed" );
  }
  ASSERT( options->getProperty( "option1" )->getString() == "this" );
  try {
    verbMsg( " -> option2 : \t" + options->getProperty( "option2" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! option2 failed" );
  }
  ASSERT( options->getProperty( "option2" )->getScalar() == 1 );
  verbMsg( "" );

  //--- test #6 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri6 );
  filename = FileUtil::uriFilename( uri6 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "/home/Petri/base/TD201210_Aperio/3399.svs" );
  options = FileUtil::uriOptions( uri6 );
  ASSERT( options.get() != 0 );
  try {
    verbMsg( " -> bx : \t" + options->getProperty( "bx" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! bx failed" );
  }
  ASSERT( options->getProperty( "bx" )->getScalar() == 5 );
  try {
    verbMsg( " -> by : \t" + options->getProperty( "by" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! by failed" );
  }
  ASSERT( options->getProperty( "by" )->getScalar() == 5 );
  try {
    verbMsg( " -> resolution_level : \t" + options->getProperty( "resolution_level" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! resolution_level failed" );
  }
  ASSERT( options->getProperty( "resolution_level" )->getScalar() == 2 );
  verbMsg( "" );

  //--- test #7 --------------------------------------------------------------
  verbMsg( "parsing uri : " + uri7 );
  filename = FileUtil::uriFilename( uri7 );
  verbMsg( " -> filename : \t" + filename );
  ASSERT( filename == "" );
  options = FileUtil::uriOptions( uri7 );
  ASSERT( options.get() != 0 );
  try {
    verbMsg( " -> option1 : \t" + options->getProperty( "option1" )->getString() );
  } catch( ... ) {
    verbMsg( "!!! option1 failed" );
  }
  ASSERT( options->getProperty( "option1" )->getScalar() == (float)1.2 );
  verbMsg( "" );

  //--- test #7: strings -----------------------------------------------------
  string text = "This is an example string; we will cut it, "
    "into several parts. Yes.";
  verbMsg( "test StringUtil" );
  vector<string> stext = StringUtil::split( text, " " );
  ASSERT( stext.size() == 13 );
  ASSERT( stext[0] == "This" && stext[12] == "Yes." );
  vector<string> patterns;
  patterns.push_back( "; " );
  patterns.push_back( ". " );
  patterns.push_back( " " );
  patterns.push_back( "." );
  stext = StringUtil::split( text, patterns );
  ASSERT( stext.size() == 15 );
  ASSERT( stext[0] == "This" );
  ASSERT( stext[4] == "string" );
  ASSERT( stext[11] == "parts" );
  ASSERT( stext[12] == "" );
  ASSERT( stext[13] == "Yes" );
  ASSERT( stext[14] == "" );
  ASSERT( StringUtil::join( stext, ". " ) == "This. is. an. example. string. we. will. cut. it,. into. several. parts. . Yes. " );
  stext = StringUtil::split( text, patterns, 10 );
  ASSERT( stext.size() == 11 );
  ASSERT( stext[9] == "into" );
  ASSERT( stext[10] == "several parts. Yes." );
  text = "   .   toto bubulle.   babar...  ";
  ASSERT( StringUtil::strip( text, " ." ) == "toto bubulle.   babar" );
  ASSERT( StringUtil::strip( text ) == ".   toto bubulle.   babar..." );
}
