#include <soma-io/transformation/transformation.h>
#include <cartobase/object/property.h>

using namespace soma;
using namespace carto;
using namespace std;

Transformation::Transformation()
  : _header( Object::value( PropertySet() ) )
{
}


Transformation::~Transformation()
{
}


void Transformation::setHeader( Object ph )
{
  _header = Object::value( PropertySet() );
  if( ph )
    _header->copyProperties( ph );
}


Transformation3d::~Transformation3d()
{
}


Transformation & Transformation::operator = ( const Transformation & other )
{
  if( &other == this )
    return *this;
  if( other.header() )
    _header = Object::value( other.header()->value<PropertySet>() );
  else
    _header->clearProperties();
  return *this;
}



