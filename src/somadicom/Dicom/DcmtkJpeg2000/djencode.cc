#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djencode.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcodece.h>
#else
#include <Dicom/DcmtkJpeg2000/djencode.h>
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#include <Dicom/DcmtkJpeg2000/djcodece.h>
#endif

#include <dcmtk/dcmdata/dccodec.h>

#ifdef HAS_OPENJPEG
#include <openjpeg.h>
#elif defined( HAS_JASPER )
#include <jasper/jas_version.h>
#endif


OFBool DJ2KEncoderRegistration::registered_ = OFFalse;
DJ2KCodecParameter* DJ2KEncoderRegistration::cp_ = NULL;
DJ2KLosslessEncoder* DJ2KEncoderRegistration::losslessencoder_ = NULL;
DJ2KEncoder* DJ2KEncoderRegistration::encoder_ = NULL;


void DJ2KEncoderRegistration::registerCodecs( Uint32 fragmentSize,
                                              OFBool createOffsetTable,
                                              J2K_UIDCreation uidCreation,
                                              OFBool convertToSC )
{

  if ( !registered_ )
  {

    cp_ = new DJ2KCodecParameter( fragmentSize, 
                                  createOffsetTable, 
                                  uidCreation,
                                  convertToSC );

    if ( cp_ )
    {

      losslessencoder_ = new DJ2KLosslessEncoder();

      if ( losslessencoder_ )
      {

        DcmCodecList::registerCodec( losslessencoder_, NULL, cp_ );

      }

      encoder_ = new DJ2KEncoder();

      if ( encoder_ )
      {

        DcmCodecList::registerCodec( encoder_, NULL, cp_ );

      }

      registered_ = OFTrue;

    }

  }

}


void DJ2KEncoderRegistration::cleanup()
{

  if ( registered_ )
  {

    DcmCodecList::deregisterCodec( losslessencoder_ );
    DcmCodecList::deregisterCodec( encoder_ );

    delete losslessencoder_;
    delete encoder_;
    delete cp_;

    registered_ = OFFalse;

#ifdef DEBUG
    losslessencoder_ = NULL;
    encoder_ = NULL;
    cp_ = NULL;
#endif

  }

}


OFString DJ2KEncoderRegistration::getLibraryVersionString()
{

#ifdef HAS_OPENJPEG
  std::string text = "OpenJpeg, Version " + std::string( opj_version() );
  return text.c_str();
#elif defined( HAS_JASPER )
  return "Jasper, Version" JAS_VERSION;
#endif

}
