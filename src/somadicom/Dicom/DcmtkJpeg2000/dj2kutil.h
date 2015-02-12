#ifndef _DcmtkJpeg2000_dj2kutil_h_
#define _DcmtkJpeg2000_dj2kutil_h_


#include <dcmtk/config/osconfig.h>
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/oflog/oflog.h>


// global definitions for logging mechanism provided by the oflog module

extern OFLogger DCM_dcmjp2kLogger;

#define DCMJP2K_TRACE(msg) OFLOG_TRACE(DCM_dcmjp2kLogger, msg)
#define DCMJP2K_DEBUG(msg) OFLOG_DEBUG(DCM_dcmjp2kLogger, msg)
#define DCMJP2K_INFO(msg)  OFLOG_INFO(DCM_dcmjp2kLogger, msg)
#define DCMJP2K_WARN(msg)  OFLOG_WARN(DCM_dcmjp2kLogger, msg)
#define DCMJP2K_ERROR(msg) OFLOG_ERROR(DCM_dcmjp2kLogger, msg)
#define DCMJP2K_FATAL(msg) OFLOG_FATAL(DCM_dcmjp2kLogger, msg)


/** describes the condition under which a compressed or decompressed image
 *  receives a new SOP instance UID.
 */
enum J2K_UIDCreation
{

  /** Upon compression, assign new SOP instance UID if compression is lossy.
   *  Upon decompression never assign new SOP instance UID.
   */
  EJ2KUC_default,

  /// always assign new SOP instance UID on compression and decompression
  EJ2KUC_always,

  /// never assign new SOP instance UID
  EJ2KUC_never

};


// CONDITION CONSTANTS

extern const OFCondition EC_J2KUncompressedBufferTooSmall;
extern const OFCondition EC_J2KCompressedBufferTooSmall;
extern const OFCondition EC_J2KCodecUnsupportedImageType;
extern const OFCondition EC_J2KUnsupportedBitDepth;
extern const OFCondition EC_J2KUnsupportedImageType;
extern const OFCondition EC_J2KNotSupportedYet;
extern const OFCondition EC_J2KJasperInitializationFailure;
extern const OFCondition EC_J2KJasperCannotOpenStream;
extern const OFCondition EC_J2KJasperDecodeFailure;


#endif
