
#include <cartobase/object/object_d.h>
#include <cartobase/object/attributed.h>

using namespace std;

namespace carto {

  //------------------//
 //  instantiations  //
//------------------//

INSTANTIATE_GENERIC_OBJECT_TYPE( int )
INSTANTIATE_GENERIC_OBJECT_TYPE( unsigned )
INSTANTIATE_GENERIC_OBJECT_TYPE( char )
INSTANTIATE_GENERIC_OBJECT_TYPE( signed char )
INSTANTIATE_GENERIC_OBJECT_TYPE( unsigned char )
INSTANTIATE_GENERIC_OBJECT_TYPE( short )
INSTANTIATE_GENERIC_OBJECT_TYPE( unsigned short )
INSTANTIATE_GENERIC_OBJECT_TYPE( long )
INSTANTIATE_GENERIC_OBJECT_TYPE( unsigned long )
INSTANTIATE_GENERIC_OBJECT_TYPE( long long )
INSTANTIATE_GENERIC_OBJECT_TYPE( unsigned long long )
INSTANTIATE_GENERIC_OBJECT_TYPE( float )
INSTANTIATE_GENERIC_OBJECT_TYPE( double )
INSTANTIATE_GENERIC_OBJECT_TYPE( bool )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::string )
INSTANTIATE_GENERIC_OBJECT_TYPE( void * )
INSTANTIATE_GENERIC_OBJECT_TYPE( Void )

// DECLARE_GENERIC_OBJECT_TYPE cannot be used for Object because it explicitly
// instantiates DictionaryInterface::getProperty<Object> and
// DictionaryInterface::setProperty<Object>, which conflict with the (pure
// virtual) overload of these methods.
template class TypedObject< Object >;
template class ValueObject< Object >;
template class ReferenceObject< Object >;
template class PointerObject< Object >;
template
Object const &GenericObject::value< Object >() const;
template
Object &GenericObject::value< Object >();
template
bool GenericObject::value( Object &dest ) const;
template
void GenericObject::setValue( Object const & x );


template class TypedObject< GenericObject >;
template class ReferenceObject< GenericObject >;
template class PointerObject< GenericObject >;

INSTANTIATE_GENERIC_OBJECT_TYPE( Syntax )
INSTANTIATE_GENERIC_OBJECT_TYPE( Semantic )
INSTANTIATE_GENERIC_OBJECT_TYPE( SyntaxSet )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr<SyntaxSet> )
INSTANTIATE_GENERIC_OBJECT_TYPE( AttributedObject * )

INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<unsigned> )

#define _TMP_ std::map< std::string, Object >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_

}

