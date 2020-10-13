#ifndef CARTOBASE_STREAM_STRINGUTIL_H
#define CARTOBASE_STREAM_STRINGUTIL_H

#include <string>
#include <vector>

namespace carto
{
  class Object;

  /// String utilities
  class StringUtil
  {
  public:
    static std::vector<std::string>
    split( const std::string & s, const std::string & split_pattern,
           unsigned maxsplit=0 );
    static std::vector<std::string>
    split( const std::string & s,
           const std::vector<std::string> & split_patterns,
           unsigned maxsplit=0 );
    static std::string join( const std::vector<std::string> & strings,
                             const std::string &join_by );
    static std::string strip( const std::string & s,
                              const std::string & chars = " \n\t" );
  };


}

#endif

