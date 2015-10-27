#ifndef _DicomImage_h_
#define _DicomImage_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/ImageModule.h>
#include <soma-io/Dicom/ImagePixel.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/ImageModule.h>
#include <Dicom/ImagePixel.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcvr.h>

#include <string>


class DcmDataset;
class DcmPixelData;


namespace dcm
{


class PatientOrientation;
class DicomProxy;


class DicomImage
{

  public:

    DicomImage( DicomProxy& proxy, ImagePixel::Parameters& parameters );
    virtual ~DicomImage();

    bool load( const std::string& fileName );
    virtual void getImagePtr();
    void fillIndex( int32_t index, int32_t inputSlice = 0 );
    void fill( int32_t z, int32_t t, int32_t inputSlice = 0 );
    void getMinMaxValues( int32_t& min, int32_t& max );

  protected:

    virtual bool getPixelData( DcmDataset* dataset );

    void chooseImagePixel( const std::string& photometric );

    DicomProxy& _proxy;
    ImagePixel::Parameters _parameters;
    ImagePixel::OffsetParameters _offsetParameters;
    DcmFileFormat _dicomFile;
    DcmDataset* _dataset;
    DcmPixelData* _pixelData;
    ImagePixel* _image;
    ImageModule _imageModule;
    std::string _photometric;
    DcmEVR _dcmEVR;
    void* _imagePtr;

};


}


#endif
