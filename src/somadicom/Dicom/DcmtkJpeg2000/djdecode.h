#ifndef _DcmtkJpeg2000_djdecode_h_
#define _DcmtkJpeg2000_djdecode_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kutil.h>
#else
#include <Dicom/DcmtkJpeg2000/dj2kutil.h>
#endif

#include <dcmtk/ofstd/oftypes.h>


class DJ2KCodecParameter;
class DJ2KLosslessDecoder;
class DJ2KDecoder;


class DJ2KDecoderRegistration
{
public:

  /** registers decoder for all supported JPEG-2000 processes.
   *  If already registered, call is ignored unless cleanup() has
   *  been performed before.
   *  @param uidcreation flag indicating whether or not
   *    a new SOP Instance UID should be assigned upon decompression.
   *  @param planarconfig flag indicating how planar configuration
   *    of color images should be encoded upon decompression.
   *  @param ignoreOffsetTable flag indicating whether to ignore the offset table when decompressing multiframe images
   */
  static void registerCodecs( J2K_UIDCreation uidcreation = EJ2KUC_default );

  /** deregisters decoders.
   *  Attention: Must not be called while other threads might still use
   *  the registered codecs, e.g. because they are currently decoding
   *  DICOM data sets through dcmdata.
   */
  static void cleanup();

  /** get version information of the Jasper library.
   *  @return name and version number of the CharLS library
   */
  static OFString getLibraryVersionString();

private:

  /// flag indicating whether the decoders are already registered.
  static OFBool registered_;

  /// pointer to codec parameter shared by all decoders
  static DJ2KCodecParameter *cp_;

  /// pointer to image compression lossless decoder
  static DJ2KLosslessDecoder *losslessdecoder_;

  /// pointer to image compression decoder
  static DJ2KDecoder *decoder_;

};


#endif
