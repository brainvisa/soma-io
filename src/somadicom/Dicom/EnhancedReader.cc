#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/EnhancedModalityLutModule.h>
#include <soma-io/Dicom/EnhancedOrientationModule.h>
#include <soma-io/Dicom/EnhancedPixelMeasuresModule.h>
#include <soma-io/Dicom/PositionModule.h>
#include <soma-io/Dicom/EnhancedIndexModule.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/EnhancedReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/EnhancedModalityLutModule.h>
#include <Dicom/EnhancedOrientationModule.h>
#include <Dicom/EnhancedPixelMeasuresModule.h>
#include <Dicom/PositionModule.h>
#include <Dicom/EnhancedIndexModule.h>
#include <Container/DataInfo.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcsequen.h>


dcm::EnhancedReader::EnhancedReader()
                   : dcm::SingleFileReader(),
                     dcm::MultiSliceReader()
{
}


bool dcm::EnhancedReader::getHeader( dcm::HeaderProxy& proxy,
                                     dcm::DataInfo& info,
                                     dcm::DicomDatasetHeader& datasetHeader )
{

  if ( dcm::SingleFileReader::getHeader( proxy, info, datasetHeader ) )
  {

    if ( !info._modalityLut && !proxy.hasAttribute( "rescale_intercept" ) )
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

    return dcm::MultiSliceReader::getHeader( proxy, info, datasetHeader );

  }

  return false;

}


bool dcm::EnhancedReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    if ( !buildIndexLut( dataset ) )
    {

      return false;

    }

    dcm::EnhancedOrientationModule orientationModule;
    dcm::EnhancedPixelMeasuresModule pixelModule;

    if ( orientationModule.parseDataset( dataset ) )
    {

      _dataInfo->_rowVec = orientationModule.getRowCosine();
      _dataInfo->_colVec = orientationModule.getColumnCosine();

    }

    if ( pixelModule.parseDataset( dataset ) )
    {

      double tmpDouble = pixelModule.getSliceThickness();

      _dataInfo->_spacingBetweenSlices = tmpDouble;
      _dataInfo->_resolution.x = pixelModule.getPixelSpacingX();
      _dataInfo->_resolution.y = pixelModule.getPixelSpacingY();
      _dataInfo->_resolution.z = tmpDouble;

    }

    if ( _dataInfo->_modalityLut )
    {

      dcm::EnhancedModalityLutModule modalityLutModule;

      if ( modalityLutModule.parseDataset( dataset ) )
      {

        _slope = modalityLutModule.getRescaleSlope();
        _intercept = modalityLutModule.getRescaleIntercept();

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

    dcm::MultiSliceReader::setOrientation( *_dataInfo );

    return true;

  }

  return false;

}


bool dcm::EnhancedReader::buildIndexLut( DcmDataset* dataset )
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
