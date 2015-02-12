#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/Siemens/MosaicDataContext.h>
#include <Dicom/Siemens/Demosaicer.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>


soma::MosaicDataContext::MosaicDataContext( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy,
                                        soma::Demosaicer& demosaicer )
                       : soma::LoopContext(),
                         _datasetHeader( datasetHeader ),
                         _proxy( proxy ),
                         _demosaicer( demosaicer )
{
}


void soma::MosaicDataContext::doIt( int32_t startIndex, int32_t count )
{

  soma::DataInfo& info = _proxy.getDataInfo();
  int32_t i, stopIndex = startIndex + count;
  int32_t min = ( 1 << info._depth ) - 1;
  int32_t max = 1 - ( 1 << info._depth );

  std::vector< std::string >& fileList = _datasetHeader.getFileList();
  std::vector< int32_t >& lut = _datasetHeader.getLut();

  for ( i = startIndex; i < stopIndex; i++ )
  {

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( fileList[ lut[ i ] ].c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer() );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        double dMin = 0.0, dMax = 0.0;

        dcmImage.getMinMaxValues( dMin, dMax );

        if ( int32_t( dMin ) < min )
        {

          min = int32_t( dMin );

        }

        if ( int32_t( dMax ) > max )
        {

          max = int32_t( dMax );

        }

        _demosaicer.demosaic( dcmImage, _proxy, 0, i );

      }

    }

  }

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
