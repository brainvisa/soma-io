//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/writer/jsonformatwriter.h>
#include <soma-io/io/formatdictionary.h>
#include <soma-io/writer/pythonwriter.h>
#include <soma-io/io/writer.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "JSONFORMATWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

bool JsonFormatWriter::filterProperties(Object /* properties */,
                                        Object /* options */)
{
    // Nothing to filter here
    return true;
}

bool JsonFormatWriter::write( const GenericObject & obj,
                              rc_ptr<DataSourceInfo> dsi,
                              Object options )
{
  rc_ptr<DataSource> ds;
  ds = dsi->list().dataSource();

  localMsg( "write " + ds->url() );
  SyntaxSet                     synt;
  PythonWriter::HelperSet       hs;
  bool                          writeinternals = false;
  if( !options.isNone() )
    {
      options->getProperty( "syntaxset", synt );
      options->getProperty( "helpers", hs );
      try
        {
          writeinternals
            = options->getProperty( "writeinternals" )->getScalar();
        }
      catch( ... )
        {
        }
    }
  PythonWriter  pw( ds, synt, hs );
  pw.setQuoteCharacter( '"' );
  pw.setNoneString( "null" );
  pw.write( obj, writeinternals, false );
  return true;
}


namespace
{

  bool initjsonformat()
  {
    JsonFormatWriter  *r = new JsonFormatWriter;
    vector<string>      exts;
    exts.push_back( "json" );
    FormatDictionary<GenericObject>::registerFormat( "JSON", r, exts );
    return true;
  }

  bool dummy __attribute__((unused)) = initjsonformat();

}


#undef localMsg
