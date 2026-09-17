#include <cartobase/exception/assert.h>
#include <iostream>
using namespace std;
using namespace carto;

assert_error::assert_error( const char* assertion, const char* file, int line )
             : logic_error( assertion ), _file( file ), _line( line )
{
}

assert_error::~assert_error() throw()
{
}


const char* assert_error::file() const
{
  return _file.c_str();
}


int assert_error::line() const throw()
{
  return _line;
}

namespace carto
{

// Function called when assert failed (it allows breakpoint setting)
void assert_failed( const char * expression,
                    const char * fileName,
                    int lineNumber )
{
  cerr << "A.I.M.S. assertion failed: " << expression
       << ", file " << fileName
       << ", line " << lineNumber
       << endl;
  throw assert_error( expression, fileName, lineNumber );
}

} // namespace carto
