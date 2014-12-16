#ifndef _CartoHeader_h_
#define _CartoHeader_h_


#include <soma-io/Object/Header.h>

#include <cartobase/object/object.h>


namespace soma
{


class CartoHeader : public Header
{


  public:

    CartoHeader( carto::Object& header );

    void addAttribute( const std::string& name, int32_t value );
    void addAttribute( const std::string& name, double value );
    void addAttribute( const std::string& name, const std::string& value );

    void addAttribute( const std::string& name, 
                       std::vector< int32_t >& value );
    void addAttribute( const std::string& name, 
                       std::vector< double >& value );
    void addAttribute( const std::string& name, 
                       std::vector< std::string >& value );

    void addAttribute( const std::string& name, 
                       std::vector< std::vector< double > >& value );

  private:

    carto::Object& m_header;

};


}


#endif
