#ifndef SOMAIO_WRITER_AWRITER_H
#define SOMAIO_WRITER_AWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/config/cartobase_config.h>
#ifndef CARTOBASE_OBJECT_SYNTAX_H
  #include <cartobase/object/syntax.h>
#endif
#include <cartobase/object/object.h>
//--- system -----------------------------------------------------------------
#include <fstream>
//----------------------------------------------------------------------------

namespace soma
{

  //==========================================================================
  //	C L A S S  D E C L A R A T I O N
  //==========================================================================

  ///	To write an AttributedObject to a file
  class CARTOBASE_API AttributedWriter
  {

  public:

    //------------------------------------------------------------------------
    ///	@name Associated types
    //------------------------------------------------------------------------
    //@{

    typedef void (*Helper)( const carto::GenericObject&, const std::string&,
                            std::ostream& );
    typedef std::map<std::string, Helper> HelperSet;

    //@}

    //------------------------------------------------------------------------
    ///	@name Constructors, destructor
    //------------------------------------------------------------------------
    //@{

    ///	configure the parameters
    /// \param filename name of the file to write into
    /// \param rules syntactic attribute / rules pairs
    /// \param helpers writing functions set
    AttributedWriter( const std::string& filename,
                      const carto::SyntaxSet& rules,
                      const HelperSet& helpers = HelperSet() );

    ///	configure the parameters
    /// \param rules syntactic attribute / rules pairs
    /// \param helpers writing functions set
    AttributedWriter( const carto::SyntaxSet& rules,
                      const HelperSet& helpers = HelperSet() );

    ///	clean up
    virtual ~AttributedWriter();

    //@}

    //------------------------------------------------------------------------
    ///	@name Open, close
    //------------------------------------------------------------------------
    //@{

    ///	open a new file
    /// @param filename name of the file to write into
    void open(const std::string& filename );

    ///	attach to an existing (and open) stream
    void attach( std::ostream & s );

    ///	close or detach the stream
    void close();

    //------------------------------------------------------------------------
    /**	@name Query*/
    //------------------------------------------------------------------------
    //@{

    ///	name of the file to read from
    virtual std::string name() const;

    //@}

    //------------------------------------------------------------------------
    ///	@name Helpers
    //------------------------------------------------------------------------
    //@{

    /// return the status of the stream
    /// @return true if ifstream::operator!() returns true
    bool operator!() const;

    ///is the stream open?
    /// @return ifstream::is_open()
    bool is_open() const;

    //@}

    void setSyntax( const carto::SyntaxSet & rules )
    { _rules = rules; }
    const carto::SyntaxSet & getSyntax() const
    { return _rules; }

  protected:

    //------------------------------------------------------------------------
    ///	@name Helpers
    //------------------------------------------------------------------------
    //@{

    ///
    void writeAttributes( const carto::GenericObject& object );

    ///
    void writeAttributes( const carto::GenericObject& object,
                          const std::set<std::string>& attributes, 
                          std::string::size_type size );

    // DO NOT USE! DEPRECIATED
    void writeAttributes( const carto::GenericObject* object )
      __attribute__((__deprecated__));

    // DO NOT USE! DEPRECIATED
    void writeAttributes( const carto::GenericObject* object,
                          const std::set<std::string>& attributes, 
                          std::string::size_type size )
      __attribute__((__deprecated__));

    //@}

    //------------------------------------------------------------------------
    /// @name Data
    //------------------------------------------------------------------------
    //@{

    ///	rules for every syntactic attribute
    carto::SyntaxSet _rules;

    /// C++ stream to write into
    std::ostream	*_stream;
    bool		_ownStream;

    //@}

  private:

    //------------------------------------------------------------------------
    ///	@name Constructors, destructor
    //------------------------------------------------------------------------
    //@{

    ///	install helpers for basic data types
    void init();

    //@}

    //------------------------------------------------------------------------
    ///	@name Dat
    //------------------------------------------------------------------------
    //@{

    /// name of the file to write into
    std::string _filename;

    ///	helper functions read basic data type
    HelperSet _helpers;

    //@}

    //------------------------------------------------------------------------
    ///	@name Disable copy
    //------------------------------------------------------------------------
    //@{

    /// Copy constructor
    AttributedWriter( const AttributedWriter& );

    /// Assignment operator
    AttributedWriter& operator=( const AttributedWriter& );

    //@}
  };

}

#endif
