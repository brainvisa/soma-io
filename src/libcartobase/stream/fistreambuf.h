#ifndef CARTOBASE_STREAM_FISTREAMBUF_H
#define CARTOBASE_STREAM_FISTREAMBUF_H


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

  /**	Filtering input stream
   */
  template <class Extractor>
  class CARTOBASE_API fistreambuf : public std::streambuf
  {

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    /**	Initialize with another streambuf as the source
	@param source source streambuf
	@param extractor filter
	@param d delete the source streambuf on exit
    */
    fistreambuf(std::streambuf* source, const Extractor& extractor,
		bool d = false);

    /**	Initialize with another streambuf as the source
	@param source source streambuf
	@param d delete the source streambuf on exit
    */
    fistreambuf(std::streambuf* source, bool d = false);

    /**	Call sync(), delete the source streambuf if asked
	to do so in the constructor
    */
    virtual ~fistreambuf();

    //@}

    //---------------------------------------------------------------------
    /**	@name Filtering*/
    //---------------------------------------------------------------------
    //@{

    /**	Actual filter
	@return the actual filter
    */
    Extractor& extractor();

    //@}

  protected:

    //---------------------------------------------------------------------
    /**	@name Get area*/
    //---------------------------------------------------------------------
    //@{

    /**	Get the first character of the pending sequence.
	If the pending sequence is null then the function returns
	EOF to indicate failure.
	@return the first character of the pending sequence
    */
    virtual int underflow();

    //@}

    //---------------------------------------------------------------------
    /**	@name Buffer management and positioning*/
    //---------------------------------------------------------------------
    //@{

    virtual int sync();

    /**	pass on to the source
	@param p 
	@param len 
	@return this
    */
    virtual std::streambuf* setbuf(char* p , std::streamsize len);
    virtual pos_type seekoff( off_type, std::ios_base::seekdir, 
                              std::ios_base::openmode = std::ios_base::in 
                              | std::ios_base::out );

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    ///	Source streambuf to read data from
    std::streambuf* _source;

    ///	Actual filter
    Extractor _extractor;

    char _buffer;

    ///	Delete _source on exit?
    bool _delete;

    //@}
  };


  //===========================================================================
  //	I N L I N E  M E T H O D S
  //===========================================================================

  template <class Extractor>
  inline
  Extractor&
  fistreambuf<Extractor>::extractor()
  {
    return _extractor;
  }

}

#endif
