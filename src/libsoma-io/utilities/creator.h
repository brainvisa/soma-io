#ifndef SOMAIO_UTILITIES_CREATOR_H
#define SOMAIO_UTILITIES_CREATOR_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
#include <cartobase/config/verbose.h>
//----------------------------------------------------------------------------

namespace soma
{
  class AllocatorContext;

  /// Object creation / resize or other setup factory
  ///
  /// Such factories are used by the IO system which has to create or setup
  /// new objects according to information provided in the file header.
  ///
  /// Specific implementations for new object types may be specialized. It is 
  /// suggested that their ObjectCreator implementation is defined along with 
  /// the object definition itself, to avoid accidentally using the default 
  /// (non-specialized) creator somewhere (just like DataTypeCode).
  template <typename T>
  class Creator
  {
  public:
    /// Create a new object according to the given header.
    /// The default implementation just returns a new T.
    static T* create( carto::Object header, 
                      const AllocatorContext & context, 
                      carto::Object options );
    /// Setup (modify) an existing object according to the given header
    /// (maybe resize it etc.).
    /// The default implementation does nothing.
    static void setup( T & obj, carto::Object header, 
                       const AllocatorContext & context, 
                       carto::Object options );
  };


  template <typename T>
  inline
  T* Creator<T>::create( carto::Object, const AllocatorContext &, 
                         carto::Object )
  {
    cartoCondMsg( 4, "create", "CREATOR" );
    return new T;
  }


  template <typename T>
  inline
  void Creator<T>::setup( T &, carto::Object, const AllocatorContext &, 
                          carto::Object )
  {
    cartoCondMsg( 4, "setup", "CREATOR" );
  }

}

#endif
