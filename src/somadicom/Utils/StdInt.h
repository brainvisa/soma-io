#ifndef _StdInt_h_
#define _StdInt_h_


#ifndef WIN32

#include <stdint.h>

#else

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#ifndef HAVE_NO_TYPEDEF_SSIZE_T
typedef unsigned __int32 ssize_t;
#endif

typedef long off_t;

#endif


#endif
