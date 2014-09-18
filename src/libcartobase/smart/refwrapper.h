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


#ifndef CARTOBASE_SMART_REFWRAPPER_H
#define CARTOBASE_SMART_REFWRAPPER_H

namespace carto {

  /// Wrapper to a reference.
  /// For now, it can only be used with objects (and not functions)
  template <typename T>
  class reference_wrapper
  {
    public:
      typedef T type;
      reference_wrapper( type & ref ): _object(&ref)
      {}
      reference_wrapper( const reference_wrapper<type> & x ): _object(x._object)
      {}
      ~reference_wrapper()
      {};
      reference_wrapper<type>& operator= ( const reference_wrapper<type> & x )
      {
        _object = x._object;
      }
      type& get() const { return *_object; }
      operator type&() const { return get(); }
    protected:
      type* _object;
  };

  namespace wrap {
    /// Function to create a wrapper holding a reference
    template <typename T>
    reference_wrapper<T> ref( T & ref )
    {
      return reference_wrapper<T>( ref );
    }

    /// Same, from copy
    template <typename T>
    reference_wrapper<T> ref( reference_wrapper<T> & other )
    {
      return reference_wrapper<T>( other );
    }

    /// Function to create a wrapper holding a const reference
    template <typename T>
    reference_wrapper<const T> cref( const T & ref )
    {
      return reference_wrapper<const T>( ref );
    }

    /// Same, from copy
    template <typename T>
    reference_wrapper<const T> cref( reference_wrapper<const T> & other )
    {
      return reference_wrapper<const T>( other );
    }
  }// namespace wrap

}// namespace carto

#endif
