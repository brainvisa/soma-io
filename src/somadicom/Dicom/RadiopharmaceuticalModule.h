#ifndef _RadiopharmaceuticalModule_h_
#define _RadiopharmaceuticalModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif

#include <string>


namespace soma
{


class RadiopharmaceuticalModule : public DicomModule
{

  public:

    RadiopharmaceuticalModule();

    bool parseItem( DcmItem* dcmItem );

    const std::string& getRadiopharmaceutical() const;
    const std::string& getRadiopharmaceuticalStartTime() const;
    double getRadionuclideTotalDose() const;
    double getRadionuclideHalfLife() const;
    double getRadionuclidePositronFraction() const;
    const std::string& getRadionuclide() const;

  private:

    std::string _radiopharmaceutical;
    std::string _radiopharmaceuticalStartTime;
    double _radionuclideTotalDose;
    double _radionuclideHalfLife;
    double _radionuclidePositronFraction;
    std::string _radionuclide;

};


}


#endif

