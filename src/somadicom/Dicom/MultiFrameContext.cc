#include <soma-io/Dicom/MultiFrameContext.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>


soma::MultiFrameContext::MultiFrameContext( DicomImage& dcmImage, 
                                            soma::Data& data,
                                            bool fillSlices,
                                            soma::Callback* progress )
                       : soma::LoopContext(),
                         m_dcmImage( dcmImage ),
                         m_data( data ),
                         m_fillSlices( fillSlices ),
                         m_progress( progress ),
                         m_count( 0 )
{
}


void soma::MultiFrameContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t bpp = m_data.m_info.m_bpp;
  uint32_t sliceSize = m_data.m_info.m_sliceSize;
  int32_t n;
  uint8_t* ptr = 0;

  if ( m_fillSlices )
  {

    n = m_data.m_info.m_slices - 1;
    ptr = m_data.getSlice( startIndex );

  }
  else
  {

    n = m_data.m_info.m_frames - 1;
    ptr = m_data.getFrame( startIndex );

  }

  if ( bpp == 3 )
  {

    int32_t offset = startIndex * sliceSize;
    uint32_t n = count * sliceSize;
    uint8_t** d = (uint8_t**)m_dcmImage.getInterData()->getData();
    uint8_t* r = d[ 0 ] + offset;
    uint8_t* g = d[ 1 ] + offset;
    uint8_t* b = d[ 2 ] + offset;
    uint8_t* f = ptr;

    while ( n-- )
    {

      *f++ = *r++;
      *f++ = *g++;
      *f++ = *b++;

    }

  }
  else if ( ( bpp == 2 ) &&
            ( m_dcmImage.getInterData()->getRepresentation() < 2 ) )
  {

    uint16_t* p = (uint16_t*)ptr;
    uint8_t* d = (uint8_t*)m_dcmImage.getInterData()->getData() +
                 startIndex * sliceSize;

    while ( sliceSize-- )
    {

      *p++ = (uint16_t)*d++;

    }

  }
  else
  {

    sliceSize *= bpp;

    uint8_t* d = (uint8_t*)m_dcmImage.getInterData()->getData() +
                 startIndex * sliceSize;

    std::memcpy( (void*)ptr, d, count * sliceSize );

  }

  lock();
  m_count += count;

  if ( m_progress )
  {

    m_progress->execute( 6 + 94 * m_count / n );

  }

  unlock();

}
