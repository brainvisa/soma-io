#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/MosaicPointerImage.h>
#include <soma-io/Dicom/RGBImage.h>
#include <soma-io/Dicom/MonochromeImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/Siemens/MosaicPointerImage.h>
#include <Dicom/RGBImage.h>
#include <Dicom/MonochromeImage.h>
#include <Container/DicomProxy.h>
#endif


dcm::MosaicPointerImage::MosaicPointerImage(
                                        dcm::DicomProxy& proxy, 
                                        dcm::ImagePixel::Parameters& parameters,
                                        int32_t mosaicSizeX,
                                        void* ptr )
                       : dcm::MosaicDicomImage( proxy, 
                                                parameters, 
                                                mosaicSizeX )
{

  dcm::DataInfo& info = proxy.getDataInfo();

  _imagePtr = ptr;

  switch ( info._bpp )
  {

    case 3:
      _image = new dcm::RGBImage( _proxy, 0 );
      break;

    case 2:
      _image = new dcm::MonochromeImage< uint16_t >( _proxy );
      break;

    default:
      _image = new dcm::MonochromeImage< uint8_t >( _proxy );
      break;

  }

}
