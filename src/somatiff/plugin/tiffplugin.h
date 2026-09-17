#ifndef SOMAIO_PLUGIN_TIFFPLUGIN_H
#define SOMAIO_PLUGIN_TIFFPLUGIN_H

#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>

namespace soma
{
  /// Plugin for TIFF images
  ///
  /// This plugin requires the external library libtiff.\n
  /// TIFF files store 2D files. 3D volumes can be stored as independent
  /// numbered axial slices (i.e. : file_001.tiff file_002.tiff ...). 
  /// Additional meta information can be stored into an additional \c minf file.
  class TiffPlugin : public carto::Plugin
  {
  public:
    TiffPlugin();
    virtual ~TiffPlugin();
    virtual std::string name() const;
    static bool noop();
  };

}

#endif

