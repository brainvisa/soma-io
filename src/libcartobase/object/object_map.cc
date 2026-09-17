
#include <cartobase/object/object_d.h>

using namespace std;

namespace carto {

  //------------------//
 //  instantiations  //
//------------------//

#define _TMP_ std::map< std::string, int >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, unsigned >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, char >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, signed char >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, unsigned char >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, short >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, unsigned short >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, long >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, unsigned long >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, long long >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, unsigned long long >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, float >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, double >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, bool >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ std::map< std::string, std::string >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define TMP std::map< int, Object >
INSTANTIATE_GENERIC_OBJECT_TYPE( TMP )
#undef TMP
#define TMP std::map< float, Object >
INSTANTIATE_GENERIC_OBJECT_TYPE( TMP )
#undef TMP
#define TMP std::map< Object, Object >
INSTANTIATE_GENERIC_OBJECT_TYPE( TMP )
#undef TMP

}

