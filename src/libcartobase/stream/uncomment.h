#ifndef CARTOBASE_STREAM_UNCOMMENT_H
#define CARTOBASE_STREAM_UNCOMMENT_H


//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/config/cartobase_config.h>
#if defined(CARTO_USE_PRE_ISO_HEADERS)
#include <streambuf.h>
#else
#include <streambuf>
#endif

namespace carto
{

  //===========================================================================
  //	C L A S S  D E C L A R A T I O N
  //===========================================================================

  /**	Extractor stripping end-of-line comments
   */
  class CARTOBASE_API UncommentExtractor
  {

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    UncommentExtractor(char c = '#');

    //@}

    //---------------------------------------------------------------------
    /**	@name Extract*/
    //---------------------------------------------------------------------
    //@{

    /**	Get a character from the source.
	@return next character from the source
		
    */
    int operator()(std::streambuf& source);

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{
    char _comment;

    //@}
  };

}

#endif
