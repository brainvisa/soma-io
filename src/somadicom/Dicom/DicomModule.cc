#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/DicomModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


soma::DicomModule::DicomModule()
{
}


soma::DicomModule::~DicomModule()
{
}


bool soma::DicomModule::parseDataset( DcmDataset* /* dataset */ )
{

  return true;

}


bool soma::DicomModule::parseHeader( 
                                 soma::DicomDatasetHeader& /* datasetHeader */ )
{

  return true;

}

