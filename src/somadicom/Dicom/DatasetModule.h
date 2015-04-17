#ifndef _DatasetModule_h_
#define _DatasetModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Utils/StdInt.h>
#endif

#include <string>


namespace dcm
{


class DatasetModule : public DicomModule
{

  public:

    DatasetModule();

    bool parseItem( DcmItem* dcmItem );

    const std::string& getManufacturer() const;
    const std::string& getSOPClassUID() const;
    const std::string& getSeriesInstanceUID() const;
    int32_t getSamplesPerPixel() const;
    int32_t getBitsAllocated() const;

  private:

    std::string _manufacturer;
    std::string _sopClassUID;
    std::string _seriesInstanceUID;
    int32_t _samplesPerPixel;
    int32_t _bitsAllocated;

}; 


}


#endif
