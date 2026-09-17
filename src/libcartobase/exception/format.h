#ifndef CARTOBASE_EXCEPTION_FORMAT_H
#define CARTOBASE_EXCEPTION_FORMAT_H


#include <cartobase/config/cartobase_config.h>
#include <cartobase/exception/ioexcept.h>
#include <stdexcept>


namespace carto
{

//////////////////////
//  invalid_number  //
//////////////////////

class CARTOBASE_API invalid_number : public format_error
{

  public:

    invalid_number( const std::string &value );
    inline invalid_number( const std::string &message, bool ) :
      format_error( message, "" ) {}

};


///////////////////////
//  invalid_integer  //
///////////////////////

class CARTOBASE_API invalid_integer : public invalid_number
{

  public:

    invalid_integer( const std::string &value );
    inline invalid_integer( const std::string &message, bool fullMessage ) :
    invalid_number( message, fullMessage ) {}

};


////////////////////////////////
//  invalid_positive_integer  //
////////////////////////////////

class CARTOBASE_API invalid_positive_integer : public invalid_integer
{

  public:

    invalid_positive_integer( const std::string &value );
    inline invalid_positive_integer( const std::string &message, 
                                     bool fullMessage ) :
    invalid_integer( message, fullMessage ) {}

};


//////////////////////////
//  out_of_range_error  //
//////////////////////////

/// there's already a out_of_range class in \<stdexcept\> !
class CARTOBASE_API out_of_range_error : public std::range_error
{

  public:

    out_of_range_error( const std::string &value );
    inline out_of_range_error( const std::string &message, bool ) :
    std::range_error( message ) {}

};


///////////////////////
//  invalid_boolean  //
///////////////////////

class CARTOBASE_API invalid_boolean : public format_error
{

  public:

    invalid_boolean( const std::string &value );
    inline invalid_boolean( const std::string &message, bool ) :
      format_error( message, "" ) {}

};


}


#endif
