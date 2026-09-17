#ifndef SOMAIO_PLUGIN_MIFPLUGIN_H
#define SOMAIO_PLUGIN_MIFPLUGIN_H

#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>

namespace soma
{
  /// Plugin for MIF images
  class MifPlugin : public carto::Plugin
  {
  public:
    MifPlugin();
    virtual ~MifPlugin();
    virtual std::string name() const;
    static bool noop();
  };

}

#endif

