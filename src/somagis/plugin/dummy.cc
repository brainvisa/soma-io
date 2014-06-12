// only needed on Mac bundle
#include <soma-io/plugin/gisplugin.h>

using namespace soma;

namespace
{
  // force using libsomagis.dylib on Mac
  bool __gisplugin = GisPlugin::noop();
}

