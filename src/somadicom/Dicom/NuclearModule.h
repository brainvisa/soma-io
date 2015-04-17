#ifndef _NuclearModule_h_
#define _NuclearModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Utils/StdInt.h>
#endif

#include <string>
#include <map>


namespace dcm
{


class NuclearModule : public DicomModule
{

  public:

    NuclearModule();

    virtual bool parseItem( DcmItem* dcmItem );

    double getScaleFactor() const;
    int32_t getNumberOfTimeSlots() const;
    int32_t getNumberOfSlices() const;
    int32_t getNumberOfTimeSlices() const;
    const std::string& getAcquisitionMode() const;
    const std::string& getCategory() const;
    const std::string& getUnits() const;
    const std::string& getCorrectionMethod() const;
    const std::string& getReconstructionMethod() const;

  protected:

    double _scaleFactor;
    int32_t _numberOfTimeSlots;
    int32_t _numberOfSlices;
    int32_t _numberOfTimeSlices;
    std::string _acquisitionMode;
    std::string _category;
    std::string _units;
    std::string _correctionMethod;
    std::string _reconstructionMethod;

    std::map< std::string, std::string > _unitNames;

};


}


#endif
