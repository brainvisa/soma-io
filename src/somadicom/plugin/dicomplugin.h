#ifndef SOMAIO_PLUGIN_DICOMPLUGIN_H
#define SOMAIO_PLUGIN_DICOMPLUGIN_H


#include <soma-io/config/soma_config.h>
#include <cartobase/plugin/plugin.h>


namespace soma
{
  /// Plugin for DICOM images using Dcmtk
  ///
  /// This plugin requires the installation of Dcmtk library. \n
  ///
  /// Writing to these formats is not supported yet.
  class DicomPlugin : public carto::Plugin
  {

  public:

    DicomPlugin();
    virtual ~DicomPlugin();

    virtual std::string name() const;

    static bool noop();

  };

}

#endif
