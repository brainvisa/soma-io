
#include <cartobase/smart/mutexrcptr.h>

using namespace carto;


MutexedObject::~MutexedObject()
{
  delete _mutex;
}


