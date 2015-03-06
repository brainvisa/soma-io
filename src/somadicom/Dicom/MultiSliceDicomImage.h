#ifndef _MultiSliceDicomImage_h_
#define _MultiSliceDicomImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomImage.h>
#else
#include <Dicom/DicomImage.h>
#endif


namespace soma
{


class MultiSliceDicomImage : public DicomImage
{

  public:

    MultiSliceDicomImage( DicomProxy& proxy, 
                          ImagePixel::Parameters& parameters );

};


}


#endif
