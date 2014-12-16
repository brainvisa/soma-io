#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Pattern/Callback.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>


soma::MosaicDataContext::MosaicDataContext( 
                                   const std::vector< std::string >& files,
                                   soma::DicomProxy& proxy,
                                   soma::Demosaicer& demosaicer,
                                   soma::Callback* progress )
                       : carto::LoopContext(),
                         m_files( files ),
                         m_proxy( proxy ),
                         m_demosaicer( demosaicer ),
                         m_progress( progress ),
                         m_count( 0 )
{
}


void soma::MosaicDataContext::doIt( int32_t startIndex, int32_t count )
{

  soma::DataInfo& info = m_proxy.getDataInfo();
  int32_t i, stopIndex = startIndex + count;
  int32_t n = info.m_frames - 1;
  int32_t min = ( 1 << info.m_depth ) - 1;
  int32_t max = 1 - ( 1 << info.m_depth );

  for ( i = startIndex; i < stopIndex; i++ )
  {

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_files[ i ].c_str() ).good() )
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

        m_demosaicer.demosaic( dcmImage, m_proxy, 0, i );
                             
        lock();
        m_count++;

        if ( m_progress )
        {

          m_progress->execute( 6 + 94 * m_count / n );

        }

        unlock();

      }

    }

  }

  lock();

  if ( min < info.m_minimum )
  {

    info.m_minimum = min;

  }

  if ( max > info.m_maximum )
  {

    info.m_maximum = max;

  }

  unlock();

}
