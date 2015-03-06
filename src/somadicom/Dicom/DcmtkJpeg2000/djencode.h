#ifndef _DcmtkJpeg2000_djencode_h_
#define _DcmtkJpeg2000_djencode_h_


#include <dcmtk/config/osconfig.h>
#include <dcmtk/ofstd/oftypes.h>
#include <dcmtk/dcmdata/dctypes.h>

#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kutil.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#else
#include <Dicom/DcmtkJpeg2000/dj2kutil.h>
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#endif


class DJ2KCodecParameter;
class DJ2KImageCompressionLosslessEncoder;
class DJ2KImageCompressionEncoder;


class DJ2KEncoderRegistration
{

  public:

  static void registerCodecs( J2K_UIDCreation uidCreation = EJ2KUC_default );

  static void cleanup();

  static OFString getLibraryVersionString();

  private:

  static OFBool registered_;

  static DJ2KCodecParameter* cp_;

  static DJ2KImageCompressionLosslessEncoder* losslessencoder_;

  static DJ2KImageCompressionEncoder *encoder_;

};


#endif
