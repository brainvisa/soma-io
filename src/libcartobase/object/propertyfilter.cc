#include <cartobase/object/propertyfilter.h>
#include <stdexcept>


using namespace carto;


PropertyFilter::PropertyFilter( const std::string& name,
                                const std::set< std::string >& propertyNames )
               : RCObject(),
                 _name( name ),
                 _propertyNames( propertyNames )
{
}


PropertyFilter::PropertyFilter( const std::string& name,
                                const std::string& propertyName )
               : RCObject(),
                 _name( name )
{

  _propertyNames.insert( propertyName );

}


PropertyFilter::~PropertyFilter()
{
}


const std::string& PropertyFilter::getName() const
{

  return _name;

}


PropertyFilter::Signal& PropertyFilter::getSignal()
{

  return _signal;

}


bool PropertyFilter::hasProperty( const std::string& propertyName ) const
{

  return _propertyNames.find( propertyName ) != _propertyNames.end();

}


bool PropertyFilter::hasOldValue( const std::string& propertyName ) const
{

  return _oldValues.find( propertyName ) != _oldValues.end();

}


void PropertyFilter::setOldValue( const std::string& propertyName,
                                  const Object& oldValue )
{

  if ( hasProperty( propertyName ) )
  {

    _oldValues[ propertyName ] = oldValue;

  }

}


Object PropertyFilter::getOldValue( const std::string& propertyName ) const
{

 std::map< std::string, Object >::const_iterator
    o = _oldValues.find( propertyName );
  if ( o == _oldValues.end() )
  {

    throw std::runtime_error( std::string( 
            "Object PropertyFilter::getOldValue( "
            "const std::string& propertyName ) const : "
            "no old value for property '" ) + propertyName + "'" +
            "in property filter '" + _name + "'" );

  }
  return o->second;

}


void PropertyFilter::emitsignal()
{

  _signal.sigcemit( *this );
  _oldValues.clear();

}
