#ifndef CARTOBASE_TYPE_LIMITS_H
#define CARTOBASE_TYPE_LIMITS_H

#include <cartobase/config/cartobase_config.h>

#if defined( __GNUC__ ) && (__GNUC__-0 < 3)
// use a copy of gcc-3.2 <limits>
#include <cartobase/type/limits_gcc3.h>
#else
// use standard C++ header
#include <limits>
#endif

#endif

