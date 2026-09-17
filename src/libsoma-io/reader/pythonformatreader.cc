//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/reader/pythonformatreader.h>            // class declaration
#include <soma-io/io/formatdictionary.h>
#include <soma-io/datasource/datasource.h>
#include <soma-io/allocator/allocator.h>
#include <soma-io/reader/pythonreader.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "PYTHONFORMATREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   I N I T
//============================================================================

namespace
{

  bool initpythonformat()
  {
    PythonFormatReader  *r = new PythonFormatReader;
    vector<string>  exts;
    exts.push_back( "minf" );
    exts.push_back( "py" );
    FormatDictionary<GenericObject>::registerFormat( "PYTHON", r, exts );
    return true;
  }

  bool dummy __attribute__((unused)) = initpythonformat();

}


//============================================================================
//   N E W   M E T H O D S
//============================================================================

GenericObject* 
PythonFormatReader::createAndRead( rc_ptr<DataSourceInfo> dsi,
                                   const AllocatorContext & /* context */, 
                                   Object options )
{
  rc_ptr<DataSource> ds = dsi->list().dataSource( "minf", 0 );
  localMsg( "createAndRead " + ds->url() );
  SyntaxSet	synt;
  rc_ptr<SyntaxSet> rsynt;
  PythonReader::HelperSet	hs;
  if( !options.isNone() )
    {
      options->getProperty( "syntaxset", rsynt )
          || options->getProperty( "syntaxset", synt );
      options->getProperty( "helpers", hs );
    }
  PythonReader	pr( ds, rsynt ? *rsynt : synt, hs );
  localMsg( "calling PythonReader" );
  return pr.read();
}


void PythonFormatReader::read( GenericObject & obj, 
                               rc_ptr<DataSourceInfo> dsi, 
                               const AllocatorContext & /* context */, 
                               Object options )
{
  rc_ptr<DataSource> ds = dsi->list().dataSource( "minf", 0 );
  SyntaxSet	synt;
  rc_ptr<SyntaxSet> rsynt;
  PythonReader::HelperSet	hs;
  if( !options.isNone() )
    {
      options->getProperty( "syntaxset", rsynt )
          || options->getProperty( "syntaxset", synt );
      options->getProperty( "helpers", hs );
    }
  PythonReader	pr( ds, rsynt ? *rsynt : synt, hs );
  pr.read( obj );
}


FormatReader<GenericObject>* PythonFormatReader::clone() const
{
  return new PythonFormatReader;
}

//============================================================================
//   O L D   M E T H O D S
//============================================================================

GenericObject* 
PythonFormatReader::createAndRead( Object, rc_ptr<DataSource> ds, 
                                   const AllocatorContext &, 
                                   Object options )
{
  localMsg( "createAndRead " + ds->url() );
  SyntaxSet synt;
  rc_ptr<SyntaxSet> rsynt;
  PythonReader::HelperSet hs;
  if( !options.isNone() )
    {
      options->getProperty( "syntaxset", rsynt )
          || options->getProperty( "syntaxset", synt );
      options->getProperty( "helpers", hs );
    }
  PythonReader  pr( ds, rsynt ? *rsynt : synt, hs );
  localMsg( "calling PythonReader" );
  return pr.read();
}


void PythonFormatReader::read( GenericObject & obj, Object /*header*/, 
                               const AllocatorContext & context, 
                               Object options )
{
  SyntaxSet synt;
  rc_ptr<SyntaxSet> rsynt;
  PythonReader::HelperSet hs;
  if( !options.isNone() )
    {
      options->getProperty( "syntaxset", rsynt )
          || options->getProperty( "syntaxset", synt );
      options->getProperty( "helpers", hs );
    }
  PythonReader  pr( context.dataSource(), rsynt ? *rsynt : synt, hs );
  pr.read( obj );
}

#undef localMsg
