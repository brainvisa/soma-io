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

