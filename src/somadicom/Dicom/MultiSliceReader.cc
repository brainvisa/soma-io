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

#include <cmath>


dcm::MultiSliceReader::MultiSliceReader()
{
}


dcm::MultiSliceReader::~MultiSliceReader()
{
}


bool dcm::MultiSliceReader::getHeader( dcm::HeaderProxy& header, 
                                       dcm::DataInfo& dataInfo,
                                       dcm::DicomDatasetHeader& datasetHeader )
{

  OFString tmpString;
  DcmDataset dataset;
  std::vector< std::string > referential;
  std::vector< double > rowVector( 3 );
  std::vector< double > columnVector( 3 );
  std::vector< double > origin( 3 );

  rowVector[ 0 ] = dataInfo._rowVec.x;
  rowVector[ 1 ] = dataInfo._rowVec.y;
  rowVector[ 2 ] = dataInfo._rowVec.z;
  columnVector[ 0 ] = dataInfo._colVec.x;
  columnVector[ 1 ] = dataInfo._colVec.y;
  columnVector[ 2 ] = dataInfo._colVec.z;
  origin[ 0 ] = dataInfo._origin.x;
  origin[ 1 ] = dataInfo._origin.y;
  origin[ 2 ] = dataInfo._origin.z;

  header.addAttribute( "row_cosine", rowVector );
  header.addAttribute( "column_cosine", columnVector );
  header.addAttribute( "origin", origin );

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
  std::vector< std::vector< double > > transformations;
  transformations.push_back( transformation );
  header.addAttribute( "transformations", transformations );

  std::vector< double > axialTransformation = 
    dataInfo._patientOrientation.
                               getAxialTransformation().getDirectCoefficients();
  header.addAttribute( "storage_to_memory", axialTransformation );

  return true;

}


void dcm::MultiSliceReader::setOrientation( dcm::DataInfo& dataInfo )
{

  dcm::Vector3d< double > origin( 0.0, 0.0, 0.0 );

  dataInfo._resolution.z = dataInfo._spacingBetweenSlices;

  if ( !_positions.empty() )
  {

    origin = _positions[ 0 ];

    if ( !dataInfo._mosaic || dataInfo._noDemosaic )
    {

      int32_t lastZ = dataInfo._slices - 1;

      if ( ( lastZ > 0 ) && ( dataInfo._fileCount > 1 ) )
      {

        dcm::Vector3d< double > normalVec = dataInfo._rowVec.cross( 
                                                             dataInfo._colVec );
        double p = std::fabs( normalVec.dot( _positions[ lastZ ] - origin ) );

        dataInfo._resolution.z = p / double( lastZ );

      }

    }

  }

  dataInfo._origin = origin;

}
