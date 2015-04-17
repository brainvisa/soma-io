#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceDicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MultiSliceDicomImage.h>
#include <Container/DicomProxy.h>
#endif


dcm::MultiSliceDicomImage::MultiSliceDicomImage(
                                       dcm::DicomProxy& proxy,
                                       dcm::ImagePixel::Parameters& parameters )
                         : dcm::DicomImage( proxy, parameters )
{

  _offsetParameters.rgbPlaneSize *= _parameters.sizeZ;

}
