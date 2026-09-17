#ifndef CARTOBASE_EXCEPTION_ERRNO_H
#define CARTOBASE_EXCEPTION_ERRNO_H


//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/config/cartobase_config.h>
#include <stdexcept>
#include <string>


//=============================================================================
//	C L A S S  D E C L A R A T I O N
//=============================================================================


namespace carto
{


/**

    System exceptions memorize the value of errno

*/
class CARTOBASE_API errno_error : public std::runtime_error
{

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors and Destructor*/
    //---------------------------------------------------------------------
    //@{

    /**	Memorize the value of "errno" when thrown
     */
    errno_error();

    ///
    virtual ~errno_error() throw ();

    //@}

    //---------------------------------------------------------------------
    /**	@name Data access*/
    //---------------------------------------------------------------------
    //@{

    ///
    int thrown_errno() const;

    //@}

  protected:

    //---------------------------------------------------------------------
    /**	@name Constructors and Destructor*/
    //---------------------------------------------------------------------
    //@{

    /**	For use by derived classes only!
	@param what_arg "what" string given by the derived class
    */
    errno_error( const std::string& what_arg );

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Initialization*/
    //---------------------------------------------------------------------
    //@{

    static std::string init();

    //@}

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    /// value of errno at the time the exception is thrown
    const int _errno_err;

    //@}

};


}  // namespace carto


#endif
