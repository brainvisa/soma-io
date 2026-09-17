#ifndef CARTOBASE_TYPE_TYPETRAITS_H
#define CARTOBASE_TYPE_TYPETRAITS_H

namespace carto
{

  // a template to be specialized in specific objects
  template <typename T> carto::Object getObjectHeader( T & )
  {
    return Object();
  }

}

#endif

