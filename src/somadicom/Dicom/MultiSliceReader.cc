#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/MultiSliceDicomImage.h>
#include <soma-io/Dicom/MultiSliceContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#else
#include <Dicom/MultiSliceReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/MultiSliceDicomImage.h>
#include <Dicom/MultiSliceContext.h>
#include <Thread/ThreadedLoop.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <cmath>


dcm::MultiSliceReader::MultiSliceReader()
                     : dcm::DicomReader()
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

  return dcm::DicomReader::getHeader( header, dataInfo, datasetHeader );

}


void dcm::MultiSliceReader::setOrientation()
{

  dcm::Vector3d< double > origin( 0.0, 0.0, 0.0 );

  _dataInfo->_resolution.z = _dataInfo->_spacingBetweenSlices;

  if ( !_positions.empty() )
  {

    origin = _positions[ 0 ];

    if ( !_dataInfo->_mosaic || _dataInfo->_noDemosaic )
    {

      int32_t lastZ = _dataInfo->_slices - 1;

      if (lastZ > 0 && _dataInfo->_fileCount > 1)
      {

        dcm::Vector3d< double > normalVec = _dataInfo->_rowVec.cross( 
                                                           _dataInfo->_colVec );
        double p = std::fabs( normalVec.dot( _positions[ lastZ ] - origin ) );

        _dataInfo->_resolution.z = p / double( lastZ );
      }

    }

  }

  _dataInfo->_origin = origin;

  dcm::DicomReader::setOrientation();

}


bool dcm::MultiSliceReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    return buildIndexLut( dataset );

  }

  return false;

}


bool dcm::MultiSliceReader::readData( dcm::DicomDatasetHeader& datasetHeader,
                                      dcm::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    dcm::ImagePixel::Parameters parameters( proxy );
    dcm::MultiSliceDicomImage dicomImage( proxy, parameters );

    if ( dicomImage.load( fileName ) )
    {

      dcm::DataInfo& info = proxy.getDataInfo();
      int32_t i, k = 0;
      int32_t startZ = info._boundingBox.getLowerZ();
      int32_t endZ = info._boundingBox.getUpperZ() + 1;
      int32_t startT = info._boundingBox.getLowerT();
      int32_t endT = info._boundingBox.getUpperT() + 1;
      std::vector< int32_t > selection( ( endZ - startZ ) * ( endT - startT ) );
      int32_t n = int32_t( _indexLut.size() );
      dcm::Vector3d< int32_t > sizes = info._patientOrientation.getOnDiskSize();

      for ( i = 0; i < n; i++ )
      {

        int32_t index = _indexLut[ i ];
        int32_t z = index % sizes.z;
        int32_t t = index / sizes.z;

        if ( ( z >= startZ ) && ( z < endZ ) && 
             ( t >= startT ) && ( t < endT ) )
        {

          selection[ k++ ] = i;

        }

      }

      dcm::MultiSliceContext context( dicomImage, _indexLut, selection );
#ifdef SOMA_IO_DICOM
      carto::ThreadedLoop threadedLoop( &context, 
                                        0, 
                                        int32_t( selection.size() ) );
#else
      dcm::ThreadedLoop threadedLoop( &context, 
                                      0, 
                                      int32_t( selection.size() ) );
#endif

      threadedLoop.launch();

      if ( info._bpp < 3 )
      {

        int32_t min = 0, max = 0;

        dicomImage.getMinMaxValues( min, max );

        if ( min != max )
        {

          info._minimum = int32_t( min );
          info._maximum = int32_t( max );

        }
        else
        {

          info._minimum = 0;
          info._maximum = ( 1 << info._bitsStored ) - 1;

        }

      }

      return true;

    }

  }

  return false;

}


bool dcm::MultiSliceReader::buildIndexLut( DcmDataset* /* dataset */ )
{

  int32_t i, n = _dataInfo->_slices * _dataInfo->_frames;

  _indexLut.resize( n );

  for ( i = 0; i < n; i++ )
  {

    _indexLut[ i ] = i;

  }

  return true;

}
