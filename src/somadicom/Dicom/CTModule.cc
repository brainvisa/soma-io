#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/CTModule.h>
#else
#include <Dicom/CTModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::CTModule::CTModule()
              : soma::DicomModule(),
                _reconstructionDiameter( 0.0 )
{
}


bool soma::CTModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpDouble;

    if ( dataset->findAndGetFloat64( DCM_ReconstructionDiameter, 
                                     tmpDouble ).good() )
    {

      _reconstructionDiameter = double( tmpDouble );

    }

    return true;

  }

  return false;

}


double soma::CTModule::getReconstructionDiameter() const
{

  return _reconstructionDiameter;

}

