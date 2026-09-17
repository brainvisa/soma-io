#ifndef CARTOBASE_STREAM_FOSTREAMBUF_H
#define CARTOBASE_STREAM_FOSTREAMBUF_H


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

  /**	Filtering output stream
   */
  template <class Inserter>
  class CARTOBASE_API fostreambuf : public std::streambuf
  {

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    /**	Initialize with another streambuf as the sink
	@param sink sink streambuf
	@param inserter filter
	@param d delete the sink streambuf on exit
    */
    fostreambuf(std::streambuf* sink, const Inserter& inserter,
		bool d = false);

    /**	Initialize with another streambuf as the sink
	@param sink sink streambuf
	@param d delete the sink streambuf on exit
    */
    fostreambuf(std::streambuf* sink, bool d = false);

    /**	Call sync(), delete the sink streambuf if asked
	to do so in the constructor
    */
    virtual ~fostreambuf();

    //@}

    //---------------------------------------------------------------------
    /**	@name Filtering*/
    //---------------------------------------------------------------------
    //@{

    /**	Actual filter
	@return the actual filter
    */
    Inserter& inserter();

    //@}

  protected:

    //---------------------------------------------------------------------
    /**	@name Put area*/
    //---------------------------------------------------------------------
    //@{

    virtual int overflow(int c);

    //@}

    //---------------------------------------------------------------------
    /**	@name Buffer management and positioning*/
    //---------------------------------------------------------------------
    //@{

    virtual int sync();

    /**	pass on to the sink
	@param p 
	@param len 
	@return this
    */
    virtual std::streambuf* setbuf(char* p , int len);

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    ///	Sink streambuf to read data from
    std::streambuf* _sink;

    ///	Actual filter
    Inserter _inserter;

    ///	Delete _source on exit?
    bool _delete;

    //@}
  };


  //===========================================================================
  //	I N L I N E  M E T H O D S
  //===========================================================================

  template <class Inserter>
  inline
  Inserter&
  fostreambuf<Inserter>::inserter()
  {
    return _inserter;
  }

}

#endif
