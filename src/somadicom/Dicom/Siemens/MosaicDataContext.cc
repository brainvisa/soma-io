#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/Siemens/MosaicDicomImage.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/Siemens/MosaicDataContext.h>
#include <Dicom/Siemens/MosaicDicomImage.h>
#include <Dicom/Siemens/Demosaicer.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#endif


soma::MosaicDataContext::MosaicDataContext( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy,
                                        soma::Demosaicer& demosaicer )
                       : soma::LoopContext(),
                         _fileList( datasetHeader.getFileList() ),
                         _lut( datasetHeader.getLut() ),
                         _proxy( proxy ),
                         _demosaicer( demosaicer )
{

  _parameters = soma::ImagePixel::Parameters( proxy );

}


void soma::MosaicDataContext::doIt( int32_t startIndex, int32_t count )
{

  soma::MosaicDicomImage dicomImage( _proxy, 
                                     _parameters,
                                     _demosaicer.getMosaicSizeX() );
  soma::DataInfo& info = _proxy.getDataInfo();
  int32_t i, stopIndex = startIndex + count;
  int32_t min = 0;
  int32_t max = 0;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    if ( dicomImage.load( _fileList[ _lut[ i ] ] ) )
    {

      int32_t dMin = 0, dMax = 0;

      _demosaicer.demosaic( dicomImage, info, 0, i );
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
