#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiFrameDicomImage.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/MultiFrameDicomImage.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dccodec.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::MultiFrameDicomImage::MultiFrameDicomImage(
                                      soma::DicomProxy& proxy,
                                      soma::ImagePixel::Parameters& parameters )
                          : soma::DicomImage( proxy, parameters )
{
}


void soma::MultiFrameDicomImage::getImagePtr()
{

//#if OFFIS_DCMTK_VERSION_NUMBER < 360
#if 1
  soma::DicomImage::getImagePtr();
#endif

}


void soma::MultiFrameDicomImage::fillSlab( int32_t start, int32_t count )
{

//#if OFFIS_DCMTK_VERSION_NUMBER >= 360
#if 0
  soma::DataInfo& info = _proxy.getDataInfo();
  int32_t bufferSize = info._sliceSize * info._bpp * count;

  if ( bufferSize & 1 )
  {

    bufferSize++;

  }

  uint8_t* buffer = new uint8_t[ bufferSize ];

  if ( buffer )
  {

    Uint32 fragment;
    OFString decompressedColorModel;
    DcmPixelSequence *dseq = 0;
    E_TransferSyntax xferSyntax = EXS_Unknown;
    const DcmRepresentationParameter *rep = NULL;

    _pixelData->getOriginalRepresentationKey( xferSyntax, rep );

    if ( _pixelData->getEncapsulatedRepresentation( xferSyntax, 
                                                    rep, 
                                                    dseq ).good() )
    {

      if ( DcmCodec::determineStartFragment( start, 
                                             info._frames, 
                                             dseq, 
                                             fragment ).good() )
      {

        if ( _pixelData->getUncompressedFrame( _dataset, 
                                               start, 
                                               fragment, 
                                               buffer,
                                               bufferSize, 
                                               decompressedColorModel ).good() )
        {

          chooseImagePixel( decompressedColorModel.c_str() );

          int32_t t, s, endT = start + count;

          for ( t = start, s = 0; t < endT; t++, s++ )
          {

            _image->fill( _parameters, 
                          _offsetParameters,
                          (void*)buffer, 
                          0, 
                          t - _parameters.outLowerT,
                          s );

          }

        }

      }

    }

    delete [] buffer;

  }
#else
  int32_t i, stopIndex = start + count;

  for ( i = start; i < stopIndex; i++ )
  {

    fill( 0, i, i );

  }
#endif

}
