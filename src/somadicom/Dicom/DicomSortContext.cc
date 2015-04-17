#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomSortContext.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/PositionModule.h>
#else
#include <Dicom/DicomSortContext.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/PositionModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::DicomSortContext::DicomSortContext( 
                        dcm::DicomDatasetHeader& datasetHeader,
                        std::multimap< double, dcm::FileInformation >& slices,
                        const dcm::Vector3d< double >& rowVector,
                        const dcm::Vector3d< double >& columnVector )
#ifdef SOMA_IO_DICOM
                     : carto::LoopContext(),
#else
                     : dcm::LoopContext(),
#endif
                       _datasetHeader( datasetHeader ),
                       _slices( slices )
{

  _normal = rowVector.cross( columnVector );

}


void dcm::DicomSortContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  std::multimap< double, dcm::FileInformation > slices;
  dcm::PositionModule positionModule;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    dcm::FileInformation fileInfo;
    DcmDataset dataset;
    OFString tmpString;
    Sint32 tmpInt;
    double index = 0.0;

    fileInfo._bufferIndex = i;
    fileInfo._instanceNumber = int32_t( slices.size() );

    _datasetHeader.get( dataset, i );

    if ( dataset.findAndGetSint32( DCM_InstanceNumber, tmpInt ).good() )
    {

      fileInfo._instanceNumber = int32_t( tmpInt );

    }
    
    if ( positionModule.parseItem( &dataset ) )
    {

      fileInfo._imagePosition = positionModule.getImagePosition();
      index = _normal.dot( fileInfo._imagePosition );

    }

    slices.insert( std::make_pair( index, fileInfo ) );

  }

  lock();
  _slices.insert( slices.begin(), slices.end() );
  unlock();

}
