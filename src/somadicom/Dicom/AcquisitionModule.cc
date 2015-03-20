#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/AcquisitionModule.h>
#else
#include <Dicom/AcquisitionModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::AcquisitionModule::AcquisitionModule()
                       : soma::DicomModule(),
                         _sliceThickness( 0.0 ),
                         _spacingBetweenSlices( 0.0 )
{
}


bool soma::AcquisitionModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Float64 tmpDouble;

    if ( dcmItem->findAndGetFloat64( DCM_SliceThickness, tmpDouble ).good() )
    {

      _sliceThickness = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_SpacingBetweenSlices,
                                     tmpDouble ).good() )
    {

      _spacingBetweenSlices = double( tmpDouble );

    }

    return true;

  }

  return false;

}


double soma::AcquisitionModule::getSliceThickness() const
{

  return _sliceThickness;

}


double soma::AcquisitionModule::getSpacingBetweenSlices() const
{

  return _spacingBetweenSlices;

}

