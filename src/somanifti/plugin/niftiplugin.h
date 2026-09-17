#ifndef SOMAIO_PLUGIN_NIFTIPLUGIN_H
#define SOMAIO_PLUGIN_NIFTIPLUGIN_H

#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>

namespace soma
{
  /// Plugin for NIFTI images
  class NiftiPlugin : public carto::Plugin
  {
  public:
    NiftiPlugin();
    virtual ~NiftiPlugin();
    virtual std::string name() const;
    static bool noop();
  };

}

#endif

