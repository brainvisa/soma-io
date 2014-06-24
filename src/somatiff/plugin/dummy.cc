// only needed on Mac bundle
#include <soma-io/plugin/tiffplugin.h>

using namespace soma;

namespace
{
  // force using libsomatiff.dylib on Mac
  bool __tiffplugin = TiffPlugin::noop();
}
