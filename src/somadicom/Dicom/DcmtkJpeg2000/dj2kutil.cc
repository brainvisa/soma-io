#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kutil.h>
#else
#include <Dicom/DcmtkJpeg2000/dj2kutil.h>
#endif

#include <dcmtk/dcmdata/dcerror.h>


OFLogger DCM_dcmjp2kGetLogger()
{

  // We don't just use a global variable, because constructors of globals are
  // executed in random order. This guarantees that the OFLogger is constructed
  // before first use.
  static OFLogger DCM_dcmjp2kLogger = OFLog::getLogger( "dcmtk.dcmjp2k" );

  return DCM_dcmjp2kLogger;

}

#define MAKE_DCMJP2K_ERROR( number, name, description ) \
const OFConditionConst ECC_##name( OFM_dcmjp2k, number, OF_error, description ); \
const OFCondition      EC_##name( ECC_##name )


MAKE_DCMJP2K_ERROR( 1, J2KUncompressedBufferTooSmall, "Uncompressed pixel data too short for uncompressed image");
MAKE_DCMJP2K_ERROR( 2, J2KCompressedBufferTooSmall, "Allocated too small buffer for compressed image data" );
MAKE_DCMJP2K_ERROR( 3, J2KCodecUnsupportedImageType, "Codec does not support this JPEG-2000 image" );
MAKE_DCMJP2K_ERROR( 4, J2KUnsupportedBitDepth, "Unsupported bit depth in JPEG-2000 transfer syntax" );
MAKE_DCMJP2K_ERROR( 5, J2KUnsupportedImageType, "Unsupported type of image for JPEG-2000 compression" );
MAKE_DCMJP2K_ERROR( 6, J2KNotSupportedYet, "Not supported yet" );
MAKE_DCMJP2K_ERROR( 7, J2KJasperInitializationFailure, "Jasper: initialization failed" );
MAKE_DCMJP2K_ERROR( 8, J2KJasperCannotOpenStream, "Jasper: cannot open stream" );
MAKE_DCMJP2K_ERROR( 9, J2KJasperDecodeFailure, "Jasper: decoding failed" );
