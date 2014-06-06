// only needed on Mac bundle
#include <soma-io/plugin/niftiplugin.h>

using namespace soma;

namespace
{
  // force using libaimsdicom.dylib on Mac
  bool __niftiplugin = NiftiPlugin::noop();
}

