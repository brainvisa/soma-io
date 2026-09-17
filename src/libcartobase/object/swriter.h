#ifndef CARTOBASE_OBJECT_SWRITER_H
#define CARTOBASE_OBJECT_SWRITER_H

//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#ifndef CARTOBASE_OBJECT_SYNTAX_H
#include <cartobase/object/syntax.h>
#endif
#include <fstream>

namespace carto
{

  //===========================================================================
  //	C L A S S  D E C L A R A T I O N
  //===========================================================================

  /**	Write a SyntaxSet to a file
   */
  class CARTOBASE_API SyntaxWriter
  {

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    SyntaxWriter(const std::string& filename);
    virtual ~SyntaxWriter();

    //@}

    //---------------------------------------------------------------------
    /**	@name Write*/
    //---------------------------------------------------------------------
    //@{

    /**	read the referenced attribute I/O properties to a stream
    */
    void write(const SyntaxSet& rules);

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Disable copy*/
    //---------------------------------------------------------------------
    //@{

    ///	Copy constructor
    SyntaxWriter(const SyntaxWriter&);

    ///	Assignment operator
    SyntaxWriter& operator=(const SyntaxWriter&);

    //@}


    //---------------------------------------------------------------------
    /**	@name Helpers*/
    //---------------------------------------------------------------------
    //@{

    void write(const SemanticSet& set, std::string::size_type tab1, 
	       std::string::size_type tab2);

    //@}


    std::string _filename;
    std::ofstream _stream;
  };

}

//===========================================================================
//	F U N C T I O N
//===========================================================================

CARTOBASE_API carto::SyntaxWriter& operator<<( carto::SyntaxWriter& writer, 
					       const carto::SyntaxSet& rules );

#endif
