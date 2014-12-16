#include <soma-io/Object/CartoHeader.h>


soma::CartoHeader::CartoHeader( carto::Object& header )
                 : m_header( header )
{
}

void soma::CartoHeader::addAttribute( const std::string& name, int32_t value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( const std::string& name, double value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( const std::string& name, 
                                      const std::string& value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( const std::string& name, 
                                      std::vector< int32_t >& value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( const std::string& name, 
                                      std::vector< double >& value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( const std::string& name, 
                                      std::vector< std::string >& value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( 
                                   const std::string& name, 
                                   std::vector< std::vector< double > >& value )
{

  m_header->setProperty( name, value );

}
