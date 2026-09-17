
#include <cartobase/object/object_d.h>

using namespace std;

namespace carto {

  //------------------//
 //  instantiations  //
//------------------//

INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<unsigned> )

INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<int> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<unsigned> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<char> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<signed char> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<unsigned char> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<short> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<unsigned short> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<unsigned long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<long long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<unsigned long long> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<float> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<double> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Object> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<Void> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::string> )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::string> * )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<int> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<unsigned> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<char> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<signed char> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<unsigned char> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<short> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<unsigned short> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<unsigned long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<long long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<unsigned long long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<float> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<double> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::set<unsigned> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::list<unsigned> > )

INSTANTIATE_GENERIC_OBJECT_TYPE( std::vector<std::vector<std::string> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( std::list<std::vector<std::string> > )

INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<int> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<unsigned> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<char> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<signed char> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<unsigned char> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<short> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<unsigned short> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<unsigned long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<long long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<unsigned long long> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<float> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<double> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<Object> > )
INSTANTIATE_GENERIC_OBJECT_TYPE( rc_ptr< std::vector<std::string> > )

}

