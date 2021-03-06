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
