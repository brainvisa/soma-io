#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/DicomModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>


dcm::DicomModule::DicomModule()
{
}


dcm::DicomModule::~DicomModule()
{
}


bool dcm::DicomModule::parseItem( DcmItem* /* dcmItem */ )
{

  return true;

}


bool dcm::DicomModule::parseHeader( 
                                  dcm::DicomDatasetHeader& /* datasetHeader */ )
{

  return true;

}
