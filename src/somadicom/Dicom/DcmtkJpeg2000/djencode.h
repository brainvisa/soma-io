#ifndef _DcmtkJpeg2000_djencode_h_
#define _DcmtkJpeg2000_djencode_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kutil.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#else
#include <Dicom/DcmtkJpeg2000/dj2kutil.h>
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#endif

#include <dcmtk/ofstd/oftypes.h>
#include <dcmtk/dcmdata/dctypes.h>


class DJ2KCodecParameter;
class DJ2KLosslessEncoder;
class DJ2KEncoder;


class DJ2KEncoderRegistration
{

  public:

  static void registerCodecs( Uint32 fragmentSize = 0,
                              OFBool createOffsetTable = OFTrue,
                              J2K_UIDCreation uidCreation = EJ2KUC_default,
                              OFBool convertToSC = OFFalse );

  static void cleanup();

  static OFString getLibraryVersionString();

  private:

  static OFBool registered_;

  static DJ2KCodecParameter* cp_;

  static DJ2KLosslessEncoder* losslessencoder_;

  static DJ2KEncoder *encoder_;

};


#endif
