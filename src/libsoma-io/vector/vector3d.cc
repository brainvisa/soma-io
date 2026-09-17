#include <soma-io/vector/vector.h>
#include <cartobase/object/object_d.h>

template class AimsVector<int16_t,3>;
template class AimsVector<int32_t,3>;
template class AimsVector<int64_t,3>;
template class AimsVector<uint16_t,3>;
template class AimsVector<uint32_t,3>;
template class AimsVector<uint64_t,3>;
template class AimsVector<float,3>;
template class AimsVector<double,3>;

namespace carto {

INSTANTIATE_GENERIC_OBJECT_TYPE( Point3ds )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3di )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dl )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dus )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dui )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dul )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3df )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point3dd )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3ds> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3di> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dl> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dus> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dui> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dul> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3df> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point3dd> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3ds> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3di> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3dl> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3dus> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3dui> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3dul> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3df> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point3dd> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3ds> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3di> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3dl> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3dus> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3dui> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3dul> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3df> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point3dd> > )


DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3ds )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3di )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3dl )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3dus )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3dui )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3dul )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3df )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point3dd )

} // namespace carto
