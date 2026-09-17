#ifndef CARTOBASE_STREAM_SSTREAM_H
#define CARTOBASE_STREAM_SSTREAM_H

#include <cartobase/config/cartobase_config.h>

#if defined (__GNUC__) && (__GNUC__-0 == 2) && (__GNUC_MINOR__-0 < 96)
// gcc 2.95.2 doesn't provide <sstream>
// gcc 2.95.3, gcc 2.96 and gcc 3.0 have both
// gcc 3.1 warns about <strstream> being obsolete
// gcc 3.2 refuses <strstream>
// (lots of fun...)

// 1st hack: provide a common <cartobase/stream/sstream> include
// (this is what this header does)
// and a way to easily know what we've done
#include <strstream>
#define CARTO_OLDSTYLE_STRSTREAM

// 2nd hack: replace [io]stringstream by [io]strstream
namespace std
{

  typedef istrstream istringstream;
  typedef ostrstream ostringstream;
  typedef strstream stringstream;

}

// This hack supposes you don't use 'expert features' of strstreams
// since the API of strstream and stringstream is different 
// (constructors are already different) !

#else // gcc >= 2.96 and other compilers

// new standard API
#include <sstream>

#endif

#endif
