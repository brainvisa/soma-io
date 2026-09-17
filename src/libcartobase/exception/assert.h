#ifndef CARTOBASE_EXCEPTION_ASSERT_H
#define CARTOBASE_EXCEPTION_ASSERT_H

#include <cartobase/config/cartobase_config.h>
#include <stdexcept>   // logic_error
#include <string>


namespace carto {


class assert_error : public std::logic_error
{
  public:
    assert_error(const char* assertion, const char* file, int line);
    virtual ~assert_error() throw();

    const char* file() const;
    int line() const throw();

  private:
    std::string _file;
    int    _line;
};




#undef ASSERT

// Function called when assert failed (it allows breakpoint setting)
#if defined( __GNUC__ ) \
  && ( __GNUC__ > 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ >= 3) )
// cold attribute is defined from gcc 4.3
void assert_failed( const char *, const char *, int )
  __attribute__((noreturn, cold, nonnull));
#else
void assert_failed( const char *, const char *, int );
#endif

inline void test_assert( bool x, const char * ex, const char *file, int line )
  __attribute__((nonnull));
inline void test_assert( bool x, const char * ex, const char *file, int line )
{
  if( !x ) carto::assert_failed( ex, file, line );
}

#define ASSERT( EX ) ( carto::test_assert( (EX), #EX, __FILE__, __LINE__ ) )


} // namespace carto
#endif
