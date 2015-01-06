#include <soma-io/Object/CartoHeader.h>

#include <cartobase/object/object_d.h>


soma::CartoHeader::CartoHeader( carto::Object header )
                 : soma::HeaderProxy(),
                   m_header( header )
{
}

void soma::CartoHeader::addAttribute( const std::string& name, int32_t value )
{

  m_header->setProperty( name, value );

}


void soma::CartoHeader::addAttribute( const std::string& name, double value )
{

  m_header->setProperty( name, float( value ) );

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

  std::vector< float > vec( value.begin(), value.end() );

  m_header->setProperty( name, vec );

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

  int32_t i = 0;
  std::vector< std::vector< float > > vecvec( value.size() );
  std::vector< std::vector< double > >::const_iterator
    v = value.begin(),
    ve = value.end();

  while ( v != ve )
  {

    vecvec[ i++ ] = std::vector< float >( v->begin(), v->end() );
    ++v;

  }

  m_header->setProperty( name, vecvec );

}


bool soma::CartoHeader::hasAttribute( const std::string& name )
{

  return m_header->hasProperty( name );

}

