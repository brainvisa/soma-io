#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomDataContext.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/DicomDataContext.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomImage.h>
#include <Container/DicomProxy.h>
#endif


dcm::DicomDataContext::DicomDataContext( 
                                       dcm::DicomDatasetHeader& datasetHeader,
                                       dcm::DicomProxy& proxy,
                                       const std::vector< int32_t >& selection )
#ifdef SOMA_IO_DICOM
                     : carto::LoopContext(),
#else
                     : dcm::LoopContext(),
#endif
                       _proxy( proxy ),
                       _selection( selection ),
                       _fileList( datasetHeader.getFileList() ),
                       _lut( datasetHeader.getLut() )
{

  _parameters = dcm::ImagePixel::Parameters( proxy );

}


void dcm::DicomDataContext::doIt( int32_t startIndex, int32_t count )
{

  dcm::DicomImage dicomImage( _proxy, _parameters );
  dcm::DataInfo& info = _proxy.getDataInfo();
  int32_t i, stopIndex = startIndex + count;
  float min = 100000.0f;
  float max = 0.0f;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    int32_t index = _selection[ i ];

    if ( dicomImage.load( _fileList[ _lut[ index ] ] ) )
    {

      float dMin = 0.0f, dMax = 0.0f;

      dicomImage.fillIndex( index );
      dicomImage.getMinMaxValues( dMin, dMax );

      if ( dMin != dMax )
      {

        if ( dMin < min )
        {

          min = dMin;

        }

        if ( dMax > max )
        {

          max = dMax;

        }

      }

    }

  }

  if ( min != max )
  {

    lock();

    if ( min < info._minimum )
    {

      info._minimum = min;

    }

    if ( max > info._maximum )
    {

      info._maximum = max;

    }

    unlock();

  }

}
