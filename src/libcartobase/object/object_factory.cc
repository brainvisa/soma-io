#include <cartobase/object/object_factory.h>

using namespace std;
using namespace carto;

ObjectFactory::ObjectFactory()
{
  _registerDefaultFactory< int >( "int" ); 
  _registerDefaultFactory< float >( "float" ); 
  _registerDefaultFactory< string >( "string" ); 
  _registerDefaultFactory< vector<int> >( "int_vector" ); 
  _registerDefaultFactory< vector<float> >( "float_vector" ); 
  _registerDefaultFactory< vector<string> >( "string_vector" ); 
}


void ObjectFactory::registerFactory( const std::string &type, ObjectFactoryFunction factory )
{
  _singleton()._registerFactory( type, factory );
}


void ObjectFactory::_registerFactory( const std::string &type, ObjectFactoryFunction factory )
{
  _factories[ type ] = factory;
}


Object ObjectFactory::createObject( const std::string &type )
{
  return _singleton()._createObject( type );
}

Object ObjectFactory::_createObject( const std::string &type ) const
{
  FactoriesType::const_iterator it = _factories.find( type );
  if ( it != _factories.end() ) {
    return it->second();
  }
  return Object();
}

ObjectFactory &ObjectFactory::_singleton()
{
  static ObjectFactory factory;
  return factory;
}



