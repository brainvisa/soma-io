// only needed on Mac bundle
#include <soma-io/plugin/osplugin.h>

using namespace soma;

namespace
{
  // force using libsomaopenslide.dylib on Mac
  bool __osplugin = OSPlugin::noop();
}

