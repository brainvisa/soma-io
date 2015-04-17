#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiFrameContext.h>
#include <soma-io/Dicom/MultiFrameDicomImage.h>
#else
#include <Dicom/MultiFrameContext.h>
#include <Dicom/MultiFrameDicomImage.h>
#endif


dcm::MultiFrameContext::MultiFrameContext( MultiFrameDicomImage& dicomImage )
#ifdef SOMA_IO_DICOM
                      : carto::LoopContext(),
#else
                      : dcm::LoopContext(),
#endif
                        _dicomImage( dicomImage )
{
}


void dcm::MultiFrameContext::doIt( int32_t startIndex, int32_t count )
{

  _dicomImage.fillSlab( startIndex, count );

}
