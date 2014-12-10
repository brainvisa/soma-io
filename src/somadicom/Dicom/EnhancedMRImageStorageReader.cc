#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/MultiSliceContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcsequen.h>


soma::EnhancedMRImageStorageReader::EnhancedMRImageStorageReader()
                                  : soma::MultiSliceReader()
{
}


std::string soma::EnhancedMRImageStorageReader::getStorageUID()
{

  return UID_EnhancedMRImageStorage;

}


bool soma::EnhancedMRImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Sint32 nSlices = 1;

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, nSlices ).good() )
    {

      m_dataInfo->m_slices = (int32_t)nSlices;

    }

    DcmSequenceOfItems* seq = 0;
    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();
      bool found = false;

      m_dataInfo->m_slope.resize( nItems );
      m_dataInfo->m_intercept.resize( nItems );

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( !found && 
             item->findAndGetSequenceItem( DCM_PlaneOrientationSequence,
                                           tmpItem ).good() )
        {

          OFString tmpString;

          if ( tmpItem->findAndGetOFStringArray( DCM_ImageOrientationPatient,
                                                 tmpString ).good() )
          {

            std::string orientationStr = tmpString.c_str();
            size_t pos = orientationStr.find( "\\" );

            while ( pos != std::string::npos )
            {

              orientationStr[ pos ] = ' ';
              pos = orientationStr.find( "\\" );

            }

            std::istringstream iss( orientationStr );

            iss >> m_dataInfo->m_rowVec.x
                >> m_dataInfo->m_rowVec.y
                >> m_dataInfo->m_rowVec.z
                >> m_dataInfo->m_colVec.x
                >> m_dataInfo->m_colVec.y
                >> m_dataInfo->m_colVec.z;

            found = true;

          }

        }

        if ( item->findAndGetSequenceItem( DCM_PlanePositionSequence,
                                           tmpItem ).good() )
        {

          OFString tmpString;

          if ( tmpItem->findAndGetOFStringArray( DCM_ImagePositionPatient,
                                                 tmpString ).good() )
          {

            std::string positionStr = tmpString.c_str();
            size_t pos = positionStr.find( "\\" );

            while ( pos != std::string::npos )
            {

              positionStr[ pos ] = ' ';
              pos = positionStr.find( "\\" );

            }

            soma::Vector position;
            std::istringstream iss( positionStr );

            iss >> position.x
                >> position.y
                >> position.z;

            m_positions.push_back( position );

          }

        }

        if ( item->findAndGetSequenceItem( DCM_PixelValueTransformationSequence,
                                           tmpItem ).good() )
        {

          Float64 tmpFloat;

          if ( tmpItem->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
          {

            m_dataInfo->m_slope[ i ] = (double)tmpFloat;

          }

          if ( tmpItem->findAndGetFloat64( DCM_RescaleIntercept,
                                           tmpFloat ).good() )
          {

            m_dataInfo->m_intercept[ i ] = (double)tmpFloat;

          }

        }

      }

    }

    setOrientation();

    return true;

  }

  return false;

}


bool soma::EnhancedMRImageStorageReader::readData( soma::DicomProxy& proxy,
                                                   soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    if ( progress )
    {

      progress->execute( 2 );

    }

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_slices[ 0 ].c_str() ).good() )
    {

      if ( progress )
      {

        progress->execute( 6 );

      }

      soma::DataInfo& info = proxy.getDataInfo();
      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, 
                           dataset->getOriginalXfer(),
                           CIF_IgnoreModalityTransformation );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        int32_t tmp1, tmp2, nSlices;

        info.m_modalityLUTApplied = false;
        info.m_pixelRepresentation = 
                                   dcmImage.getInterData()->getRepresentation();
        info.m_patientOrientation.getOnDiskSize( tmp1, tmp2, nSlices );

        soma::MultiSliceContext context( dcmImage, proxy, progress );
        carto::ThreadedLoop threadedLoop( &context, 0, nSlices );

        threadedLoop.launch();

        if ( info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info.m_minimum = int32_t( min );
          info.m_maximum = int32_t( max );

        }

        soma::DicomDatasetHeader datasetHeader( proxy );
        datasetHeader.add( dataset, 0 );

        if ( progress )
        {

          progress->execute( 100 );

        }

        return true;

      }

    }

  }

  return false;

}
