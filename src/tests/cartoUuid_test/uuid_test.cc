#include <cstdlib>

#define private public // don't have time to augment the API for testing...
#include <cartobase/uuid/uuid.h>
#include <cartobase/exception/assert.h>


int main()
{

  carto::UUID uuid;
  ASSERT( uuid.isNull() );
  carto::UUID uuid1;
  uuid1.generate();
  ASSERT( !uuid1.isNull() );

#if defined(__linux) || defined(__osf__)

  ASSERT( ( uuid1.data[ 10 ] & 0x01 ) == 0);

#elif !defined(_WIN32)

  ASSERT( ( uuid1.data[ 10 ] & 0x01 ) != 0);

#endif

  carto::UUID uuid2;
  uuid2.generate();
  ASSERT( !uuid2.isNull() );

#if defined(__linux) || defined(__osf__)

  ASSERT( ( uuid2.data[ 10 ] & 0x01 ) == 0);

#elif !defined(_WIN32)

  ASSERT( ( uuid2.data[ 10 ] & 0x01 ) != 0);

#endif

  ASSERT( !( uuid1 == uuid2 ) );
  std::string input( "1d827ae8-57ca-45bb-871d-c3e080495795" );
  carto::UUID uuid3( input );
  std::string output = uuid3.toString();
  ASSERT( input == output );
  carto::UUID uuid4( uuid );
  ASSERT( uuid4 == uuid );
  uuid1 = uuid;
  ASSERT( uuid1 == uuid );

  return EXIT_SUCCESS;
}
