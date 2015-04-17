#include <soma-io/Object/CartoHeader.h>

#include <cartobase/object/object_d.h>


dcm::CartoHeader::CartoHeader( carto::Object header )
                : dcm::HeaderProxy(),
                  _header( header )
{
}

void dcm::CartoHeader::addAttribute( const std::string& name, int32_t value )
{

  _header->setProperty( name, value );

}


void dcm::CartoHeader::addAttribute( const std::string& name, double value )
{

  _header->setProperty( name, float( value ) );

}


void dcm::CartoHeader::addAttribute( const std::string& name, 
                                     const std::string& value )
{

  _header->setProperty( name, value );

}


void dcm::CartoHeader::addAttribute( const std::string& name, 
                                     const std::vector< int32_t >& value )
{

  _header->setProperty( name, value );

}


void dcm::CartoHeader::addAttribute( const std::string& name, 
                                     const std::vector< double >& value )
{

  std::vector< float > vec( value.begin(), value.end() );

  _header->setProperty( name, vec );

}


void dcm::CartoHeader::addAttribute( const std::string& name, 
                                     const std::vector< std::string >& value )
{

  _header->setProperty( name, value );

}


void dcm::CartoHeader::addAttribute( 
                             const std::string& name, 
                             const std::vector< std::vector< double > >& value )
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

  _header->setProperty( name, vecvec );

}


bool dcm::CartoHeader::hasAttribute( const std::string& name )
{

  return _header->hasProperty( name );

}
