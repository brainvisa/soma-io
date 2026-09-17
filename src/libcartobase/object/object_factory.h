#ifndef CARTOBASE_OBJECT_OBJECT_FACTORY_H
#define CARTOBASE_OBJECT_OBJECT_FACTORY_H

#include <cartobase/object/object.h>

namespace carto {

class ObjectFactory : public RCObject
{
public:

  typedef Object (*ObjectFactoryFunction)();

  static void registerFactory( const std::string &type, ObjectFactoryFunction );
  template <typename T>
  inline static void registerDefaultFactory( const std::string &type );
  static Object createObject( const std::string &type );


private:

  typedef std::map< std::string, ObjectFactoryFunction > FactoriesType;

  template <typename T>
  static Object _defaultFactoryFunction();

  ObjectFactory();

  void _registerFactory( const std::string &type, ObjectFactoryFunction );
  template <typename T>
  inline void _registerDefaultFactory( const std::string &type );
  Object _createObject( const std::string &type ) const;

  static ObjectFactory &_singleton();

  FactoriesType _factories;
};


template <typename T>
inline void ObjectFactory::registerDefaultFactory( const std::string &type )
{
  _singleton()._registerDefaultFactory<T>( type );
}


template <typename T>
Object ObjectFactory::_defaultFactoryFunction()
{
  return Object::value( T() );
}

// template <>
// Object ObjectFactory::_defaultFactoryFunction<int>()
// {
//   return Object::value( int() );
// }

template <typename T>
inline void ObjectFactory::_registerDefaultFactory( const std::string &type )
{
  _registerFactory( type, &ObjectFactory::_defaultFactoryFunction<T> );
}


} // namespace carto

#endif // ifndef CARTOBASE_OBJECT_OBJECT_FACTORY_H
