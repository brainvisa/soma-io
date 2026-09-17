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

#ifndef CARTOBASE_TYPE_SELECT_H
#define CARTOBASE_TYPE_SELECT_H


#include <cartobase/config/cartobase_config.h>


namespace carto
{


template < bool flag, typename U, typename V >
struct select 
{

  typedef U result;

};


template < typename U, typename V >
struct select< false, U, V >
{

  typedef V result;

};


} // namespace carto


#endif
