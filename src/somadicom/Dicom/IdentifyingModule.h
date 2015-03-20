#ifndef _IdentifyingModule_h_
#define _IdentifyingModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Utils/StdInt.h>
#endif

#include <string>
#include <vector>


namespace soma
{


class IdentifyingModule : public DicomModule
{

  public:

    IdentifyingModule();

    bool parseItem( DcmItem* dcmItem );
    bool parseHeader( DicomDatasetHeader& datasetHeader );

    const std::string& getStudyDate() const;
    const std::string& getSeriesDate() const;
    const std::string& getAcquisitionDate() const;
    const std::string& getStudyTime() const;
    const std::string& getSeriesTime() const;
    const std::vector< std::string >& getAcquisitionTimes() const;
    const std::string& getModality() const;
    const std::string& getManufacturer() const;
    const std::string& getInstitutionName() const;
    const std::string& getSeriesDescription() const;
    int32_t getZeroStartTime() const;

  private:

    std::string _studyDate;
    std::string _seriesDate;
    std::string _acquisitionDate;
    std::string _studyTime;
    std::string _seriesTime;
    std::vector< std::string > _acquisitionTimes;
    std::string _modality;
    std::string _manufacturer;
    std::string _institutionName;
    std::string _seriesDescription;
    int32_t _zeroStartTime;

}; 


}


#endif

