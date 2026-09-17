
#include <cartobase/object/object_d.h>

using namespace std;

namespace carto {

  //------------------//
 //  instantiations  //
//------------------//

INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<int> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<char> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<signed char> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<unsigned char> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<short> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<unsigned short> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<unsigned long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<long long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<unsigned long long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<float> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<double> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<std::string> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::set<Object> )

}

