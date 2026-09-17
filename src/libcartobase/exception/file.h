
#ifndef CARTOBASE_EXCEPTION_FILE_H
#define CARTOBASE_EXCEPTION_FILE_H


//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/config/cartobase_config.h>
#include <cartobase/exception/errno.h>


//=============================================================================
//	C L A S S  D E C L A R A T I O N
//=============================================================================


namespace carto
{

/** Exceptions thrown during file manipulation
 */
class CARTOBASE_API file_error : public errno_error
{

  public:

    //---------------------------------------------------------------------
    /**	@name Constructors, destructor*/
    //---------------------------------------------------------------------
    //@{

    /**	Only use this version
	@param filename_arg name of the file
    */
    file_error( const std::string& filename_arg );

    /*	DO NOT USE! DEPRECIATED
	@param what_arg not used
	@param filename_arg name of the file
    */
    file_error( const std::string& what_arg, const std::string& filename_arg )
      __attribute__((__deprecated__));

    ///
    virtual ~file_error() throw ();

    //@}

    //---------------------------------------------------------------------
    /**	@name Data access*/
    //---------------------------------------------------------------------
    //@{

    ///
    const char* filename() const;

    //@}

  private:

    //---------------------------------------------------------------------
    /**	@name Initialization*/
    //---------------------------------------------------------------------
    //@{

    /**	format the "what" message
	@param filename_arg name of the file
    */
    static std::string init( const std::string& filename_arg );

    //@}

    //---------------------------------------------------------------------
    /**	@name Data*/
    //---------------------------------------------------------------------
    //@{

    /// the name of the file is supplied by the user
    const std::string _filename;

    //@}

};


}  // namespace carto


#endif
