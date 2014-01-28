#include <soma-io/Dicom/NMImageStorageReader.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <Thread/ThreadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <algorithm>


soma::NMImageStorageReader::NMImageStorageReader()
                          : soma::DicomReader()
{
}


std::string soma::NMImageStorageReader::getStorageUID()
{

  return UID_NuclearMedicineImageStorage;

}


bool soma::NMImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    Uint16 nSlices = 1;

    if ( dataset->findAndGetUint16( DCM_NumberOfSlices, nSlices ).good() )
    {

      m_dataInfo.m_slices = (int32_t)nSlices;

    }

    return true;

  }

  return false;

}


bool soma::NMImageStorageReader::readData( soma::Data& data,
                                           soma::Callback* progress )
{

  if ( data.Create( m_dataInfo ) )
  {

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_slices[ 0 ].c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, 
                           dataset->getOriginalXfer(),
                           CIF_IgnoreModalityTransformation );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        Float64 tmpFloat;
 
        data.m_info.m_modalityLUTApplied = false;

        if ( dataset->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
        {

          std::fill( data.m_info.m_slope.begin(),
                     data.m_info.m_slope.end(),
                     (double)tmpFloat );

        }

        if ( dataset->findAndGetFloat64( DCM_RescaleIntercept, 
                                         tmpFloat ).good() )
        {

          std::fill( data.m_info.m_intercept.begin(),
                     data.m_info.m_intercept.end(),
                     (double)tmpFloat );

        }

        soma::MultiFrameContext context( dcmImage, data, true, progress );
        soma::ThreadedLoop threadedLoop( &context,
                                         0,
                                         data.m_info.m_slices );

        threadedLoop.launch();

        data.m_info.m_pixelRepresentation = 
                               dcmImage.getInterData()->getRepresentation() & 1;

        if ( data.m_info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          data.m_info.m_minimum = int32_t( min );
          data.m_info.m_maximum = int32_t( max );

        }

        return true;

      }

    }

  }

  return false;

}
