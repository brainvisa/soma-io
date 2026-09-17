//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/formatdictionary_d.h>
#include <soma-io/io/reader_d.h>
#include <soma-io/reader/formatreader_d.h>
#include <soma-io/utilities/creator.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/property.h>
//----------------------------------------------------------------------------

using namespace carto;
using namespace std;

namespace soma
{

  template <>
  inline
  GenericObject* 
  Creator<GenericObject>::create( Object, const AllocatorContext &, Object )
  {
    return new ValueObject<PropertySet>;
  }

template class Reader<GenericObject>;
template class FormatReader<GenericObject>;
template class FormatDictionary<GenericObject>;
template class Reader<Object>;
template class FormatReader<Object>;
template class FormatDictionary<Object>;

} // namespace soma
