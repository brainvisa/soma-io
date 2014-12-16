#include <soma-io/Dicom/DicomDataContext.h>
#include <soma-io/Container/DicomProxy.h>
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
                                   soma::DicomProxy& proxy,
                                   bool applyModalityLUT,
                                   soma::Callback* progress )
                      : carto::LoopContext(),
                        m_files( files ),
                        m_proxy( proxy ),
                        m_progress( progress ),
                        m_count( 0 )
{

  m_proxy.getDataInfo().m_modalityLUTApplied = applyModalityLUT;

}


void soma::DicomDataContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  soma::DataInfo& info = m_proxy.getDataInfo();
  int32_t px, py, pz, sizeX, sizeY, sizeZ;
  int32_t bpp = info.m_bpp;
  int32_t min = ( 1 << info.m_depth ) - 1;
  int32_t max = 1 - ( 1 << info.m_depth );
  unsigned long flags = 0;

  info.m_patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

  int32_t n = sizeZ * info.m_frames - 1;

  if ( !info.m_modalityLUTApplied )
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

        int32_t x, y;
        int32_t z = i % sizeZ;
        int32_t t = i / sizeZ;
        double dMin = 0.0, dMax = 0.0;
        uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData();
        int32_t representation = dcmImage.getInterData()->getRepresentation();

        dcmImage.getMinMaxValues( dMin, dMax );

        info.m_pixelRepresentation = representation;

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

          if ( info.m_noFlip )
          {

            for ( y = 0; y < sizeY; y++ )
            {

              for ( x = 0; x < sizeX; x++ )
              {

                *( (int16_t*)m_proxy( x, y, z, t ) ) = (int16_t)*pIn++;

              }

            }

          }
          else
          {

            for ( y = 0; y < sizeY; y++ )
            {

              for ( x = 0; x < sizeX; x++ )
              {

                info.m_patientOrientation.getDirect( x, y, z, px, py, pz );

                *( (uint16_t*)m_proxy( px, py, pz, t ) ) = (uint16_t)*pIn++;

              }

            }

          }

        }
        else
        {

          if ( info.m_noFlip )
          {

            if ( m_proxy.isMemoryMapped() )
            {

              for ( y = 0; y < sizeY; y++ )
              {

                for ( x = 0; x < sizeX; x++, pIn += bpp )
                {

                  std::memcpy( m_proxy( x, y, z, t ), pIn, bpp );

                }

              }

            }
            else
            {

              std::memcpy( m_proxy( 0, 0, z, t ), 
                           pIn, 
                           info.m_sliceSize * bpp );

            }

          }
          else
          {

            for ( y = 0; y < sizeY; y++ )
            {

              for ( x = 0; x < sizeX; x++, pIn += bpp )
              {

                info.m_patientOrientation.getDirect( x, y, z, px, py, pz );
                std::memcpy( m_proxy( px, py, pz, t ), pIn, bpp );

              }

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
