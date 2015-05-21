#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djdecode.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcodecd.h>
#else
#include <Dicom/DcmtkJpeg2000/djdecode.h>
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#include <Dicom/DcmtkJpeg2000/djcodecd.h>
#endif

#include <dcmtk/dcmdata/dccodec.h>
#include <jasper/jas_version.h>


OFBool DJ2KDecoderRegistration::registered_ = OFFalse;
DJ2KCodecParameter* DJ2KDecoderRegistration::cp_ = NULL;
DJ2KLosslessDecoder* DJ2KDecoderRegistration::losslessdecoder_ = NULL;
DJ2KDecoder* DJ2KDecoderRegistration::decoder_ = NULL;


void DJ2KDecoderRegistration::registerCodecs( J2K_UIDCreation uidCreation )
{

  if ( !registered_ )
  {

    cp_ = new DJ2KCodecParameter( 0, OFTrue, uidCreation );

    if ( cp_ )
    {

      losslessdecoder_ = new DJ2KLosslessDecoder();

      if ( losslessdecoder_ )
      {

        DcmCodecList::registerCodec( losslessdecoder_, NULL, cp_ );

      }

      decoder_ = new DJ2KDecoder();

      if ( decoder_ )
      {

        DcmCodecList::registerCodec( decoder_, NULL, cp_ );

      }

      registered_ = OFTrue;

    }

  }

}


void DJ2KDecoderRegistration::cleanup()
{

  if ( registered_ )
  {

    DcmCodecList::deregisterCodec( losslessdecoder_ );
    DcmCodecList::deregisterCodec( decoder_ );

    delete losslessdecoder_;
    delete decoder_;
    delete cp_;

    registered_ = OFFalse;

#ifdef DEBUG
    losslessdecoder_ = NULL;
    decoder_ = NULL;
    cp_ = NULL;
#endif

  }

}


OFString DJ2KDecoderRegistration::getLibraryVersionString()
{

  return "Jasper, Version" JAS_VERSION;

}
