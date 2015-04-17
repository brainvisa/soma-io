#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedMRImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/EnhancedModalityLutModule.h>
#include <soma-io/Dicom/EnhancedOrientationModule.h>
#include <soma-io/Dicom/PositionModule.h>
#include <soma-io/Dicom/EnhancedIndexModule.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedMRImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/EnhancedModalityLutModule.h>
#include <Dicom/EnhancedOrientationModule.h>
#include <Dicom/PositionModule.h>
#include <Dicom/EnhancedIndexModule.h>
#include <Container/DataInfo.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcsequen.h>


dcm::EnhancedMRImageStorageReader::EnhancedMRImageStorageReader()
                                 : dcm::MultiSliceReader()
{
}


std::string dcm::EnhancedMRImageStorageReader::getStorageUID()
{

  return UID_EnhancedMRImageStorage;

}


bool dcm::EnhancedMRImageStorageReader::getHeader(
                                        dcm::HeaderProxy& proxy,
                                        dcm::DataInfo& info,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::MultiSliceReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "rescale_intercept" ) )
  {

    dcm::EnhancedModalityLutModule modalityLutModule;

    if ( modalityLutModule.parseHeader( datasetHeader ) )
    {

      proxy.addAttribute( "rescale_intercept",  
                          modalityLutModule.getRescaleIntercept());
      proxy.addAttribute( "rescale_slope", 
                          modalityLutModule.getRescaleSlope() );

    }

  }

  return true;

}


bool dcm::EnhancedMRImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    if ( !buildIndexLut( dataset ) )
    {

      return false;

    }

    dcm::EnhancedOrientationModule orientationModule;

    if ( orientationModule.parseDataset( dataset ) )
    {

      _dataInfo->_rowVec = orientationModule.getRowCosine();
      _dataInfo->_colVec = orientationModule.getColumnCosine();

    }

    DcmItem* seqItem = 0;

    if ( dataset->findAndGetSequenceItem( DCM_SharedFunctionalGroupsSequence,
                                          seqItem ).good() )
    {

      DcmItem* item = 0;

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

    }

    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence, 
                                      seq ).good() )
    {

      dcm::PositionModule positionModule;
      uint32_t i, nItems = seq->card();

      _positions.resize( _dataInfo->_slices * _dataInfo->_frames );

      for ( i = 0; i < nItems; i++ )
      {

        int32_t index = _indexLut[ i ];
        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_PlanePositionSequence,
                                           tmpItem ).good() )
        {

          if ( positionModule.parseItem( tmpItem ) )
          {

            _positions[ index ] = positionModule.getImagePosition();

          }

        }

      }

    }

    return true;

  }

  return false;

}


bool dcm::EnhancedMRImageStorageReader::buildIndexLut( DcmDataset* dataset )
{

  if ( dataset )
  {

    dcm::EnhancedIndexModule indexModule;

    if ( indexModule.parseItem( dataset ) )
    {

      _dataInfo->_slices = indexModule.getZCount();
      _dataInfo->_frames = indexModule.getTCount();
      _indexLut = indexModule.getIndices();

      return true;

    }

  }

  return false;

}
