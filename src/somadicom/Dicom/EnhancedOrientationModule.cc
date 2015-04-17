#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedOrientationModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedOrientationModule.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::EnhancedOrientationModule::EnhancedOrientationModule()
                              : dcm::OrientationModule()
{
}


bool dcm::EnhancedOrientationModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmItem* seqItem = 0;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequenceItem( DCM_SharedFunctionalGroupsSequence,
                                          seqItem ).good() )
    {

      DcmItem* item = 0;

      if ( seqItem->findAndGetSequenceItem( DCM_PlaneOrientationSequence,
                                            item ).good() )
      {

        if ( parseItem( item ) )
        {

          return true;

        }

      }

    }

    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_PlaneOrientationSequence,
                                           tmpItem ).good() )
        {

          if ( parseItem( tmpItem ) )
          {

            return true;

          }

        }

      }

    }

  }

  return false;

}
