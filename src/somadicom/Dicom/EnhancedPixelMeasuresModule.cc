#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedPixelMeasuresModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedPixelMeasuresModule.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcsequen.h>


dcm::EnhancedPixelMeasuresModule::EnhancedPixelMeasuresModule()
                                : dcm::DicomModule(),
                                  _sliceThickness( 0.0 ),
                                  _pixelSpacingX( 1.0 ),
                                  _pixelSpacingY( 1.0 )
{
}


bool dcm::EnhancedPixelMeasuresModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Float64 tmpDouble;

    if ( dcmItem->findAndGetFloat64( DCM_SliceThickness, tmpDouble ).good() )
    {

      _sliceThickness = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 0 ).good() )
    {

      _pixelSpacingX = double( tmpDouble );

    }

    if ( dcmItem->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 1 ).good() )
    {

      _pixelSpacingY = double( tmpDouble );

      return ( _sliceThickness > 0.0 );

    }

  }

  return false;

}


bool dcm::EnhancedPixelMeasuresModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmItem* seqItem = 0;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequenceItem( DCM_SharedFunctionalGroupsSequence,
                                          seqItem ).good() )
    {

      DcmItem* item = 0;

      if ( seqItem->findAndGetSequenceItem( DCM_PixelMeasuresSequence,
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

        if ( item->findAndGetSequenceItem( DCM_PixelMeasuresSequence,
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


double dcm::EnhancedPixelMeasuresModule::getSliceThickness() const
{

  return _sliceThickness;

}


double dcm::EnhancedPixelMeasuresModule::getPixelSpacingX() const
{

  return _pixelSpacingY;

}


double dcm::EnhancedPixelMeasuresModule::getPixelSpacingY() const
{

  return _pixelSpacingX;

}
