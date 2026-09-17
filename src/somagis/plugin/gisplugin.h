#ifndef SOMAIO_PLUGIN_GISPLUGIN_H
#define SOMAIO_PLUGIN_GISPLUGIN_H

#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>

namespace soma
{
  /// Plugin for GIS images
  ///
  /// This plugin does not require any external library. You can use it
  /// as an example when implementing any other i/o plugin.\n
  /// GIS files store uncompressed 4D volumes as two files : a \c dim ASCII
  /// header and a \c ima binary or ascii data file. Additional meta
  /// information can be stored into an additional \c minf file.
  class GisPlugin : public carto::Plugin
  {
  public:
    GisPlugin();
    virtual ~GisPlugin();
    virtual std::string name() const;
    static bool noop();
  };

}

#endif

