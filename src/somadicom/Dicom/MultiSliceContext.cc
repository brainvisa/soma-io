#include <soma-io/Dicom/MultiSliceContext.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>


soma::MultiSliceContext::MultiSliceContext( 
                                   DicomImage& dcmImage, 
                                   soma::Data& data,
                                   soma::Callback* progress )
                       : carto::LoopContext(),
                         m_dcmImage( dcmImage ),
                         m_data( data ),
                         m_progress( progress ),
                         m_count( 0 )
{
}


void soma::MultiSliceContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  int32_t px, py, pz, sizeX, sizeY, sizeZ;
  int32_t bpp = m_data.m_info.m_bpp;

  m_data.m_info.m_patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

  int32_t n = sizeZ - 1;
  uint32_t sliceSize = sizeX * sizeY;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    if ( bpp == 3 )
    {

      int32_t x, y;
      int32_t offset = i * sliceSize;
      uint8_t** d = (uint8_t**)m_dcmImage.getInterData()->getData();
      uint8_t* r = d[ 0 ] + offset;
      uint8_t* g = d[ 1 ] + offset;
      uint8_t* b = d[ 2 ] + offset;

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

          uint8_t* pOut = m_data.getLine( py, pz, i / sizeZ ) + px * bpp;

          *pOut++ = *r++;
          *pOut++ = *g++;
          *pOut = *b++;

        }

      }

    }
    else if ( ( bpp == 2 ) &&
              ( m_dcmImage.getInterData()->getRepresentation() < 2 ) )
    {

      int32_t x, y;
      uint8_t* pIn = (uint8_t*)m_dcmImage.getInterData()->getData() +
                     i * sliceSize;

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

      int32_t x, y;
      uint8_t* pIn = (uint8_t*)m_dcmImage.getInterData()->getData() +
                     i * sliceSize * bpp;

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
