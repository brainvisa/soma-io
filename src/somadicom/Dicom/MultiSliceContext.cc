#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceContext.h>
#include <soma-io/Dicom/MultiSliceDicomImage.h>
#else
#include <Dicom/MultiSliceContext.h>
#include <Dicom/MultiSliceDicomImage.h>
#endif


soma::MultiSliceContext::MultiSliceContext( 
                                       soma::MultiSliceDicomImage& dicomImage, 
                                       const std::vector< int32_t >& indexLut,
                                       const std::vector< int32_t >& selection )
                       : soma::LoopContext(),
                         _dicomImage( dicomImage ),
                         _indexLut( indexLut ),
                         _selection( selection )
{
}


void soma::MultiSliceContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    int32_t selectedIndex = _selection[ i ];
    int32_t index = _indexLut[ selectedIndex ];

    _dicomImage.fillIndex( index, selectedIndex );

  }

}
