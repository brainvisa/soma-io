#include <cartobase/exception/format.h>

using namespace std;
using namespace carto;


//////////////////////
//  invalid_number  //
//////////////////////

invalid_number::invalid_number( const string &value )
               : format_error( value + " is not a valid number", "" )
{
}


///////////////////////
//  invalid_integer  //
///////////////////////

invalid_integer::invalid_integer( const string &value )
                : invalid_number( value + " is no a valid integer", true )
{
}


////////////////////////////////
//  invalid_positive_integer  //
////////////////////////////////

invalid_positive_integer::invalid_positive_integer( const string &value )
                         : invalid_integer( value +
                                            " is no a valid positive integer",
                                            true )
{
}


//////////////////////////
//  out_of_range_error  //
//////////////////////////

out_of_range_error::out_of_range_error( const string &value )
                   : range_error( value + " is out of range" )
{
}


///////////////////////
//  invalid_boolean  //
///////////////////////

invalid_boolean::invalid_boolean( const string &value )
                : format_error( value + " is no a valid boolean", "" )
{
}
