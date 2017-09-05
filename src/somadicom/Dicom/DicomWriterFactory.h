#ifndef _DicomWriterFactory_h_
#define _DicomWriterFactory_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif

#include <string>
#include <map>


namespace dcm
{


class DicomWriter;
class DicomProxy;


class DicomWriterFactory
{

  public:

    DicomWriterFactory();
    virtual ~DicomWriterFactory();

    bool add( DicomWriter* writer );

    bool write( const std::string& fileName,
                DicomProxy& proxy,
                bool forceSecondaryCapture = false );

  protected:

    std::map< std::string, DicomWriter* > _writers;

};


}


#endif
