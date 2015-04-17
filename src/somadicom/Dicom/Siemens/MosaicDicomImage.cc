#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/MosaicDicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/Siemens/MosaicDicomImage.h>
#include <Container/DicomProxy.h>
#endif


dcm::MosaicDicomImage::MosaicDicomImage(
                                        dcm::DicomProxy& proxy, 
                                        dcm::ImagePixel::Parameters& parameters,
                                        int32_t mosaicSizeX )
                     : dcm::DicomImage( proxy, parameters )
{

  _offsetParameters.lineShift = mosaicSizeX + _parameters.startX - 
                                _parameters.endX;

}


void dcm::MosaicDicomImage::setLineShift( int32_t lineShift )
{

  _offsetParameters.lineShift = lineShift;

}


void dcm::MosaicDicomImage::setOffset( int32_t offset )
{

  _offsetParameters.offset = offset;

}
