/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */


#include <cartobase/object/object_d.h>

using namespace std;

namespace carto {

  //------------------//
 //  instantiations  //
//------------------//

#define _TMP_  rc_ptr< std::map< std::string, int > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, unsigned > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, char > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, signed char > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, unsigned char > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, short > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, unsigned short > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, long > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, unsigned long > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, long long > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, unsigned long long > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, float > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, double > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, bool > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, std::string > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_  rc_ptr< std::map< std::string, Object > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ rc_ptr< std::map< int, Object > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ rc_ptr< std::map< float, Object > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_
#define _TMP_ rc_ptr< std::map< Object, Object > >
INSTANTIATE_GENERIC_OBJECT_TYPE( _TMP_ )
#undef _TMP_

}

