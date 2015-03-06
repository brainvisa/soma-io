#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/MosaicDicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/Siemens/MosaicDicomImage.h>
#include <Container/DicomProxy.h>
#endif


soma::MosaicDicomImage::MosaicDicomImage(
                                       soma::DicomProxy& proxy, 
                                       soma::ImagePixel::Parameters& parameters,
                                       int32_t mosaicSizeX )
                      : soma::DicomImage( proxy, parameters)
{

  _offsetParameters.lineShift = mosaicSizeX + _parameters.startX - 
                                _parameters.endX;

}


void soma::MosaicDicomImage::setLineShift( int32_t lineShift )
{

  _offsetParameters.lineShift = lineShift;

}


void soma::MosaicDicomImage::setOffset( int32_t offset )
{

  _offsetParameters.offset = offset;

}
