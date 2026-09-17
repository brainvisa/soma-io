#ifndef SOMAIO_READER_AREADER_H
#define SOMAIO_READER_AREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/config/cartobase_config.h>
#ifndef CARTOBASE_OBJECT_SYNTAX_H
  #include <cartobase/object/syntax.h>
#endif
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{

  //=========================================================================
  //	C L A S S  D E C L A R A T I O N
  //=========================================================================

  ///	To read GenericObject from a file
  class CARTOBASE_API AttributedReader
  {

  public:

    //------------------------------------------------------------------------
    /**	@name Associated types*/
    //------------------------------------------------------------------------
    //@{

    typedef void (*Helper)( carto::GenericObject&, const std::string&, 
                            DataSource &);
    typedef std::map<std::string,Helper> HelperSet;

    //@}

    //------------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //------------------------------------------------------------------------
    //@{

    AttributedReader( carto::rc_ptr<DataSource> ds,
                      const carto::SyntaxSet& rules,
                      const HelperSet& helpers = HelperSet() );

    /**	configure the parameters
	@param filename name of the file to read into
	@param rules list of all possible semantic attributes
	@param helpers for reading basic data types
    */
    AttributedReader(const std::string& filename,
		     const carto::SyntaxSet& rules,
		     const HelperSet& helpers = HelperSet());

    /**	configure the parameters
	@param rules list of all possible semantic attributes
	@param helpers for reading basic data types
    */
    AttributedReader(const carto::SyntaxSet& rules,
		     const HelperSet& helpers = HelperSet());

    /**	clean up
     */
    virtual ~AttributedReader();

    //@}

    //------------------------------------------------------------------------
    /**	@name Open, close*/
    //------------------------------------------------------------------------
    //@{

    /**	open a new file
	@param filename name of the file to read from
    */
    void open(const std::string& filename);
    void attach( carto::rc_ptr<DataSource> ds );
    ///	attach to an existing istream
    void attach( std::istream & stream, int line_num = 1 );

    /**	detach or close the stream
     */
    void close();
    carto::rc_ptr<DataSource> dataSource() { return _datasource; }

    //@}

    //------------------------------------------------------------------------
    /**	@name Query*/
    //------------------------------------------------------------------------
    //@{

    ///	name of the file to read from
    virtual std::string name() const;

    ///
    int line() const;

    //@}

    //------------------------------------------------------------------------
    /**	@name Helpers*/
    //------------------------------------------------------------------------
    //@{

    /**	return the status of the stream
	@return true if ifstream::operator!() returns true
    */
    bool operator!() const;

    /**	is the stream open?
	@return ifstream::is_open()
    */
    bool is_open() const;

    /**	have we hit EOF?
	@return true if EOF already hit, false otherwise
    */
    bool eof() const;

    //@}

  protected:

    //------------------------------------------------------------------------
    /**	@name Helpers*/
    //------------------------------------------------------------------------
    //@{

    /**	construct the value of a semantic attribute
	@param object GenericObject to attach to
	@param semantic semantic attribute name
    */
    void readAttribute(carto::GenericObject& object, 
		       const std::string& semantic);

    // DO NOT USE! DEPRECIATED
    void readAttribute(carto::GenericObject* object, 
		       const std::string& semantic)
      __attribute__((__deprecated__));

    //@}

    //------------------------------------------------------------------------
    /**	@name Data*/
    //------------------------------------------------------------------------
    //@{

    ///	rules for every syntactic attribute
    carto::SyntaxSet _rules;

    ///	filtering stream to read from
    carto::rc_ptr<DataSource> _datasource;

    ///	helper functions read basic data type
    HelperSet _helpers;

    //@}

  private:

    /**	install helpers for basic data types
     */
    void init();

    //------------------------------------------------------------------------
    /**	@name Disable copy*/
    //------------------------------------------------------------------------
    //@{

    /// Copy constructor
    AttributedReader(const AttributedReader&);

    /// Assignment operator
    AttributedReader& operator=(const AttributedReader&);

    //@}
  };

}

#endif
