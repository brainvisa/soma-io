#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Object/HeaderProxy.h>
#else
#include <Dicom/MultiSliceReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Object/HeaderProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <limits>


soma::MultiSliceReader::MultiSliceReader()
                      : soma::DicomReader()
{
}


bool soma::MultiSliceReader::getHeader( 
                                       soma::HeaderProxy& header, 
                                       soma::DataInfo& dataInfo,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  OFString tmpString;
  DcmDataset dataset;
  std::vector< std::string > referential;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFString( DCM_FrameOfReferenceUID, tmpString ).good() )
  {

    referential.push_back( tmpString.c_str() );
    header.addAttribute( "referential", referential );

  }

  std::vector< std::string > referentials;
  referentials.push_back(
                        std::string( "Scanner-based anatomical coordinates" ) );
  header.addAttribute( "referentials", referentials );

  std::vector< double > transformation = 
         dataInfo._patientOrientation.getReferential().getDirectCoefficients();
  transformation[ 3 ] *= -1.0;
  transformation[ 7 ] *= -1.0;
  transformation[ 11 ] *= -1.0;
  std::vector< std::vector< double > > transformations;
  transformations.push_back( transformation );
  header.addAttribute( "transformations", transformations );

  std::vector< double > axialTransformation = 
    dataInfo._patientOrientation.
                               getAxialTransformation().getDirectCoefficients();
  header.addAttribute( "storage_to_memory", axialTransformation );

  return soma::DicomReader::getHeader( header, dataInfo, datasetHeader );

}


void soma::MultiSliceReader::setOrientation()
{

  soma::Vector origin( 0.0, 0.0, 0.0 );
  soma::Vector normalVec = _dataInfo->_rowVec.cross( _dataInfo->_colVec );

  _dataInfo->_resolution.z = _dataInfo->_spacingBetweenSlices;

  if ( !_positions.empty() )
  {

    if ( _positions.size() > 1 )
    {

      soma::Vector normVec = _positions[ 1 ] - _positions[ 0 ];

      if ( normVec.getNorm() > 0.0 )
      {

        normVec.normalize();
        normalVec = normVec;

      }

    }

    double minPos = std::numeric_limits< double >::max();
    double maxPos = -std::numeric_limits< double >::max();
    double n = (double)_dataInfo->_slices - 1.0;
    std::vector< soma::Vector >::iterator 
      p = _positions.begin(),
      pe = _positions.end();

    origin = *p;

    while ( p != pe )
    {

      double dotProduct = normalVec.dot( *p );

      if ( dotProduct < minPos )
      {

        minPos = dotProduct;
        origin = *p;

      }
      else if ( dotProduct > maxPos )
      {

        maxPos = dotProduct;

      }

      ++p;

    }

    if ( ( n >= 1.0 ) && ( maxPos > minPos ) )
    {

      _dataInfo->_resolution.z = ( maxPos - minPos ) / n;

    }

  }

  _dataInfo->_normVec = normalVec;
  _dataInfo->_origin = origin;

  soma::DicomReader::setOrientation();

}

