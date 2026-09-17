//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/writer/pythonformatwriter.h>
#include <soma-io/io/formatdictionary.h>
#include <soma-io/writer/pythonwriter.h>
#include <soma-io/io/writer.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "PYTHONFORMATWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

bool PythonFormatWriter::filterProperties(Object /* properties */,
                                          Object /* options */)
{
    // Nothing to filter here
    return true;
}

bool PythonFormatWriter::write( const GenericObject & obj, 
                                rc_ptr<DataSourceInfo> dsi, 
                                Object options )
{
  rc_ptr<DataSource> ds;
  if( !dsi->list().empty( "minf" ) )
    ds = dsi->list().dataSource( "minf" );
  else
    ds = dsi->list().dataSource();

  localMsg( "write " + ds->url() );
  SyntaxSet			synt;
  PythonWriter::HelperSet	hs;
  bool				writeinternals = false;
  bool				writevariable = true;
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
      try
        {
          writevariable 
            = options->getProperty( "writevariable" )->getScalar();
        }
      catch( ... )
        {
        }
    }
  PythonWriter	pw( ds, synt, hs );
  pw.write( obj, writeinternals, writevariable );
  return true;
}


namespace
{

  bool initpythonformat()
  {
    PythonFormatWriter	*r = new PythonFormatWriter;
    vector<string>	exts;
    exts.push_back( "minf" );
    exts.push_back( "py" );
    FormatDictionary<GenericObject>::registerFormat( "PYTHON", r, exts );
    return true;
  }

  bool dummy __attribute__((unused)) = initpythonformat();

}


#undef localMsg
