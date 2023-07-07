// only needed on Mac bundle
#include <soma-io/plugin/mifplugin.h>

using namespace soma;

namespace
{
  // force using libaimsdicom.dylib on Mac
  bool __mifplugin = MifPlugin::noop();
}

