
#include <cartobase/stream/stringutil.h>
#include <iostream>

using namespace std;
using namespace carto;


std::vector<std::string>
StringUtil::split( const std::string & s, const std::string & split_pattern,
  unsigned maxsplit )
{
  char c;
  string cur;
  size_t i, last = 0, m = split_pattern.length(), n = s.length() - m;
  vector<string> split;
  unsigned ns = 0;

  for( i=0; i<n && ( maxsplit == 0 || ns<maxsplit ); ++i )
  {
    c = s[i];
    cur += c;
    if( cur.length() >= m
        && cur.substr( cur.length() - m, m ) == split_pattern )
    {
      split.push_back( s.substr( last, i - last - m + 1 ) );
      last = i + 1;
      cur.clear();
      ++ns;
    }
  }
  if( last < n )
    split.push_back( s.substr( last, s.length() - last ) );

  return split;
}


std::vector<std::string>
StringUtil::split( const std::string & s,
                   const std::vector<std::string> & split_patterns,
                   unsigned maxsplit )
{
}


std::string
StringUtil::join( const std::vector<std::string> & strings,
                  const std::string &join_by )
{
  string res;
  vector<string>::const_iterator is, es=strings.end();
  for( is=strings.begin(); is!=es; ++is )
  {
    if( !res.empty() )
      res += join_by;
    res += *is;
  }

  return res;
}


std::string StringUtil::strip( const std::string & s,
                               const std::string & chars )
{
  size_t i, n = s.length(), start = 0, stop = 0;
  bool begin = true;
  bool end = false;
  char c;

  for( i=0; i<n; ++i )
  {
    c = s[i];
    if( chars.find( c ) != string::npos )
    {
      if( begin )
      {
        start = i + 1;
      }
      else
      {
        if( !end )
        {
          end = true;
          stop = i;
        }
      }
    }
    else
    {
      begin = false;
      end = false;
    }
  }
  if( !end )
    stop = n;

  return s.substr( start, stop - start );
}


