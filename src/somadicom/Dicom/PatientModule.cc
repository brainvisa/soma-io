#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/PatientModule.h>
#else
#include <Dicom/PatientModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


dcm::PatientModule::PatientModule()
                  : dcm::DicomModule(),
                    _name( "" ),
                    _id( "" ),
                    _birthDate( "" ),
                    _sex( "" ),
                    _age( "" ),
                    _weight( 0.0 )
{
}


bool dcm::PatientModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;
    Float64 tmpDouble;

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
    if ( dcmItem->findAndGetOFString( DCM_PatientName, tmpString ).good() )
#else
    if ( dcmItem->findAndGetOFString( DCM_PatientsName, tmpString ).good() )
#endif
    {

      _name = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_PatientID, tmpString ).good() )
    {

      _id = tmpString.c_str();

    }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
    if ( dcmItem->findAndGetOFString( DCM_PatientBirthDate, tmpString ).good() )
#else
    if ( dcmItem->findAndGetOFString( DCM_PatientsBirthDate, 
                                      tmpString ).good() )
#endif
    {

      _birthDate = tmpString.c_str();

    }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
    if ( dcmItem->findAndGetOFString( DCM_PatientSex, tmpString ).good() )
#else
    if ( dcmItem->findAndGetOFString( DCM_PatientsSex, tmpString ).good() )
#endif
    {

      _sex = tmpString.c_str();

    }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
    if ( dcmItem->findAndGetOFString( DCM_PatientAge, tmpString ).good() )
#else
    if ( dcmItem->findAndGetOFString( DCM_PatientsAge, tmpString ).good() )
#endif
    {

      _age = tmpString.c_str();

    }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
    if ( dcmItem->findAndGetFloat64( DCM_PatientWeight, tmpDouble ).good() )
#else
    if ( dcmItem->findAndGetFloat64( DCM_PatientsWeight, tmpDouble ).good() )
#endif
    {

      _weight = double( tmpDouble );

    }

    return true;

  }

  return false;

}


const std::string& dcm::PatientModule::getName() const
{

  return _name;

}


const std::string& dcm::PatientModule::getId() const
{

  return _id;

}


const std::string& dcm::PatientModule::getBirthDate() const
{

  return _birthDate;

}


const std::string& dcm::PatientModule::getSex() const
{

  return _sex;

}


const std::string& dcm::PatientModule::getAge() const
{

  return _age;

}


double dcm::PatientModule::getWeight() const
{

  return _weight;

}
