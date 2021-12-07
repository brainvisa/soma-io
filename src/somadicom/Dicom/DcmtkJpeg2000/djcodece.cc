#include <dcmtk/config/osconfig.h>

#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djcodece.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djrparam.h>
#else
#include <Dicom/DcmtkJpeg2000/djcodece.h>
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#include <Dicom/DcmtkJpeg2000/djrparam.h>
#endif

#include <dcmtk/ofstd/ofstream.h>
#include <dcmtk/ofstd/offile.h>
#include <dcmtk/ofstd/ofstd.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcpixseq.h>
#include <dcmtk/dcmdata/dcpxitem.h>
#include <dcmtk/dcmdata/dcuid.h>

#ifdef HAS_OPENJPEG
#include <openjpeg.h>
#elif defined( HAS_JASPER )
#include <jasper/jasper.h>
#endif

#if OFFIS_DCMTK_VERSION_NUMBER >= 361
  // OFCondition has undergone an incompatible change between 3.6.0 and 3.6.1
#  define build_OFCondition OFCondition
#else
#  define build_OFCondition OFConditionConst
#endif


DJ2KEncoderBase::DJ2KEncoderBase()
               : DcmCodec()
{
}


DJ2KEncoderBase::~DJ2KEncoderBase()
{
}


// the API changed in dcmtk 3.6.6
#if PACKAGE_VERSION_NUMBER >= 366
OFCondition DJ2KEncoderBase::decode(
                           const DcmRepresentationParameter* /* fromRepParam */,
                           DcmPixelSequence* /* pixSeq */,
                           DcmPolymorphOBOW& /* uncompressedPixelData */,
                           const DcmCodecParameter* /* cp */,
                           const DcmStack& /* objStack */,
                           OFBool & /*removeOldRep*/ ) const
#else
OFCondition DJ2KEncoderBase::decode(
                           const DcmRepresentationParameter* /* fromRepParam */,
                           DcmPixelSequence* /* pixSeq */,
                           DcmPolymorphOBOW& /* uncompressedPixelData */,
                           const DcmCodecParameter* /* cp */,
                           const DcmStack& /* objStack */ ) const
#endif
{

  return EC_IllegalCall;

}


OFCondition DJ2KEncoderBase::decodeFrame(
                              const DcmRepresentationParameter* /* fromParam */,
                              DcmPixelSequence* /* fromPixSeq */,
                              const DcmCodecParameter* /* cp */,
                              DcmItem* /* dataset */,
                              Uint32 /* frameNo */,
                              Uint32& /* startFragment */,
                              void* /* buffer */,
                              Uint32 /* bufSize */,
                              OFString& /* decompressedColorModel */ ) const
{

  return EC_IllegalCall;

}


// the API changed in dcmtk 3.6.6
#if PACKAGE_VERSION_NUMBER >= 366
OFCondition DJ2KEncoderBase::encode(
                             const Uint16* /* pixelData */,
                             const Uint32 /* length */,
                             const DcmRepresentationParameter* /* toRepParam */,
                             DcmPixelSequence*& /* pixSeq */,
                             const DcmCodecParameter* /* cp */,
                             DcmStack& /* objStack */,
                             OFBool & /*removeOldRep*/ ) const
#else
OFCondition DJ2KEncoderBase::encode(
                             const Uint16* /* pixelData */,
                             const Uint32 /* length */,
                             const DcmRepresentationParameter* /* toRepParam */,
                             DcmPixelSequence*& /* pixSeq */,
                             const DcmCodecParameter* /* cp */,
                             DcmStack& /* objStack */ ) const
#endif
{
#if 0
                                   const Uint16* pixelData,
                                   const Uint32 length,
                                   const DcmRepresentationParameter* toRepParam,
                                   DcmPixelSequence*& pixSeq,
                                   const DcmCodecParameter* cp,
                                   DcmStack& objStack ) const
{

  if ( jas_init() )
  {

    //return EC_J2KJasperInitializationFailure;
    return OFCondition( OFM_dcmjp2k, 7, OF_error,
                        "Jasper: initialization failed" );

  }

  OFCondition result = EC_Normal;

  // retrieve pointer to dataset from parameter stack
  DcmStack localStack( objStack );
  (void)localStack.pop();
  DcmObject* dobject = localStack.pop();

  if ( !dobject || ( ( dobject->ident() != EVR_dataset ) && 
                     ( dobject->ident() != EVR_item ) ) )
  {

    return EC_InvalidTag;

  }

  DcmItem *dataset = OFstatic_cast( DcmItem *, dobject );

  // assume we can cast the codec and representation parameters to what we need
  const DJ2KCodecParameter *djcp = 
                            OFreinterpret_cast( const DJ2KCodecParameter*, cp );
  const DJ2KRepresentationParameter *djrp = 
           OFreinterpret_cast( const DJ2KRepresentationParameter*, toRepParam );

  Uint16 quality = djrp->getQuality();

  if ( ( supportedTransferSyntax() == EXS_JPEG2000LosslessOnly ) ||
       djrp->useLosslessProcess() )
  {

    quality = 100;

  }

  Uint8 colorType;
  EP_Interpretation photometricInterpretation = getPhotometricInterpretation(
                                                                      dataset );

  switch ( photometricInterpretation )
  {

    case EPI_Monochrome1:
    case EPI_Monochrome2:
      colorType = JAS_CLRSPC_FAM_GRAY;
      break;

    case EPI_PaletteColor:
    case EPI_RGB:
    case EPI_HSV:
    case EPI_ARGB:
    case EPI_CMYK:
      colorType = JAS_CLRSPC_FAM_RGB;
      break;

    case EPI_YBR_Partial_422:
      colorType = JAS_CLRSPC_FAM_YCBCR_422;
      break;

    case EPI_YBR_Full:
    case EPI_YBR_Full_422:
      colorType = CLRSPC_FAM_YCBCR_F422;
      break;

    case EPI_Unknown:
    case EPI_Missing:
      result = EJ_UnsupportedPhotometricInterpretation;
      break;

  }

  if ( result.bad() )
  {

    return result;

  }






  // determine image properties
  double compressionRatio = 0.0;
  const Uint16* pixelData;
  unsigned long length = 0;
  Uint16 bitsAllocated = 0;
  Uint16 bitsStored = 0;
  Uint16 bytesAllocated = 0;
  Uint16 samplesPerPixel = 0;
  Uint16 planarConfiguration = 0;
  Uint16 columns = 0;
  Uint16 rows = 0;
  Sint32 numberOfFrames = 1;
  OFBool byteSwapped = OFFalse;  // true if we have byte-swapped the original pixel data
  OFString photometricInterpretation;
  DcmElement* dummyElem;

  result = dataset->findAndGetUint16( DCM_BitsAllocated, bitsAllocated );
  if ( result.good() )
  {

    result = dataset->findAndGetUint16( DCM_BitsStored, bitsStored );

  }
  if ( result.good() )
  {

    result = dataset->findAndGetUint16( DCM_SamplesPerPixel, samplesPerPixel );

  }
  if ( result.good() )
  {

    result = dataset->findAndGetUint16( DCM_Columns, columns );

  }
  if ( result.good() )
  {

    result = dataset->findAndGetUint16( DCM_Rows, rows );

  }
  if ( result.good() )
  {

    result = dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                          photometricInterpretation );

  }
  if ( result.good() )
  {

    result = datsetItem->findAndGetElement( DCM_PixelData, dummyElem );

  }
  if ( result.good() )
  {

    length = dummyElem->getLength();
    result = dataset->findAndGetSint32( DCM_NumberOfFrames, numberOfFrames );

    if ( result.bad() || ( numberOfFrames < 1 ) ) 
    {

      numberOfFrames = 1;

    }

    result = EC_Normal;

  }
  if ( result.good() )
  {

    // check if bitsAllocated is 8 or 16 - we don't handle anything else
    if ( bitsAllocated == 8 )
    {

      bytesAllocated = 1;

    }
    else if ( bitsAllocated == 16 )
    {

      bytesAllocated = 2;

    }
    else
    {

      //result = EC_J2KUnsupportedImageType;
      result = OFCondition( OFM_dcmjp2k, 3, OF_error,
                            "Codec does not support this JPEG-2000 image" );

    }

    // make sure that all the descriptive attributes have sensible values
    if ( ( columns < 1 ) || ( rows < 1 ) || ( samplesPerPixel < 1 ) )
    {

      //result = EC_J2KUnsupportedImageType;
      result = OFCondition( OFM_dcmjp2k, 3, OF_error,
                            "Codec does not support this JPEG-2000 image" );

    }

    // make sure that we have at least as many bytes of pixel data as we expect
    if ( ( bytesAllocated * samplesPerPixel * columns * rows *
           OFstatic_cast( unsigned long, numberOfFrames ) ) > length )
    {

      //result = EC_J2KUncompressedBufferTooSmall;
      result = OFCondition(
                  OFM_dcmjp2k, 1, OF_error,
                  "Uncompressed pixel data too short for uncompressed image" );

    }
  if ( result.good() && ( samplesPerPixel > 1 ) )
  {

    result = dataset->findAndGetUint16( DCM_PlanarConfiguration, 
                                        planarConfiguration );

  }

  }











  if ( result.good() && ( dataset->ident() == EVR_dataset ) )
  {

    if (result.good())
    {

      if ( ( supportedTransferSyntax() == EXS_JPEG2000LosslessOnly || 
           djrp->useLosslessProcess() )
      {

        if ( djcp->getConvertToSC() || 
             ( djcp->getUIDCreation() == EJ2KUC_always ) )
        {

          result = DcmCodec::newInstance( dataset, "DCM", "121320", 
                                          "Uncompressed predecessor" );

        }

      }
      else
      {

        if ( djcp->getConvertToSC() || 
             ( djcp->getUIDCreation() != EJ2KUC_never ) )
        {

          result = DcmCodec::newInstance( dataset, "DCM", "121320", 
                                          "Uncompressed predecessor" );

        }

        // update image type
        if ( result.good() )
        {

          result = DcmCodec::updateImageType( dataset );

        }

        // update derivation description
        if ( result.good() )
        {

          result = updateDerivationDescription( dataset, compressionRatio );

        }

        // update lossy compression ratio
        if ( result.good() )
        {

          result = updateLossyCompressionRatio( dataset, compressionRatio );

        }

      }

    }

    // convert to Secondary Capture if requested by user.
    // This method creates a new SOP class UID, so it should be executed
    // after the call to newInstance() which creates a Source Image Sequence.
    if ( result.good() && djcp->getConvertToSC() )
    {

      result = DcmCodec::convertToSecondaryCapture( dataset );

    }

  }

  return result;
#endif

  //return EC_J2KNotSupportedYet;
  return build_OFCondition( OFM_dcmjp2k, 6, OF_error, "Not supported yet" );

}


// the API changed in dcmtk 3.6.6
#if PACKAGE_VERSION_NUMBER >= 366
OFCondition DJ2KEncoderBase::encode(
                           const E_TransferSyntax /* fromRepType */,
                           const DcmRepresentationParameter* /* fromRepParam */,
                           DcmPixelSequence* /* fromPixSeq */,
                           const DcmRepresentationParameter* /* toRepParam */,
                           DcmPixelSequence*& /* toPixSeq */,
                           const DcmCodecParameter* /* cp */,
                           DcmStack& /* objStack */,
                           OFBool & /*removeOldRep*/ ) const
#else
OFCondition DJ2KEncoderBase::encode(
                           const E_TransferSyntax /* fromRepType */,
                           const DcmRepresentationParameter* /* fromRepParam */,
                           DcmPixelSequence* /* fromPixSeq */,
                           const DcmRepresentationParameter* /* toRepParam */,
                           DcmPixelSequence*& /* toPixSeq */,
                           const DcmCodecParameter* /* cp */,
                           DcmStack& /* objStack */ ) const
#endif
{

  // we don't support re-coding for now.
  return EC_IllegalCall;

}


OFBool DJ2KEncoderBase::canChangeCoding(
                                       const E_TransferSyntax oldRepType,
                                       const E_TransferSyntax newRepType ) const
{

  // this codec only handles conversion from uncompressed to JPEG-2000.

  DcmXfer oldRep( oldRepType );

  return ( oldRep.isNotEncapsulated() && 
           ( newRepType == supportedTransferSyntax() ) );

}


OFCondition DJ2KEncoderBase::determineDecompressedColorModel(
                              const DcmRepresentationParameter* /* fromParam */,
                              DcmPixelSequence* /* fromPixSeq */,
                              const DcmCodecParameter* /* cp */,
                              DcmItem* /* dataset */,
                              OFString& /* decompressedColorModel */ ) const
{

  return EC_IllegalCall;

}


OFCondition DJ2KEncoderBase::losslessRawEncode(
                                 const Uint16 * /* pixelData */,
                                 const Uint32 /* length */,
                                 DcmItem * /* dataset */,
                                 const DJ2KRepresentationParameter * /* djrp */,
                                 DcmPixelSequence * & /* pixSeq */,
                                 const DJ2KCodecParameter * /* djcp */,
                                 double& /* compressionRatio */ ) const
{

  OFCondition result;

  return result;

}


OFCondition DJ2KEncoderBase::updateLossyCompressionRatio(
                                                      DcmItem * /* dataset */, 
                                                      double /* ratio */ ) const
{

  OFCondition result;

  return result;

}


OFCondition DJ2KEncoderBase::updateDerivationDescription(
                                 DcmItem * /* dataset */,
                                 const DJ2KRepresentationParameter * /* djrp */,
                                 double /* ratio */ ) const
{

  OFCondition result;

  return result;

}


OFCondition DJ2KEncoderBase::compressRawFrame(
                                const Uint8 * /* framePointer */,
                                Uint16 /* bitsAllocated */,
                                Uint16 /* columns */,
                                Uint16 /* rows */,
                                Uint16 /* samplesPerPixel */,
                                Uint16 /* planarConfiguration */,
                                const OFString& /* photometricInterpretation */,
                                DcmPixelSequence * /* pixelSequence */,
                                DcmOffsetList & /* offsetList */,
                                unsigned long & /* compressedSize */,
                                const DJ2KCodecParameter * /* djcp */ ) const
{

  OFCondition result;

  return result;

}


E_TransferSyntax DJ2KLosslessEncoder::supportedTransferSyntax() const
{

  return EXS_JPEG2000LosslessOnly;

}


E_TransferSyntax DJ2KEncoder::supportedTransferSyntax() const
{

  return EXS_JPEG2000;

}
