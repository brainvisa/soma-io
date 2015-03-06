#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiFrameContext.h>
//#include <soma-io/Dicom/DicomImage.h>
#include <soma-io/Dicom/MultiFrameDicomImage.h>
#else
#include <Dicom/MultiFrameContext.h>
//#include <Dicom/MultiFrameDicomImage.h>
#include <Dicom/DicomImage.h>
#endif


//soma::MultiFrameContext::MultiFrameContext( MultiFrameDicomImage& dicomImage )
soma::MultiFrameContext::MultiFrameContext( DicomImage& dicomImage )
                       : soma::LoopContext(),
                         _dicomImage( dicomImage )
{
}


void soma::MultiFrameContext::doIt( int32_t startIndex, int32_t count )
{

  //_dicomImage.fillSlab( startIndex, count );
  int32_t i, stopIndex = startIndex + count;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    _dicomImage.fill( 0, i, i );

  }

}
