#include <cartobase/object/syntobject_d.h>
#include <cartobase/object/property.h>
#include <cartobase/object/object_d.h>

using namespace carto;
using namespace std;


namespace carto
{
INSTANTIATE_GENERIC_OBJECT_TYPE( internal::SyntaxedInterfaceType<PropertySet> )
}
template class internal::SyntaxedNonInterfaceObject<Dictionary>;
template class internal::SyntaxedInterfaceType<PropertySet>;
template class internal::SyntaxedInterfaceObject<PropertySet>;


