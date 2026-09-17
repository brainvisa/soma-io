#include <soma-io/vector/vector.h>
#include <cartobase/object/object_d.h>

template class AimsVector<int16_t,2>;
template class AimsVector<int32_t,2>;
template class AimsVector<int64_t,2>;
template class AimsVector<uint16_t,2>;
template class AimsVector<uint32_t,2>;
template class AimsVector<uint64_t,2>;
template class AimsVector<float,2>;
template class AimsVector<double,2>;

namespace carto {

INSTANTIATE_GENERIC_OBJECT_TYPE( Point2ds )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2di )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dl )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dus )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dui )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dul )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2df )
INSTANTIATE_GENERIC_OBJECT_TYPE( Point2dd )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2ds> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2di> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dl> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dus> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dui> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dul> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2df> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Point2dd> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2ds> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2di> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2dl> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2dus> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2dui> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2dul> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2df> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<Point2dd> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2ds> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2di> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2dl> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2dus> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2dui> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2dul> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2df> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<Point2dd> > )


DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2ds )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2di )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2dl )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2dus )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2dui )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2dul )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2df )
DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION ( Point2dd )

} // namespace carto
