#ifndef _CartoHeader_h_
#define _CartoHeader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Object/HeaderProxy.h>

#include <cartobase/object/object.h>


namespace soma
{


class CartoHeader : public HeaderProxy
{


  public:

    CartoHeader( carto::Object header );

    void addAttribute( const std::string& name, int32_t value );
    void addAttribute( const std::string& name, double value );
    void addAttribute( const std::string& name, const std::string& value );

    void addAttribute( const std::string& name, 
                       const std::vector< int32_t >& value );
    void addAttribute( const std::string& name, 
                       const std::vector< double >& value );
    void addAttribute( const std::string& name, 
                       const std::vector< std::string >& value );

    void addAttribute( const std::string& name, 
                       const std::vector< std::vector< double > >& value );

    bool hasAttribute( const std::string& name );

  private:

    carto::Object _header;

};


}


#endif
