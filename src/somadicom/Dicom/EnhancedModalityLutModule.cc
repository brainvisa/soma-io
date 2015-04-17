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


dcm::EnhancedModalityLutModule::EnhancedModalityLutModule()
                              : dcm::ModalityLutModule()
{
}


bool dcm::EnhancedModalityLutModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmItem* seqItem = 0;

    if ( dataset->findAndGetSequenceItem( DCM_SharedFunctionalGroupsSequence,
                                          seqItem ).good() )
    {

      DcmItem* item = 0;

      if ( seqItem->findAndGetSequenceItem( 
                                           DCM_PixelValueTransformationSequence,
                                           item ).good() )
      {

        _rescaleIntercept.clear();
        _rescaleSlope.clear();

        if ( parseItem( item ) )
        {

          return true;

        }

      }

    }

    DcmSequenceOfItems* seq = 0;
    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();

      _rescaleIntercept.clear();
      _rescaleSlope.clear();

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_PixelValueTransformationSequence,
                                           tmpItem ).good() )
        {

          if ( !parseItem( tmpItem ) )
          {

            _rescaleIntercept.push_back( 0.0 );
            _rescaleSlope.push_back( 1.0 );

          }

        }

      }

      return true;

    }

  }

  return false;

}
