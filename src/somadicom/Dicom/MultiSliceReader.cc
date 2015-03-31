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

  _dataInfo->_resolution.z = _dataInfo->_spacingBetweenSlices;

  if ( !_positions.empty() )
  {

    origin = _positions[ 0 ];

    if ( !_dataInfo->_mosaic )
    {

      int32_t lastZ = _dataInfo->_slices - 1;

      if ( lastZ > 0 )
      {

        soma::Vector normalVec = _dataInfo->_rowVec.cross( _dataInfo->_colVec );
        double p = std::fabs( normalVec.dot( _positions[ lastZ ] - origin ) );

        _dataInfo->_resolution.z = p / double( lastZ );

      }

    }

  }

  _dataInfo->_origin = origin;

  soma::DicomReader::setOrientation();

}


bool soma::MultiSliceReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    return buildIndexLut( dataset );

  }

  return false;

}


bool soma::MultiSliceReader::readData( soma::DicomDatasetHeader& datasetHeader,
                                       soma::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    soma::ImagePixel::Parameters parameters( proxy );
    soma::MultiSliceDicomImage dicomImage( proxy, parameters );

    if ( dicomImage.load( fileName ) )
    {

      soma::DataInfo& info = proxy.getDataInfo();
      int32_t i, k = 0;
      int32_t sizeX, sizeY, sizeZ;
      int32_t startZ = info._boundingBox.getLowerZ();
      int32_t endZ = info._boundingBox.getUpperZ() + 1;
      int32_t startT = info._boundingBox.getLowerT();
      int32_t endT = info._boundingBox.getUpperT() + 1;
      std::vector< int32_t > selection( ( endZ - startZ ) * ( endT - startT ) );

      info._patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

      //int32_t n = sizeZ * info._frames;
      int32_t n = int32_t( _indexLut.size() );

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

      soma::MultiSliceContext context( dicomImage, _indexLut, selection );
      soma::ThreadedLoop threadedLoop( &context, 
                                       0, 
                                       int32_t( selection.size() ) );

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


bool soma::MultiSliceReader::buildIndexLut( DcmDataset* /* dataset */ )
{

  int32_t i, n = _dataInfo->_slices * _dataInfo->_frames;

  _indexLut.resize( n );

  for ( i = 0; i < n; i++ )
  {

    _indexLut[ i ] = i;

  }

  return true;

}
