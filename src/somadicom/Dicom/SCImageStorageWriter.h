#ifndef _SCImageStorageWriter_h_
#define _SCImageStorageWriter_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomWriter.h>
#include <soma-io/Pattern/Singleton.h>
#else
#include <Dicom/DicomWriter.h>
#include <Pattern/Singleton.h>
#endif


namespace dcm
{


class SCImageStorageWriter : public DicomWriter,
                             public Singleton< SCImageStorageWriter >
{

  public:

    std::string getStorageUID();

  protected:

    friend class Singleton< SCImageStorageWriter >;

    SCImageStorageWriter();

    virtual bool writeDatasets( const std::string& directoryName,
                                const std::string& fileName,
                                DicomProxy& proxy );

};


}



#endif
