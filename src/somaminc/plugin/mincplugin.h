#ifndef SOMAIO_PLUGIN_MINCPLUGIN_H
#define SOMAIO_PLUGIN_MINCPLUGIN_H

#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>

namespace soma
{
  /// Plugin for MINC images
  class MincPlugin : public carto::Plugin
  {
  public:
    MincPlugin();
    virtual ~MincPlugin();
    virtual std::string name() const;
    static bool noop();
  };

}

#endif

