#ifndef CARTOBASE_STREAM_CUIFSTREAM_H
#define CARTOBASE_STREAM_CUIFSTREAM_H


//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/config/cartobase_config.h>
#ifndef CARTOBASE_STREAM_FISTREAMBUF_H
#include <cartobase/stream/fistreambuf.h>
#endif
#ifndef CARTOBASE_STREAM_COUNTER_H
#include <cartobase/stream/counter.h>
#endif
#ifndef CARTOBASE_STREAM_UNCOMMENT_H
#include <cartobase/stream/uncomment.h>
#endif
#include <fstream>

namespace carto
{

  //===========================================================================
  //	C L A S S  D E C L A R A T I O N
  //===========================================================================

  /**	Filtering ifstream - counts lines and skips comments
   */
  class CARTOBASE_API cuifstream : public std::istream
  {

  public:
    /** open modes are not a standard type in C++, they changed from 
        int in g++ 2.9x to std::ios_base::openmode in gcc 3.x */
#if defined( __GNUC__ ) && ( __GNUC__-0 < 3 )
    typedef int openmode;
#else
    typedef  std::ios_base::openmode openmode;
#endif

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    cuifstream();
    cuifstream(const char *name, openmode mode = in);

    virtual ~cuifstream();

    //@}

    //---------------------------------------------------------------------
    /**	@name Open, close*/
    //---------------------------------------------------------------------
    //@{

    virtual void open(const char *name, openmode mode = in);
    virtual void close();

    //@}

    //---------------------------------------------------------------------
    /**	@name Count*/
    //---------------------------------------------------------------------
    //@{

    /**	Get the current line number
	@return current line number
    */
    int line() const;

    //@}

    //---------------------------------------------------------------------
    /**	@name Stream state*/
    //---------------------------------------------------------------------
    //@{

    /**	Return the state of both associated streams
     */
    virtual bool operator!() const;

    /**	Is the ifstream open?
     */
    virtual bool is_open() const;
    void enableUncomment( bool );
    bool uncommentEnabled() const { return _enableuncomment; }
    virtual bool eof() const;

    //@}

  protected:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    virtual void init_buffers();
    virtual void destroy_buffers();

    //@}

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    mutable std::ifstream _base;	// mutable because is_open is not
					// const on gcc-3
    fistreambuf<CounterExtractor>* _counter;
    fistreambuf<UncommentExtractor>* _uncomment;
    bool  _enableuncomment;

    //@}
  };


  //===========================================================================
  //	I N L I N E  M E T H O D S
  //===========================================================================

  inline
  int
  cuifstream::line() const
  {
    return _counter->extractor().line();
  }

}

#endif
