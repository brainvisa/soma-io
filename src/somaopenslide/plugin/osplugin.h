/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

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

