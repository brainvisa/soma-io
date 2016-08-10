#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/SingleFileReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/MultiSliceDicomImage.h>
#include <soma-io/Dicom/MultiSliceContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/SingleFileReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/MultiSliceDicomImage.h>
#include <Dicom/MultiSliceContext.h>
#include <Thread/ThreadedLoop.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <cmath>


dcm::SingleFileReader::SingleFileReader()
                     : dcm::DicomReader()
{
}


bool dcm::SingleFileReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    return buildIndexLut( dataset );

  }

  return false;

}


bool dcm::SingleFileReader::readData( dcm::DicomDatasetHeader& datasetHeader,
                                      dcm::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    dcm::DicomImage* dicomImage = getDicomImage( proxy );

    if ( dicomImage && dicomImage->load( fileName ) )
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

      if ( n > 1 )
      {

        dcm::MultiSliceContext context( *dicomImage,
                                        _indexLut,
                                        selection,
                                        _slope,
                                        _intercept );
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

      }
      else
      {

        dicomImage->fill( 0, 0 );

      }

      if ( info._bpp != 3 )
      {

        dicomImage->getMinMaxValues( info._minimum, info._maximum );

      }

      delete dicomImage;

      return true;

    }

  }

  return false;

}


bool dcm::SingleFileReader::buildIndexLut( DcmDataset* /* dataset */ )
{

  int32_t i, n = _dataInfo->_slices * _dataInfo->_frames;

  _indexLut.resize( n );

  for ( i = 0; i < n; i++ )
  {

    _indexLut[ i ] = i;

  }

  return true;

}


dcm::DicomImage* dcm::SingleFileReader::getDicomImage( dcm::DicomProxy& proxy )
{

  dcm::ImagePixel::Parameters parameters( proxy );

  return new dcm::MultiSliceDicomImage( proxy, parameters );

}
