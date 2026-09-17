
#include <cartobase/object/object_d.h>

using namespace std;

namespace carto {

  //------------------//
 //  instantiations  //
//------------------//

#define _TMP_  rc_ptr< std::map< std::string, std::vector<int> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<unsigned> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<char> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<signed char> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<unsigned char> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<short> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<unsigned short> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<long> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<unsigned long>  > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<long long> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<unsigned long long> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<float> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<double> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<std::string> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::vector<Object> > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_

}

