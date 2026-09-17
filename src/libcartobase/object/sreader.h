#ifndef CARTOBASE_OBJECT_SREADER_H
#define CARTOBASE_OBJECT_SREADER_H

//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#ifndef CARTOBASE_STREAMS_CUIFSTREAM_H
#include <cartobase/stream/cuifstream.h>
#endif
#ifndef CARTOBASE_OBJECT_SYNTAX_H
#include <cartobase/object/syntax.h>
#endif

namespace carto
{

  //===========================================================================
  //	C L A S S  D E C L A R A T I O N
  //===========================================================================

  /**	Read a SyntaxSet from a file
   */
  class CARTOBASE_API SyntaxReader
  {

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    SyntaxReader(const std::string& filename);
    virtual ~SyntaxReader();

    //@}

    //---------------------------------------------------------------------
    /**	@name Read*/
    //---------------------------------------------------------------------
    //@{

    /**	read the referenced attributes from a stream
	@return syntactic attribute / associated rule pairs
    */
    void read(SyntaxSet& rules);

    //@}

  protected:

    //---------------------------------------------------------------------
    /**	@name Helpers*/
    //---------------------------------------------------------------------
    //@{

    /**	read characters not in s
	@param s continue while characters outside s
	@return accepted characters
    */
    std::string readUntil(const std::string& s);

    /**	skip characters in s
	@param s continue while characters in s
    */
    void skipWhile(const std::string& s);

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Disable copy*/
    //---------------------------------------------------------------------
    //@{

    ///	Copy constructor
    SyntaxReader(const SyntaxReader&);

    ///	Assignment operator
    SyntaxReader& operator=(const SyntaxReader&);

    //@}

    //---------------------------------------------------------------------
    /**	@name Helpers*/
    //---------------------------------------------------------------------
    //@{

    /**	read a token from the input stream
	@return next token from the input stream
    */
    std::string nextToken();

    /**	
     */
    void read(Syntax& syntax);

    //@}

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    ///
    std::string _filename;

    ///	filtering stream to read from
    cuifstream _stream;

    //@}
  };

}

//===========================================================================
//	F U N C T I O N
//===========================================================================

CARTOBASE_API carto::SyntaxReader& operator>>( carto::SyntaxReader& reader, 
					       carto::SyntaxSet& rules );

#endif
