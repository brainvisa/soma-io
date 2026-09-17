#include <soma-io/vector/vector.h>
#include <cartobase/object/object_d.h>

template class AimsVector<int16_t,4>;
template class AimsVector<int32_t,4>;
template class AimsVector<int64_t,4>;
template class AimsVector<uint16_t,4>;
template class AimsVector<uint32_t,4>;
template class AimsVector<uint64_t,4>;
template class AimsVector<float,4>;
template class AimsVector<double,4>;

namespace carto {

INSTANTIATE_GENERIC_OBJECT_TYPE( Point4ds )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4di )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dl )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dus )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dui )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dul )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4df )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point4dd )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4ds> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4di> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dl> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dus> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dui> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dul> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4df> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point4dd> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4ds> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4di> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4dl> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4dus> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4dui> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4dul> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4df> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point4dd> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4ds> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4di> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4dl> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4dus> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4dui> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4dul> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4df> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point4dd> > )


DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4ds )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4di )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4dl )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4dus )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4dui )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4dul )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4df )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point4dd )

} // namespace carto
