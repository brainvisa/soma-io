#ifndef CARTOBASE_CONFIG_CARTOBASE_CONFIG_H
#define CARTOBASE_CONFIG_CARTOBASE_CONFIG_H

// define this to issue compiler warnings when using obsolete headers
#define CARTO_ATTRIBUTED_OBSOLETE_WARNING

#if defined( _WIN32 ) && !defined( __GNUC__ )
#ifdef CARTOBASE_EXPORTS
#define CARTOBASE_API __declspec(dllexport)
#else
#define CARTOBASE_API __declspec(dllimport)
#endif
#else // _WIN32
#define CARTOBASE_API
#endif // _WIN32

#if defined(_MSC_VER)
#  define CARTO_NO_PARTIAL_TEMPLATE_SPECIALIZATION 1
#elif defined(__BORLANDC__)
#  define CARTO_NO_MEMBER_TEMPLATE_FRIENDS 1
#  define CARTO_BROKEN_FRIEND_TEMPLATE_FUNCTION 1
#elif defined (__GNUC__)
#  if ( __GNUC__-0 == 2 )
#    define CARTO_USE_PRE_ISO_HEADERS
#    if ( __GNUC_MINOR__-0 == 95 )
#      define CARTO_NO_MEMBER_TEMPLATE_FRIENDS 1
#    endif
#  endif
#endif

// useful macro which is not always provided in the system
#ifndef __GLIBC_PREREQ
#if defined( __GLIBC__ ) && defined( __GLIBC_MINOR__ )
// taken from /usr/include/features.h in case it's not defined
#define __GLIBC_PREREQ(maj, min) \
	((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __GLIBC_PREREQ(maj, min) 1
#endif
#endif

// allow large files
#undef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGE_FILES
#define _LARGE_FILES
#endif

#if ( defined(_WIN32) && !defined( __GNUC__ ) ) || defined(__osf__)
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
#ifndef INT8_MIN
#define INT8_MIN   (-128)
#endif
#ifndef INT8_MAX
#define INT8_MAX   (127)
#endif
#ifndef UINT8_MAX
#define UINT8_MAX  (255)
#endif
#ifndef INT16_MIN
#define INT16_MIN  (-32768)
#endif
#ifndef INT16_MAX
#define INT16_MAX  (32767)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (65535)
#endif
#ifndef INT32_MIN
#define INT32_MIN  (-2147483648)
#endif
#ifndef INT32_MAX
#define INT32_MAX  (2147483647)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif
#elif defined(__sun) || defined(__sgi)
#include <inttypes.h>
#else
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#endif

// isnan() portablility function or macro
#if defined( _WIN32 ) && !defined( isnan )
#  ifdef __MINGW32__
#    include <math.h>
#  else
#    include <float.h>
#    define rint(x) floor(x+0.5)
#    define isnan(x) _isnan(x)
#  endif
#elif defined( __APPLE__ ) && (__GNUC__==3) && (__GNUC_MINOR__<=3)
     // BUG On MacOS 10.2 / 10.3
#    include <cmath>
#    define  isnan( x )         ( ( sizeof ( x ) == sizeof(double) ) ?        \
                              __isnand ( x ) :                                \
                                ( sizeof ( x ) == sizeof( float) ) ?          \
                              __isnanf ( x ) :                                \
                              __isnan  ( x ) )
#else
// Standards-compliant compiler
#  include <cmath>
#endif

#ifdef __MINGW32__
#define CARTO_BROKEN_EXTERN_TEMPLATES
#endif

// test if long is a different type from both int32_t and int64_t
#if !defined( __LP64__ ) || defined( __APPLE__ )
#define CARTO_LONG_IS_DISTINCT
#endif

// Compatibility with clang feature checks for non-clang compilers.
#ifndef __has_feature
  #define __has_feature(x) 0
#endif
#ifndef __has_extension
  #define __has_extension __has_feature // Compatibility with pre-3.0 compilers.
#endif

// Ignore __attribute__ on non-GCC compilers
#if !(defined(__GNUC__) || defined(__attribute__))
#define __attribute__(a) /* nothing */
#endif

// GCC < 4.5 does not support the argument to the __deprecated__ attribute
#if ((__GNUC__-0 < 4) || (__GNUC_MINOR__-0 < 5)) && !__has_extension(attribute_deprecated_with_message)
#define __deprecated__(msg) __deprecated__
#endif

/** \def CARTO_OVERRIDE
    \brief The \c override keyword of C++11 (expands to nothing in C++98).

    The \c override keyword of C++11 should be used when declaring an override
    to an inherited virtual function. It helps prevent programming errors where
    the prototype does not match the base virtual function, which results in a
    new member function being defined. The compiler will output an error if
    there is no corresponding virtual function in a base class.
**/

#if __cplusplus >= 201103L
#define CARTO_OVERRIDE override
#else
#define CARTO_OVERRIDE
#define static_assert(expr, msg)
// in non-c++11 mode, replace unique_ptr with auto_ptr
#include <memory>
#define unique_ptr auto_ptr
#endif

#include <cartobase/config/config.h>

#endif // CARTOBASE_CONFIG_CARTOBASE_CONFIG_H
