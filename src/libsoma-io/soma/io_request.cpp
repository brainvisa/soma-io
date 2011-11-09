#include <soma/io_request.hpp>

using namespace std;

namespace soma {

  
IORequest::IORequest( const std::string &request_string )
{
  // TODO
}


void IORequest::set( const std::string &key, const std::string &value )
{
  _attributes[ key ] = value;
}


const std::string &IORequest::get( const std::string &key ) const
{
  map< string, string >::const_iterator i = _attributes.find( key );
  if ( i != _attributes.end() ) {
    return i->second;
  }
  return empty;
}


void IORequest::remove( const std::string &key )
{
  _attributes.erase( key );
}

const std::string IORequest::empty = string();

} // namespace soma
