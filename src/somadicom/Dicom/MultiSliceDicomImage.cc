#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiSliceDicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MultiSliceDicomImage.h>
#include <Container/DicomProxy.h>
#endif


soma::MultiSliceDicomImage::MultiSliceDicomImage(
                                      soma::DicomProxy& proxy,
                                      soma::ImagePixel::Parameters& parameters )
                          : soma::DicomImage( proxy, parameters )
{

  _offsetParameters.rgbPlaneSize *= _parameters.sizeZ;

}
