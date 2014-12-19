#include <soma-io/Dicom/NMImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Dicom/MultiFrameContext.h>
#include <cartobase/thread/threadedLoop.h>
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


bool soma::NMImageStorageReader::getHeader(
                                       soma::HeaderProxy& header,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::DicomReader::getHeader( header, info, datasetHeader ) )
  {

    return false;

  }

  header.addAttribute( "number_of_frames", info.m_slices );

  OFString tmpString;
  DcmDataset dataset;

  datasetHeader.get( dataset );

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  if ( dataset.findAndGetOFString( DCM_PatientSex, tmpString ).good() )
#else
  if ( dataset.findAndGetOFString( DCM_PatientsSex, tmpString ).good() )
#endif
  {

    header.addAttribute( "patient_sex", tmpString.c_str() );

  }

#if OFFIS_DCMTK_VERSION_NUMBER >= 360
  if ( dataset.findAndGetOFString( DCM_PatientBirthDate, tmpString ).good() )
#else
  if ( dataset.findAndGetOFString( DCM_PatientsBirthDate, tmpString ).good() )
#endif
  {

    header.addAttribute( "patient_birthdate", tmpString.c_str() );

  }

  if ( dataset.findAndGetOFString( DCM_CorrectedImage, tmpString ).good() )
  {

    header.addAttribute( "corrected_image", tmpString.c_str() );

  }

  return true;

}


bool soma::NMImageStorageReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    m_dataInfo->m_fileCount = 1;

    Uint16 nSlices = 1;
    Float64 tmpFloat;

    if ( dataset->findAndGetUint16( DCM_NumberOfSlices, nSlices ).good() )
    {

      m_dataInfo->m_slices = (int32_t)nSlices;

    }

    m_dataInfo->m_slope.resize( m_dataInfo->m_slices, 1 );
    m_dataInfo->m_intercept.resize( m_dataInfo->m_slices, 0 );

    if ( dataset->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
    {

      std::fill( m_dataInfo->m_slope.begin(),
                 m_dataInfo->m_slope.end(),
                 (double)tmpFloat );

    }

    if ( dataset->findAndGetFloat64( DCM_RescaleIntercept, 
                                     tmpFloat ).good() )
    {

      std::fill( m_dataInfo->m_intercept.begin(),
                 m_dataInfo->m_intercept.end(),
                 (double)tmpFloat );

    }

    return true;

  }

  return false;

}


bool soma::NMImageStorageReader::readData( soma::DicomProxy& proxy,
                                           soma::Callback* progress )
{

  if ( proxy.allocate() )
  {

    if ( progress )
    {

      progress->execute( 2 );

    }

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_slices[ 0 ].c_str() ).good() )
    {

      if ( progress )
      {

        progress->execute( 6 );

      }

      soma::DataInfo& info = proxy.getDataInfo();
      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, 
                           dataset->getOriginalXfer(),
                           CIF_IgnoreModalityTransformation );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        info.m_modalityLUTApplied = false;
        info.m_pixelRepresentation = 
                                   dcmImage.getInterData()->getRepresentation();

        soma::MultiFrameContext context( dcmImage, proxy, true, progress );
        carto::ThreadedLoop threadedLoop( &context, 0, info.m_slices );

        threadedLoop.launch();

        if ( info.m_bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info.m_minimum = int32_t( min );
          info.m_maximum = int32_t( max );

        }

        if ( progress )
        {

          progress->execute( 100 );

        }

        return true;

      }

    }

  }

  return false;

}
