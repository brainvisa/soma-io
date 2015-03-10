#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomSortContext.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/DicomSortContext.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <sstream>


soma::DicomSortContext::DicomSortContext( 
                         soma::DicomDatasetHeader& datasetHeader,
                         std::multimap< double, soma::FileInformation >& slices,
                         const soma::Vector& rowVector,
                         const soma::Vector& columnVector )
                      : _datasetHeader( datasetHeader ),
                        _slices( slices )
{

  _normal = rowVector.cross( columnVector );

}


void soma::DicomSortContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  std::multimap< double, soma::FileInformation > slices;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    soma::FileInformation fileInfo;
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
    
    if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient, 
                                          tmpString ).good() )
    {

      std::string strPosition = tmpString.c_str();
      size_t pos = strPosition.find( "\\" );

      while ( pos != std::string::npos )
      {

        strPosition[ pos ] = ' ';
        pos = strPosition.find( "\\" );

      }

      std::istringstream iss( strPosition );

      iss >> fileInfo._imagePosition.x
          >> fileInfo._imagePosition.y
          >> fileInfo._imagePosition.z;

      index = _normal.dot( fileInfo._imagePosition );

    }

    slices.insert( std::make_pair( index, fileInfo ) );

  }

  lock();
  _slices.insert( slices.begin(), slices.end() );
  unlock();

}
