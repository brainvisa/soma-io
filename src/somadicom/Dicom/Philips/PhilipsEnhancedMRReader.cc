#include <soma-io/Dicom/Philips/PhilipsEnhancedMRReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PhilipsEnhancedMRReader::PhilipsEnhancedMRReader()
                             : soma::EnhancedMRImageStorageReader(),
                               soma::Singleton< 
                                              soma::PhilipsEnhancedMRReader >(),
                               m_tIndex( 0 )
{
}


soma::PhilipsEnhancedMRReader::~PhilipsEnhancedMRReader()
{
}


std::string soma::PhilipsEnhancedMRReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool soma::PhilipsEnhancedMRReader::getHeader( 
                                       soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::EnhancedMRImageStorageReader::getHeader( proxy, 
                                                       info, 
                                                       datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    DcmDataset dataset;
    DcmSequenceOfItems* seq = 0;

    datasetHeader.get( dataset );

    if ( dataset.findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                     seq ).good() )
    {

      uint32_t i, nItems = seq->card();
      DcmSequenceOfItems* seqDiff = 0;
      DcmItem* itemDiff = seq->getItem( 0 );

      if ( itemDiff->findAndGetSequence( DCM_MRDiffusionSequence, 
                                         seqDiff ).good() )
      {

        int32_t frameCount = info.m_frames;
        std::vector< double > bValues( frameCount, 0.0 );
        std::vector< std::vector< double > > directions( frameCount );
        std::vector< int32_t > lut( frameCount, 0 );

        for ( i = 0; i < nItems; i++ )
        {

          DcmItem* item = seq->getItem( i );
          DcmItem* tmpItem = 0;

          if ( item->findAndGetSequenceItem( DCM_FrameContentSequence,
                                             tmpItem ).good() )
          {

            Uint32 tmpUint;

            if ( tmpItem->findAndGetUint32( DCM_DimensionIndexValues,
                                            tmpUint,
                                            m_tIndex ).good() )
            {

              int32_t index = int32_t( tmpUint ) % frameCount;

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

                    bValues[ index ] = double( tmpFloat );

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

                  }

                  directions[ index ] = direction;
                  lut[ index ] = 1;

                }

              }

            }

          }

        }

        if ( bValues.size() )
        {

          proxy.addAttribute( "b_values", bValues );

        }

        if ( directions.size() )
        {

          proxy.addAttribute( "diffusion_directions", directions );

        }

      }

    }

  }

  return true;

}


bool soma::PhilipsEnhancedMRReader::buildIndexLut( DcmDataset* dataset )
{

  if ( dataset )
  {

    int32_t nItems = 0;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      nItems = seq->card();

      if ( nItems < 4 )
      {

        return soma::EnhancedMRImageStorageReader::buildIndexLut( dataset );

      }

    }

    OFString tmpString;

    if ( dataset->findAndGetOFString( DCM_ImageType, tmpString, 2 ).good() )
    {

      if ( !tmpString.compare( "DIFFUSION" ) )
      {

        int32_t zIndex = 0;
        int32_t frameCount = m_dataInfo->m_slices;
        int32_t i;
        DcmTagKey inStackPos( DCM_InStackPositionNumber );
        DcmTagKey diffGradDir( DCM_DiffusionGradientOrientation );

        for ( i = 0; i < nItems; i++ )
        {

          DcmItem* item = seq->getItem( i );

           if ( item->findAndGetOFString( DCM_DimensionIndexPointer, 
                                          tmpString ).good() )
           {

             if ( !tmpString.compare( inStackPos.toString() ) )
             {

               zIndex = i;

             }
             else if ( !tmpString.compare( diffGradDir.toString() ) )
             {

               m_tIndex = i;

             }

           }

        }

        if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence,
                                          seq ).good() )
        {

          uint32_t i, nItems = seq->card();
          std::vector< std::pair< int32_t, int32_t > > tmpLut( nItems );
          int32_t zMax = 0;

          for ( i = 0; i < nItems; i++ )
          {

            DcmItem* item = seq->getItem( i );
            DcmItem* tmpItem = 0;

            if ( item->findAndGetSequenceItem( DCM_FrameContentSequence,
                                               tmpItem ).good() )
            {

              Uint32 z, t;

              tmpItem->findAndGetUint32( DCM_DimensionIndexValues, z, zIndex );
              tmpItem->findAndGetUint32( DCM_DimensionIndexValues, 
                                         t, 
                                         m_tIndex );

              if  ( int32_t( z ) > zMax )
              {

                zMax = z;

              }

              tmpLut[ i ] = std::make_pair( z - 1, t );

            }

          }

          if ( zMax < frameCount )
          {

            m_dataInfo->m_frames = frameCount / zMax;
            m_dataInfo->m_slices = zMax;

          }

          for ( i = 0; i < nItems; i++ )
          {

            m_indexLut[ i ] = 
                          ( tmpLut[ i ].second % m_dataInfo->m_frames ) * zMax +
                          tmpLut[ i ].first;

          }

        }
 
        return true;

      }

    }

  }

  return false;

}


RegisterDicomReaderFunction( PhilipsEnhancedMRReader );

