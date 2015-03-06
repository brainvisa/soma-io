#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Dicom/EnhancedIndexModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedMRImageStorageReader.h>
#include <Dicom/EnhancedIndexModule.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
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

    if ( !buildIndexLut( dataset ) )
    {

      return false;

    }

    bool found = false;
    bool rescale = false;
    DcmItem* seqItem = 0;

    if ( dataset->findAndGetSequenceItem( DCM_SharedFunctionalGroupsSequence,
                                          seqItem ).good() )
    {

      DcmItem* item = 0;

      if ( seqItem->findAndGetSequenceItem( DCM_PlaneOrientationSequence,
                                            item ).good() )
      {

        OFString tmpString;

        if ( item->findAndGetOFStringArray( DCM_ImageOrientationPatient,
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

          iss >> _dataInfo->_rowVec.x
              >> _dataInfo->_rowVec.y
              >> _dataInfo->_rowVec.z
              >> _dataInfo->_colVec.x
              >> _dataInfo->_colVec.y
              >> _dataInfo->_colVec.z;

          found = true;

        }

      }

      if ( seqItem->findAndGetSequenceItem( DCM_PixelMeasuresSequence,
                                            item ).good() )
      {

        Float64 tmpDouble;

        if ( item->findAndGetFloat64( DCM_SliceThickness, tmpDouble ).good() )
        {

          _dataInfo->_spacingBetweenSlices = double( tmpDouble );
          _dataInfo->_resolution.z = double( tmpDouble );

        }

        if ( item->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 0 ).good() )
        {

          _dataInfo->_resolution.x = double( tmpDouble );

        }

        if ( item->findAndGetFloat64( DCM_PixelSpacing, tmpDouble, 1 ).good() )
        {

          _dataInfo->_resolution.y = double( tmpDouble );

        }

      }

      if ( seqItem->findAndGetSequenceItem( 
                                           DCM_PixelValueTransformationSequence,
                                           item ).good() )
      {

        Float64 tmpDouble;

        rescale = true;
        _dataInfo->_slope.resize( 1, 1.0 );
        _dataInfo->_intercept.resize( 1, 0.0 );


        if ( item->findAndGetFloat64( DCM_RescaleSlope, tmpDouble ).good() )
        {

          _dataInfo->_slope[ 0 ] = double( tmpDouble );

        }

        if ( item->findAndGetFloat64( DCM_RescaleIntercept, tmpDouble ).good() )
        {

          _dataInfo->_intercept[ 0 ] = double( tmpDouble );

        }

      }

    }

    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();

      _positions.resize( nItems );

      if ( !rescale )
      {

        _dataInfo->_slope.resize( nItems, 1.0 );
        _dataInfo->_intercept.resize( nItems, 0.0 );

      }

      for ( i = 0; i < nItems; i++ )
      {

        int32_t index = _indexLut[ i ];
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

            iss >> _dataInfo->_rowVec.x
                >> _dataInfo->_rowVec.y
                >> _dataInfo->_rowVec.z
                >> _dataInfo->_colVec.x
                >> _dataInfo->_colVec.y
                >> _dataInfo->_colVec.z;

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

            _positions[ index ] = position;

          }

        }

        if ( !rescale &&
             item->findAndGetSequenceItem( DCM_PixelValueTransformationSequence,
                                           tmpItem ).good() )
        {

          Float64 tmpFloat;

          if ( tmpItem->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
          {

            _dataInfo->_slope[ index ] = (double)tmpFloat;

          }

          if ( tmpItem->findAndGetFloat64( DCM_RescaleIntercept,
                                           tmpFloat ).good() )
          {

            _dataInfo->_intercept[ index ] = (double)tmpFloat;

          }

        }

      }

    }

    return true;

  }

  return false;

}


bool soma::EnhancedMRImageStorageReader::buildIndexLut( DcmDataset* dataset )
{

  if ( dataset )
  {

    soma::EnhancedIndexModule indexModule;

    if ( indexModule.parseDataset( dataset ) )
    {

      int32_t zMax = indexModule.getZCount();
      int32_t frameCount = indexModule.getNumberOfFrames();

      if ( zMax < frameCount )
      {

        _dataInfo->_frames = frameCount / zMax;
        _dataInfo->_slices = zMax;

      }
      else
      {

        _dataInfo->_slices = frameCount;

      }

      _indexLut = indexModule.getIndices();

      return true;

    }

  }

  return false;

}
