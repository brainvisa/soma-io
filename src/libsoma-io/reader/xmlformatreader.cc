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
