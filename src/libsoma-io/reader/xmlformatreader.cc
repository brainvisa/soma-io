//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/reader/xmlformatreader.h>
#include <soma-io/utilities/minfXML2.h>
#include <soma-io/io/formatdictionary.h>
#include <soma-io/allocator/allocator.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "XMLFORMATREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   U T I L I T I E S
//============================================================================

namespace
{
  rc_ptr<SyntaxSet> getSyntax( Object options )
  {
    rc_ptr<SyntaxSet> rsynt;
    if( !options.isNone() )
      {
        if( options->getProperty( "syntaxset", rsynt ) )
          return rsynt;
        rsynt.reset( new SyntaxSet );
        if( options->getProperty( "syntaxset", *rsynt ) )
          return rsynt;
      }
    return rc_ptr<SyntaxSet>();
  }
}

//============================================================================
//   I N I T
//============================================================================

namespace
{

  bool initxmlformat()
  {
    XMLFormatReader *r = new XMLFormatReader;
    vector<string>  exts;
    exts.push_back( "minf" );
    exts.push_back( "xml" );
    FormatDictionary<GenericObject>::registerFormat( "XML", r, exts );
    return true;
  }

  bool dummy __attribute__((unused)) = initxmlformat();

}


//============================================================================
//   N E W   M E T H O D S
//============================================================================

GenericObject* XMLFormatReader::createAndRead(
                                         rc_ptr<DataSourceInfo> dsi,
                                         const AllocatorContext & /* context */,
                                         Object options )
{
  rc_ptr<DataSource> ds = dsi->list().dataSource( "minf", 0 );
  rc_ptr<SyntaxSet> syntax = getSyntax( options );
  Object o = readDictionaryMinfXML( *ds, syntax );
  GenericObject *go = o.get();
  o.release();
  return go;
}


void XMLFormatReader::read( GenericObject & obj, rc_ptr<DataSourceInfo> dsi, 
                            const AllocatorContext & /* context */, 
                            Object options )
{
  rc_ptr<DataSource> ds = dsi->list().dataSource( "minf", 0 );
  localMsg( "read " + ds->url() );
  Object  o( &obj );
  try
    {
      rc_ptr<SyntaxSet> syntax = getSyntax( options );
      readDictionaryMinfXML( *ds, o, syntax );
      o.release();
    }
  catch( ... )
    {
      o.release();
      throw;
    }
}


FormatReader<GenericObject>* XMLFormatReader::clone() const
{
  return new XMLFormatReader;
}


//============================================================================
//   O L D   M E T H O D S
//============================================================================

GenericObject* XMLFormatReader::createAndRead( Object, rc_ptr<DataSource> ds, 
                                               const AllocatorContext &, 
                                               Object options )
{
  rc_ptr<SyntaxSet> syntax = getSyntax( options );
  Object o = readDictionaryMinfXML( *ds, syntax );
  GenericObject	*go = o.get();
  o.release();
  return go;
}


void XMLFormatReader::read( GenericObject & obj, Object /*header*/, 
                            const AllocatorContext & context, 
                            Object options )
{
  localMsg( "read " + context.dataSource()->url() );
  Object	o( &obj );
  try
    {
      rc_ptr<SyntaxSet> syntax = getSyntax( options );
      readDictionaryMinfXML( *context.dataSource(), o, syntax );
      o.release();
    }
  catch( ... )
    {
      o.release();
      throw;
    }
}

#undef localMsg
