// only needed on Mac bundle
#include <soma-io/plugin/dicomplugin.h>

using namespace soma;

namespace
{
  // force using libsomadicom.dylib on Mac
  bool __dicomplugin = DicomPlugin::noop();
}

