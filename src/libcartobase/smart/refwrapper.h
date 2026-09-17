
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
        if(this != &x) {
          _object = x._object;
        }
        return *this;
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
