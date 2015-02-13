#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/EnhancedIndexModule.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Dicom/MultiSliceContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedMRImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/EnhancedIndexModule.h>
#include <Container/DicomProxy.h>
#include <Dicom/MultiSliceContext.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
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

      _positions.resize( nItems );
      _dataInfo->_slope.resize( nItems, 1.0 );
      _dataInfo->_intercept.resize( nItems, 0.0 );

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

        if ( item->findAndGetSequenceItem( DCM_PixelValueTransformationSequence,
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


bool soma::EnhancedMRImageStorageReader::readData(
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( fileName.c_str() ).good() )
    {

      soma::DataInfo& info = proxy.getDataInfo();
      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, 
                           dataset->getOriginalXfer(),
                           CIF_IgnoreModalityTransformation );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        int32_t i, k = 0;
        int32_t sizeX, sizeY, sizeZ;
        int32_t startZ = info._boundingBox.getLowerZ();
        int32_t endZ = info._boundingBox.getUpperZ() + 1;
        int32_t startT = info._boundingBox.getLowerT();
        int32_t endT = info._boundingBox.getUpperT() + 1;
        std::vector< int32_t > selection( ( endZ - startZ ) * 
                                          ( endT - startT ) );

        info._patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

        int32_t n = sizeZ * info._frames;

        for ( i = 0; i < n; i++ )
        {

          int32_t index = _indexLut[ i ];
          int32_t z = index % sizeZ;
          int32_t t = index / sizeZ;

          if ( ( z >= startZ ) && ( z < endZ ) && 
               ( t >= startT ) && ( t < endT ) )
          {

            selection[ k++ ] = i;

          }

        }

        info._modalityLUTApplied = false;
        info._pixelRepresentation = 
                                   dcmImage.getInterData()->getRepresentation();

        soma::MultiSliceContext context( dcmImage, 
                                         proxy, 
                                         _indexLut, 
                                         selection );
        soma::ThreadedLoop threadedLoop( &context, 
                                         0, 
                                         int32_t( selection.size() ) );

        threadedLoop.launch();

        if ( info._bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info._minimum = int32_t( min );
          info._maximum = int32_t( max );

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

