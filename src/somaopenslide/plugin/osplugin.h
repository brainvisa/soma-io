#ifndef SOMAIO_PLUGIN_OSPLUGIN_H
#define SOMAIO_PLUGIN_OSPLUGIN_H

#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>

namespace soma
{
  /// Plugin for virtual microscopy images using OpenSlide
  ///
  /// This plugin requires the installation of openslide library version
  /// 3.3 or higher (only tested with openslide 3.3.3). \n
  /// OpenSlide is an open source C library which allows to read
  /// partially and at multiple resolutions into a pyramidal image file.
  /// Those files are used for virtual microscopy purpose. They allow
  /// storing of large images and several downsampled copies. These
  /// outputs aren't standardized and most constructors use specific
  /// proprietary formats which can be based on TIFF or on multiple
  /// JPEG tiles.\n
  ///
  /// This plugin can read files from vendors Aperio (\c svs, \c tif),
  /// Hamamatsu (\c vms, \c vmu), Leica (\c scn), MIRAX (\c mrxs),
  /// Trestle (\c tif). It can also read generic tiled TIFF (\c tif). \n
  /// The use of BigTIFF based formats (Leica) requires libTIFF
  /// version 4 or higher.\n
  /// Hamamatsu files consist in a ASCII header and multiple
  /// image files. Only those based on TIFF or JPEG can currently be read,
  /// as opposed to RAW images.\n
  /// To chose the resolution to read, use the option \c "resolution_level"
  /// (default value being 0 : the maximum resolution contained in the
  /// file) \n
  /// Writing to these formats is not supported.
  class OSPlugin : public carto::Plugin
  {
  public:
    OSPlugin();
    virtual ~OSPlugin();
    virtual std::string name() const;
    static bool noop();
  };

}

#endif

