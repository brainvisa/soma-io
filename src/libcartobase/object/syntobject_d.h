#ifndef CARTOBASE_OBJECT_SYNTOBJECT_D_H
#define CARTOBASE_OBJECT_SYNTOBJECT_D_H

#include <cartobase/object/syntobject.h>

namespace carto
{

  namespace internal
  {

    template<typename T> 
    SyntaxedNonInterfaceObject<T>::~SyntaxedNonInterfaceObject()
    {
    }


    template<typename T> 
    SyntaxedInterfaceType<T>::~SyntaxedInterfaceType()
    {
    }

  }

}

#endif

