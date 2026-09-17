#ifndef SOMAIO_UTILITIES_HEADERTRANSLATOR_H
#define SOMAIO_UTILITIES_HEADERTRANSLATOR_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{

  /// Properties translator
  ///
  /// The translator transforms some specific properties into others. It is  
  /// useful for instance to transform Aims 2.x .minf headers or AimsFileInfo  
  /// output to cartodata-style headers
  class HeaderTranslator
  {
  public:
    virtual ~HeaderTranslator() {}

    /// if \c dstheader is given, \c srcheader is copied to \c dstheader while 
    /// it is translated. If \c dstheader is left to \c none, \c srcheader is 
    /// modified instead.
    virtual void translate( carto::Object srcheader, 
                            carto::Object dstheader = carto::none() ) const = 0;
  };

}

#endif

