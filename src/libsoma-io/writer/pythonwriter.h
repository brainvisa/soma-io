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

#ifndef SOMAIO_WRITER_PYTHONWRITER_H
#define SOMAIO_WRITER_PYTHONWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/config/cartobase_config.h>
#include <cartobase/object/syntax.h>
#include <cartobase/object/object.h>
//--- system -----------------------------------------------------------------
#include <fstream>
//----------------------------------------------------------------------------

namespace carto
{
  class GenericObject;
  class Object;
}

namespace soma
{

  class CARTOBASE_API PythonWriter
  {
  public:
    typedef void (*Helper)( const carto::GenericObject &, PythonWriter & w, 
                            int indent, bool writeInternals );
    typedef std::map<std::string, Helper> HelperSet;
    typedef void (*CatchFunction)( PythonWriter &, std::exception &,
                                   const carto::Object & );

    PythonWriter( const std::string& filename,
                  const carto::SyntaxSet& rules = carto::SyntaxSet(),
                  const HelperSet& helpers = HelperSet() );
    PythonWriter( const carto::SyntaxSet& rules = carto::SyntaxSet(),
                  const HelperSet& helpers = HelperSet() );
    /// PythonWriter takes ownership of the DataSource
    PythonWriter( carto::rc_ptr<DataSource> ds, 
                  const carto::SyntaxSet& rules = carto::SyntaxSet(),
                  const HelperSet& helpers = HelperSet() );
    virtual ~PythonWriter();

    void open(const std::string& filename);
    ///	attach to an existing (and open) stream
    void attach( std::ostream & s );
    void attach( carto::rc_ptr<DataSource> ds );
    void close();
    virtual std::string name() const;
    bool operator ! () const;
    bool is_open() const;
    carto::rc_ptr<DataSource> dataSource() { return _datasource; }

    /// helper functions read basic data type
    const HelperSet & helpers() const;
          HelperSet & helpers();
    const carto::SyntaxSet & syntaxes() const;
          carto::SyntaxSet & syntaxes();

    /// Writes a generic object to stream
    void write( const carto::GenericObject & object, 
                bool writeInternals = false, 
                bool writevariable = true );
    void write( const carto::Object & object, 
                bool writeInternals = false, 
                bool writevariable = true );
    /// Writes an element according to the syntax
    void write( const carto::GenericObject & object, int indent, 
                const std::string & syntax = "", 
                const std::string & semantic = "", 
                bool writeInternals = false );
    /// Writes an element according to the syntax
    void write( const carto::Object & object, int indent,
                const std::string & syntax = "",
                const std::string & semantic = "",
                bool writeInternals = false );
    bool isInternal( const std::string & syntax,
                     const std::string & semantic );
    void setSingleLineMode( bool x ) { _singleLine = x; }
    bool singleLine() const { return _singleLine; }
    /// if set, the catch function is used instead of throwing the regular
    /// exception during writing
    void setCatchFunction( CatchFunction f );
    char quoteCharacter() const { return _quoteChar; }
    void setQuoteCharacter( char q ) { _quoteChar = q; }
    std::string noneString() const { return _noneString; }
    void setNoneString( const std::string & none ) { _noneString = none; }

    /// utility function
    void writeString( DataSource &, std::string );

    template<typename T>
    static void genericSequenceHelper( const carto::GenericObject & obj, 
                                       PythonWriter & w, int ind, 
                                       bool writeInternals );

  protected:
    void init( const HelperSet & helpers );

    carto::SyntaxSet          _rules;
    carto::rc_ptr<DataSource> _datasource;
    HelperSet                 _helpers;
    bool                      _singleLine;
    CatchFunction             _catchFunction;
    char                      _quoteChar;
    std::string               _noneString;
  };
} // namespace soma

namespace carto {
  DECLARE_GENERIC_OBJECT_TYPE( soma::PythonWriter::Helper )
  DECLARE_GENERIC_OBJECT_TYPE( soma::PythonWriter::HelperSet )
}

#endif
