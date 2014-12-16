#ifndef _Header_h_
#define _Header_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <vector>


namespace soma
{


class Header
{

  public:

    Header();
    virtual ~Header();

    virtual void addAttribute( const std::string& name, int32_t value ) = 0;
    virtual void addAttribute( const std::string& name, double value ) = 0;
    virtual void addAttribute( const std::string& name, 
                               const std::string& value ) = 0;

    virtual void addAttribute( const std::string& name, 
                               std::vector< int32_t >& value ) = 0;
    virtual void addAttribute( const std::string& name, 
                               std::vector< double >& value ) = 0;
    virtual void addAttribute( const std::string& name, 
                               std::vector< std::string >& value ) = 0;

    virtual void addAttribute( 
                              const std::string& name, 
                              std::vector< std::vector< double > >& value ) = 0;

};


}


#endif
