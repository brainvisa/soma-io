#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
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

#include <map>


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

    Sint32 frameCount = 1;

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, frameCount ).good() )
    {

      m_dataInfo->m_slices = (int32_t)frameCount;

    }

    m_indexLut.resize( frameCount );

    if ( !buildIndexLut( dataset ) )
    {

      return false;

    }

    DcmSequenceOfItems* seq = 0;
    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();
      bool found = false;

      m_positions.resize( nItems );
      m_dataInfo->m_slope.resize( nItems, 1.0 );
      m_dataInfo->m_intercept.resize( nItems, 0.0 );

      for ( i = 0; i < nItems; i++ )
      {

        int32_t index = m_indexLut[ i ];
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

            m_positions[ index ] = position;

          }

        }

        if ( item->findAndGetSequenceItem( DCM_PixelValueTransformationSequence,
                                           tmpItem ).good() )
        {

          Float64 tmpFloat;

          if ( tmpItem->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
          {

            m_dataInfo->m_slope[ index ] = (double)tmpFloat;

          }

          if ( tmpItem->findAndGetFloat64( DCM_RescaleIntercept,
                                           tmpFloat ).good() )
          {

            m_dataInfo->m_intercept[ index ] = (double)tmpFloat;

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

        soma::MultiSliceContext context( dcmImage, 
                                         proxy, 
                                         m_indexLut, 
                                         progress );
        carto::ThreadedLoop threadedLoop( &context, 
                                          0, 
                                          nSlices * info.m_frames );

        threadedLoop.launch();

        if ( info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info.m_minimum = int32_t( min );
          info.m_maximum = int32_t( max );

        }

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


bool soma::EnhancedMRImageStorageReader::buildIndexLut( DcmDataset* dataset )
{

  if ( dataset )
  {

    int32_t zIndex = -1, tIndex = 0, dim = 0;
    int32_t frameCount = m_dataInfo->m_slices;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      dim = seq->card();

      // Generic version only support up to 4D data structure.
      // Any other data organisation have to be described in a derived class.
      if ( dim > 3 )
      {

        return false;

      }

      int32_t i;
      DcmTagKey stackID( DCM_StackID );
      DcmTagKey inStackPos( DCM_InStackPositionNumber );

      for ( i = 0; i < dim; i++ )
      {

        OFString tmpString;
        DcmItem* item = seq->getItem( i );

         if ( item->findAndGetOFString( DCM_DimensionIndexPointer, 
                                        tmpString ).good() )
         {

           if ( !tmpString.compare( inStackPos.toString() ) )
           {

             zIndex = i;

           }
           else if ( tmpString.compare( stackID.toString() ) )
           {

             tIndex = i;

           }

         }

      }

    }

    if ( zIndex < 0 )
    {

      return false;

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

          Uint32 z, t = 1;

          tmpItem->findAndGetUint32( DCM_DimensionIndexValues, z, zIndex );

          if ( dim == 3 )
          {

            tmpItem->findAndGetUint32( DCM_DimensionIndexValues, t, tIndex );

          }

          if  ( int32_t( z ) > zMax )
          {

            zMax = z;

          }

          tmpLut[ i ] = std::make_pair( z - 1, t - 1 );

        }

      }

      for ( i = 0; i < nItems; i++ )
      {

        m_indexLut[ i ] = tmpLut[ i ].second * zMax + tmpLut[ i ].first;

      }

      if ( zMax < frameCount )
      {

        m_dataInfo->m_frames = frameCount / zMax;
        m_dataInfo->m_slices = zMax;

      }

    }
    else
    {

      int32_t i;

      for ( i = 0; i < frameCount; i++ )
      {

        m_indexLut[ i ] = i;

      }

    }

  }

  return true;

}

