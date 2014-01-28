#include <soma-io/Dicom/DcmtkJpeg2000/djencode.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kcode.h>

#include <dcmtk/dcmdata/dccodec.h>
#include <jasper/jas_version.h>


OFBool DJ2KEncoderRegistration::registered_ = OFFalse;
DJ2KCodecParameter* DJ2KEncoderRegistration::cp_ = NULL;
DJ2KImageCompressionLosslessEncoder*
                               DJ2KEncoderRegistration::losslessencoder_ = NULL;
DJ2KImageCompressionEncoder* DJ2KEncoderRegistration::encoder_ = NULL;


void DJ2KEncoderRegistration::registerCodecs( J2K_UIDCreation uidCreation )
{

  if ( !registered_ )
  {

    cp_ = new DJ2KCodecParameter( uidCreation );

    if ( cp_ )
    {

      losslessencoder_ = new DJ2KImageCompressionLosslessEncoder();

      if ( losslessencoder_ )
      {

        DcmCodecList::registerCodec( losslessencoder_, NULL, cp_ );

      }

      encoder_ = new DJ2KImageCompressionEncoder();

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

  return "Jasper, Version" JAS_VERSION;

}
