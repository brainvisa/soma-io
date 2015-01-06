#include <soma-io/Dicom/MRImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::MRImageStorageReader::MRImageStorageReader()
                          : soma::MultiFileReader()
{
}


std::string soma::MRImageStorageReader::getStorageUID()
{

  return UID_MRImageStorage;

}


bool soma::MRImageStorageReader::getHeader(
                                       soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MultiSliceReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  proxy.addAttribute( "tr", info.m_repetitionTime );

  DcmDataset dataset;
  DcmSequenceOfItems* seq = 0;
  Float64 tmpFloat;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetFloat64( DCM_EchoTime, tmpFloat ).good() )
  {

    proxy.addAttribute( "te", double( tmpFloat ) );

  }

  if ( dataset.findAndGetFloat64( DCM_InversionTime, tmpFloat ).good() )
  {

    proxy.addAttribute( "ti", double( tmpFloat ) );

  }

  if ( dataset.findAndGetFloat64( DCM_FlipAngle, tmpFloat ).good() )
  {

    proxy.addAttribute( "flip_angle", double( tmpFloat ) );

  }

  if ( dataset.findAndGetSequence( DCM_MRDiffusionSequence, seq ).good() )
  {

    int32_t i, n = datasetHeader.size();
    std::vector< double > bValues;
    std::vector< std::vector< double > > directions;

    for ( i = 0; i < n; i++ )
    {

      datasetHeader.get( dataset, i );

      if ( dataset.findAndGetSequence( DCM_MRDiffusionSequence, seq ).good() )
      {

        Float64 tmpFloat;
        DcmSequenceOfItems* seqDir = 0;
        DcmItem* item = seq->getItem( 0 );

        if ( item->findAndGetFloat64( DCM_DiffusionBValue, tmpFloat ).good() )
        {

          bValues.push_back( double( tmpFloat ) );

        }

        if ( item->findAndGetSequence( DCM_DiffusionGradientDirectionSequence,
                                       seqDir ).good() )
        {

          int32_t d;
          std::vector< double > direction( 3, 0.0 );
          DcmItem* itemDir = seqDir->getItem( 0 );

          for ( d = 0; d < 3; d++ )
          {

            if ( itemDir->findAndGetFloat64( DCM_DiffusionGradientOrientation,
                                             tmpFloat,
                                             d ).good() );
            {

              direction[ d ] = double( tmpFloat );

            }

          }

          directions.push_back( direction );

        }

      }

    }

    if ( bValues.size() )
    {

      proxy.addAttribute( "b_values", bValues );

    }

    if ( directions.size() )
    {

      proxy.addAttribute( "diffusion_direction", directions );

    }

  }

  return true;

}


bool soma::MRImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpFloat;

    if ( dataset->findAndGetFloat64( DCM_RepetitionTime, tmpFloat ).good() )
    {

      m_dataInfo->m_repetitionTime= (double)tmpFloat / 1000.0;

    }

    return soma::MultiFileReader::readHeader( dataset );

  }

  return false;

}


bool soma::MRImageStorageReader::readData( soma::DicomProxy& proxy,
                                           soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    soma::DicomDataContext context( m_slices, proxy, true, progress );
    carto::ThreadedLoop threadedLoop( &context,
                                     0,
                                     int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}
