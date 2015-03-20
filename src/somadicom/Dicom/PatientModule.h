#ifndef _PatientModule_h_
#define _PatientModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif

#include <string>


namespace soma
{


class PatientModule : public DicomModule
{

  public:

    PatientModule();

    bool parseItem( DcmItem* dcmItem );

    const std::string& getName() const;
    const std::string& getId() const;
    const std::string& getBirthDate() const;
    const std::string& getSex() const;
    const std::string& getAge() const;
    double getWeight() const;

  private:

    std::string _name;
    std::string _id;
    std::string _birthDate;
    std::string _sex;
    std::string _age;
    double _weight;

};


}


#endif

