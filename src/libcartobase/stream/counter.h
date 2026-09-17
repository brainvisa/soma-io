#ifndef CARTOBASE_STREAM_COUNTER_H
#define CARTOBASE_STREAM_COUNTER_H


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

  /**	Extractor counting lines
   */
  class CARTOBASE_API CounterExtractor
  {

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    CounterExtractor(int line = 1);

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

    //---------------------------------------------------------------------
    /**	@name Count*/
    //---------------------------------------------------------------------
    //@{

    /**	Get the current line number.
	@return current line number
		
    */
    int line() const;

    /**	Reset the line counter.
	@param line new value of the line counter
	@return old value
    */
    int reset(int line = 1);

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    int _line;
    bool _newline;

    //@}
  };


  //===========================================================================
  //	I N L I N E  M E T H O D S
  //===========================================================================

  inline
  int
  CounterExtractor::line() const
  {
    return _line;
  }

}

#endif
