#ifndef SOMA_IO_REQUEST
#define SOMA_IO_REQUEST

#include <string>
#include <map>


namespace soma {
  
class IORequest
{
public:
  IORequest( const std::string & );
  void set( const std::string &key, const std::string &value );
  const std::string &get( const std::string &key ) const;
  void remove( const std::string &key );

  static const std::string empty;
  
protected:
  std::map< std::string, std::string > _attributes;
};


class ImageReadRequest : public IORequest
{
public:
  
};

} // namespace soma

#endif // SOMA_IO_REQUEST
