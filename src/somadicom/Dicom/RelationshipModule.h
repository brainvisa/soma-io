#ifndef _RelationshipModule_h_
#define _RelationshipModule_h_


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


class RelationshipModule : public DicomModule
{

  public:

    RelationshipModule();

    bool parseItem( DcmItem* dcmItem );
    bool parseHeader( DicomDatasetHeader& datasetHeader );

    const std::string& getSeriesInstanceUID() const;
    const std::string& getStudyId() const;
    int32_t getSeriesNumber() const;
    const std::vector< int32_t >& getAcquisitionNumber() const;

  private:

    std::string _seriesInstanceUID;
    std::string _studyId;
    int32_t _seriesNumber;
    std::vector< int32_t > _acquisitionNumber;

};

}


#endif

