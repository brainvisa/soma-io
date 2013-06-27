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
#include <soma-io/config/soma_config.h>
#include <soma-io/reader/pythonformatreader.h>              // class declaration
#include <soma-io/io/formatdictionary.h>
#include <soma-io/datasource/datasource.h>
#include <soma-io/allocator/allocator.h>
#include <soma-io/reader/pythonreader.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/object/object.h>
//--- debug --------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "PYTHONFORMATREADER" )
// localMsg must be undef at end of file
//------------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//==============================================================================
//   I N I T
//==============================================================================

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

  bool dummy = initpythonformat();

}


//==============================================================================
//   N E W   M E T H O D S
//==============================================================================

GenericObject* 
PythonFormatReader::createAndRead( rc_ptr<DataSourceInfo> dsi,
                                   const AllocatorContext & context, 
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
                               const AllocatorContext & context, 
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


//==============================================================================
//   O L D   M E T H O D S
//==============================================================================

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