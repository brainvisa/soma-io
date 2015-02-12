#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedModalityLutModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedModalityLutModule.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::EnhancedModalityLutModule::EnhancedModalityLutModule()
                               : soma::ModalityLutModule()
{
}


bool soma::EnhancedModalityLutModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmSequenceOfItems* seq = 0;
    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();

      _rescaleIntercept.resize( nItems, 0.0 );
      _rescaleSlope.resize( nItems, 1.0 );

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_PixelValueTransformationSequence,
                                           tmpItem ).good() )
        {

          Float64 tmpDouble;

          if ( tmpItem->findAndGetFloat64( DCM_RescaleIntercept,
                                           tmpDouble ).good() )
          {

            _rescaleIntercept[ i ] = double( tmpDouble );

          }

          if ( tmpItem->findAndGetFloat64( DCM_RescaleSlope, 
                                           tmpDouble ).good() )
          {

            _rescaleSlope[ i ] = double( tmpDouble );

          }

        }

      }

      return true;

    }

  }

  return false;

}

