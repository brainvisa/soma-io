#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceContext.h>
#include <soma-io/Dicom/DicomImage.h>
#else
#include <Dicom/MultiSliceContext.h>
#include <Dicom/DicomImage.h>
#endif


dcm::MultiSliceContext::MultiSliceContext( 
                                        dcm::DicomImage& dicomImage, 
                                        const std::vector< int32_t >& indexLut,
                                        const std::vector< int32_t >& selection,
                                        const std::vector< double >& slope,
                                        const std::vector< double >& intercept )
#ifdef SOMA_IO_DICOM
                      : carto::LoopContext(),
#else
                      : dcm::LoopContext(),
#endif
                        _dicomImage( dicomImage ),
                        _indexLut( indexLut ),
                        _selection( selection ),
                        _slope( slope ),
                        _intercept( intercept )
{
}


void dcm::MultiSliceContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  int32_t slopeSize = int32_t( _slope.size() );
  int32_t interceptSize = int32_t( _intercept.size() );

  for ( i = startIndex; i < stopIndex; i++ )
  {

    int32_t selectedIndex = _selection[ i ];
    int32_t index = _indexLut[ selectedIndex ];

    if ( ( index < slopeSize ) && ( index < interceptSize ) )
    {

      _dicomImage.setSlopeAndIntercept( _slope[ index ], _intercept[ index ] );

    }

    _dicomImage.fillIndex( index, selectedIndex );

  }

}
