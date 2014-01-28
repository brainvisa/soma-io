#include <soma-io/Dicom/DicomDataContext.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>

#include <iostream>


soma::DicomDataContext::DicomDataContext( 
                                   const std::vector< std::string >& files,
                                   soma::Data& data,
                                   bool applyModalityLUT,
                                   soma::Callback* progress )
                      : soma::LoopContext(),
                        m_files( files ),
                        m_data( data ),
                        m_progress( progress ),
                        m_count( 0 )
{

  m_data.m_info.m_modalityLUTApplied = applyModalityLUT;

}


void soma::DicomDataContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  int32_t px, py, pz, sizeX, sizeY, sizeZ;
  int32_t bpp = m_data.m_info.m_bpp;
  int32_t min = ( 1 << m_data.m_info.m_depth ) - 1;
  int32_t max = 1 - ( 1 << m_data.m_info.m_depth );
  unsigned long flags = 0;

  m_data.m_info.m_patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

  int32_t n = sizeZ * m_data.m_info.m_frames - 1;

  if ( !m_data.m_info.m_modalityLUTApplied )
  {

    flags |= CIF_IgnoreModalityTransformation;

  }

  for ( i = startIndex; i < stopIndex; i++ )
  {

    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( m_files[ i ].c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer(), flags );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        Float64 tmpFloat;
        int32_t x, y;
        double dMin = 0.0, dMax = 0.0;
        uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData();
        int32_t representation = dcmImage.getInterData()->getRepresentation();

        dcmImage.getMinMaxValues( dMin, dMax );

        m_data.m_info.m_pixelRepresentation = representation & 1;

        if ( dataset->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
        {

          m_data.m_info.m_slope[ i ] = (double)tmpFloat;

        }

        if ( dataset->findAndGetFloat64( DCM_RescaleIntercept, 
                                         tmpFloat ).good() )
        {

          m_data.m_info.m_intercept[ i ] = (double)tmpFloat;

        }

        if ( int32_t( dMin ) < min )
        {

          min = int32_t( dMin );

        }

        if ( int32_t( dMax ) > max )
        {

          max = int32_t( dMax );

        }

        if ( ( bpp == 2 ) && ( representation < 2 ) )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              m_data.m_info.m_patientOrientation.getDirect( x, 
                                                            y, 
                                                            i % sizeZ, 
                                                            px, 
                                                            py, 
                                                            pz );

              uint16_t* pOut = (uint16_t*)m_data.getLine( py, 
                                                          pz, 
                                                          i / sizeZ ) + px;

              *pOut = (uint16_t)*pIn++;

            }

          }

        }
        else
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++, pIn += bpp )
            {

              m_data.m_info.m_patientOrientation.getDirect( x, 
                                                            y, 
                                                            i % sizeZ, 
                                                            px, 
                                                            py, 
                                                            pz );

              uint8_t* pOut = m_data.getLine( py, pz, i / sizeZ ) + px * bpp;

              std::memcpy( pOut, pIn, bpp );

            }

          }

        }

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

  if ( min < m_data.m_info.m_minimum )
  {

    m_data.m_info.m_minimum = min;

  }

  if ( max > m_data.m_info.m_maximum )
  {

    m_data.m_info.m_maximum = max;

  }

  unlock();

}
