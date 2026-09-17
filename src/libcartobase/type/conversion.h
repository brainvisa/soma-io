////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef CARTOBASE_TYPE_CONVERSION_H
#define CARTOBASE_TYPE_CONVERSION_H


#include <cartobase/config/cartobase_config.h>


namespace carto {


template <typename T, typename U>
class conversion 
{

    template <typename V, typename W>
    struct Helper
    {
      typedef char no;               // sizeof(no) == 1
      struct yes { char dummy[2]; }; // sizeof(yes) >= 2
      static no test( ... );
      static yes test( W* );
    };
    typedef Helper<T, U> H;

  public:

    enum 
    {
      exists = sizeof( typename H::yes ) == sizeof( H::test( static_cast<T*>(0) ) )
    };

};


} // namespace carto


#define SUPERSUBCLASS( T, U ) \
  ( static_cast<bool>(::carto::conversion<const U, const T>::exists) )

#define SUPERSUBCLASS_NONCONST( T, U ) \
  ( static_cast<bool>(::carto::conversion<U, T>::exists) )

#endif
