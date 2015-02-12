#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsEnhancedDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/Philips/PhilipsEnhancedDiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::PhilipsEnhancedDiffusionModule::PhilipsEnhancedDiffusionModule()
                                    : soma::DiffusionModule()
{
}


bool soma::PhilipsEnhancedDiffusionModule:: parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    int32_t frameCount, zCount, tIndex = 0;
    OFString tmpString;
    Sint32 tmpInt;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetOFString( DCM_ImageType, tmpString, 2 ).good() )
    {

      if ( tmpString.compare( "DIFFUSION" ) )
      {

        return false;

      }

    }

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, tmpInt ).good() )
    {

      frameCount = int32_t( tmpInt );

    }

    if ( dataset->findAndGetSint32( DcmTagKey( 0x2001, 0x1018 ), 
                                    tmpInt ).good() )
    {

      zCount = int32_t( tmpInt );

    }

    if ( dataset->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();
      DcmTagKey diffGradDir( DCM_DiffusionGradientOrientation );

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );

        if ( item->findAndGetOFString( DCM_DimensionIndexPointer, 
                                       tmpString ).good() )
        {

          if ( !tmpString.compare( diffGradDir.toString() ) )
          {

            tIndex = i;

          }

        }

      }

    }

    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();
      int32_t frames = frameCount / zCount;
      std::vector< int32_t > lut( frames, 0 );

      _bValues.resize( frames );
      _directions.resize( frames );

      for ( i = 0; i < nItems; i++ )
      {

        int32_t index = 0;
        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_FrameContentSequence,
                                           tmpItem ).good() )
        {

          Uint32 t;

          tmpItem->findAndGetUint32( DCM_DimensionIndexValues, t, tIndex );

          index = int32_t( t % frames );

        }

        if ( !lut[ index ] )
        {

          if ( item->findAndGetSequenceItem( DCM_MRDiffusionSequence,
                                             tmpItem ).good() )
          {

            std::vector< double > direction( 3, 0.0 );
            Float64 tmpFloat;
            DcmItem* itemDir = 0;

            if ( tmpItem->findAndGetFloat64( DCM_DiffusionBValue, 
                                             tmpFloat ).good() )
            {

              _bValues[ index ] = double( tmpFloat );

            }

            if ( tmpItem->findAndGetSequenceItem( 
                                         DCM_DiffusionGradientDirectionSequence,
                                         itemDir ).good() )
            {

              int32_t d;

              for ( d = 0; d < 3; d++ )
              {

                if ( itemDir->findAndGetFloat64( 
                                               DCM_DiffusionGradientOrientation,
                                               tmpFloat,
                                               d ).good() )
                {

                  direction[ d ] = double( tmpFloat );

                }

              }

              direction[ 2 ] *= -1.0;

            }

            _directions[ index ] = direction;
            lut[ index ] = 1;

          }

        }

      }

      return true;

    }

  }

  return false;

}


int32_t soma::PhilipsEnhancedDiffusionModule::getStep( 
                                 soma::DicomDatasetHeader& /* datasetHeader */ )
{

  return 1;

}

