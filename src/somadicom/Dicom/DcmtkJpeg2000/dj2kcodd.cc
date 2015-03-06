#include <dcmtk/config/osconfig.h>

#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kcodd.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#else
#include <Dicom/DcmtkJpeg2000/dj2kcodd.h>
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#endif

#include <dcmtk/ofstd/ofstream.h>
#include <dcmtk/ofstd/ofcast.h>
#include <dcmtk/ofstd/offile.h>
#include <dcmtk/ofstd/ofstd.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcpixseq.h>
#include <dcmtk/dcmdata/dcpxitem.h>
#include <dcmtk/dcmdata/dcvrpobw.h>
#include <dcmtk/dcmdata/dcswap.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <jasper/jasper.h>


DJ2KDecoderBase::DJ2KDecoderBase()
               : DcmCodec()
{
}


DJ2KDecoderBase::~DJ2KDecoderBase()
{
}


OFCondition DJ2KDecoderBase::decode( 
                           const DcmRepresentationParameter* /* fromRepParam */,
                           DcmPixelSequence* pixSeq,
                           DcmPolymorphOBOW& uncompressedPixelData,
                           const DcmCodecParameter* cp,
                           const DcmStack& objStack ) const
{

  if ( jas_init() )
  {

    return EC_J2KJasperInitializationFailure;

  }

  // retrieve pointer to dataset from parameter stack
  DcmStack localStack( objStack );
  (void)localStack.pop();
  DcmObject* dobject = localStack.pop();

  if ( !dobject || ( ( dobject->ident() != EVR_dataset ) && 
                     ( dobject->ident() != EVR_item ) ) )
  {

    return EC_InvalidTag;

  }

  DcmItem* dataset = OFstatic_cast( DcmItem *, dobject );
  OFBool numberOfFramesPresent = OFFalse;

  // determine properties of uncompressed dataset
  Uint16 imageSamplesPerPixel = 0;
  if ( dataset->findAndGetUint16( DCM_SamplesPerPixel, 
                                  imageSamplesPerPixel ).bad() ) 
  {

    return EC_TagNotFound;

  }
  // we only handle one or three samples per pixel
  if ( ( imageSamplesPerPixel != 3 ) && ( imageSamplesPerPixel != 1 ) ) 
  {

    return EC_InvalidTag;

  }

  Uint16 imageRows = 0;
  if ( dataset->findAndGetUint16( DCM_Rows, imageRows ).bad() )
  {

    return EC_TagNotFound;

  }
  if ( imageRows < 1 )
  {

    return EC_InvalidTag;

  }

  Uint16 imageColumns = 0;
  if ( dataset->findAndGetUint16( DCM_Columns, imageColumns ).bad() )
  {

    return EC_TagNotFound;

  }
  if ( imageColumns < 1 )
  {

    return EC_InvalidTag;

  }

  // number of frames is an optional attribute - 
  // we don't mind if it isn't present.
  Sint32 imageFrames = 0;
  if ( dataset->findAndGetSint32( DCM_NumberOfFrames, imageFrames ).good() )
  {

    numberOfFramesPresent = OFTrue;

  }

  if ( imageFrames >= OFstatic_cast( Sint32, pixSeq->card() ) )
  {

    imageFrames = pixSeq->card() - 1;

  }
  if ( imageFrames < 1 )
  {

    imageFrames = 1;

  }

  Uint16 imageBitsStored = 0;
  if ( dataset->findAndGetUint16( DCM_BitsStored, imageBitsStored ).bad() )
  {

    return EC_TagNotFound;

  }

  Uint16 imageBitsAllocated = 0;
  if ( dataset->findAndGetUint16( DCM_BitsAllocated, 
                                  imageBitsAllocated ).bad() )
  {

    return EC_TagNotFound;

  }

  Uint16 imageHighBit = 0;
  if ( dataset->findAndGetUint16( DCM_HighBit, imageHighBit ).bad() )
  {

    return EC_TagNotFound;

  }

  //we only support up to 16 bits per sample
  if ( ( imageBitsStored < 1 ) || ( imageBitsStored > 16 ) )
  {

    return EC_J2KUnsupportedBitDepth;

  }

  // determine the number of bytes per sample (bits allocated) 
  // for the de-compressed object.
  Uint16 bytesPerSample = 1;
  if ( imageBitsStored > 8 )
  {

    bytesPerSample = 2;

  }
  else if ( imageBitsAllocated > 8 )
  {
    
    bytesPerSample = 2;

  }

  // compute size of uncompressed frame, in bytes
  Uint32 frameSize = bytesPerSample * imageRows * 
                     imageColumns * imageSamplesPerPixel;

  // compute size of pixel data attribute, in bytes
  Uint32 totalSize = frameSize * imageFrames;
  if ( totalSize & 1 )
  {

    totalSize++;

  }

  // assume we can cast the codec parameter to what we need
  const DJ2KCodecParameter *djcp = 
                            OFreinterpret_cast( const DJ2KCodecParameter*, cp );

  // determine planar configuration for uncompressed data
  OFString imageSopClass;
  OFString imagePhotometricInterpretation;
  dataset->findAndGetOFString( DCM_SOPClassUID, imageSopClass );
  dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                               imagePhotometricInterpretation );

  // allocate space for uncompressed pixel data element
  Uint16 *pixeldata16 = NULL;
  OFCondition result = uncompressedPixelData.createUint16Array(
                                                   totalSize / sizeof( Uint16 ), 
                                                   pixeldata16 );
  if ( result.bad() )
  {

    return result;

  }

  Uint8 *pixeldata8 = OFreinterpret_cast( Uint8 *, pixeldata16 );
  Sint32 currentFrame = 0;
  OFBool done = OFFalse;

  while ( result.good() && !done )
  {

      result = decodeFrame( pixSeq, pixeldata8, frameSize, 
                            imageSamplesPerPixel, bytesPerSample );

      if ( result.good() )
      {

        // increment frame number, check if we're finished
        if ( ++currentFrame == imageFrames )
        {

          done = OFTrue;

        }

        pixeldata8 += frameSize;

      }

  }

  // Number of Frames might have changed in case the previous value was wrong
  if ( result.good() && ( numberOfFramesPresent || ( imageFrames > 1 ) ) )
  {

    char numBuf[ 20 ];
    sprintf(numBuf, "%ld", OFstatic_cast( long, imageFrames ) );
    result = ((DcmItem *)dataset)->putAndInsertString( DCM_NumberOfFrames, 
                                                       numBuf );

  }

  if ( result.good() )
  {
    // the following operations do not affect the Image Pixel Module
    // but other modules such as SOP Common.  We only perform these
    // changes if we're on the main level of the dataset,
    // which should always identify itself as dataset, not as item.
    if ( ( dataset->ident() == EVR_dataset ) && 
         ( djcp->getUIDCreation() == EJ2KUC_always ) )
    {

        // create new SOP instance UID
        result = DcmCodec::newInstance( (DcmItem *)dataset, NULL, NULL, NULL );

    }

  }

  return result;

}


OFCondition DJ2KDecoderBase::decodeFrame(
                              const DcmRepresentationParameter* /* fromParam */,
                              DcmPixelSequence* fromPixSeq,
                              const DcmCodecParameter* /* cp */,
                              DcmItem* dataset,
                              Uint32 /* frameNo */,
                              Uint32& /* startFragment */,
                              void* buffer,
                              Uint32 bufSize,
                              OFString& decompressedColorModel ) const
{

  OFCondition result = EC_Normal;

  if ( jas_init() )
  {

    return EC_J2KJasperInitializationFailure;

  }

  // determine properties of uncompressed dataset
  Uint16 imageSamplesPerPixel = 0;
  if ( dataset->findAndGetUint16( DCM_SamplesPerPixel, 
                                  imageSamplesPerPixel ).bad() ) 
  {

    return EC_TagNotFound;

  }
  // we only handle one or three samples per pixel
  if ( ( imageSamplesPerPixel != 3 ) && ( imageSamplesPerPixel != 1 ) ) 
  {

    return EC_InvalidTag;

  }

  Uint16 imageRows = 0;
  if ( dataset->findAndGetUint16( DCM_Rows, imageRows ).bad() )
  {

    return EC_TagNotFound;

  }
  if ( imageRows < 1 )
  {

    return EC_InvalidTag;

  }

  Uint16 imageColumns = 0;
  if ( dataset->findAndGetUint16( DCM_Columns, imageColumns ).bad() )
  {

    return EC_TagNotFound;

  }
  if ( imageColumns < 1 )
  {

    return EC_InvalidTag;

  }

  // number of frames is an optional attribute - 
  // we don't mind if it isn't present.
  Sint32 imageFrames = 0;
  dataset->findAndGetSint32( DCM_NumberOfFrames, imageFrames );

  if ( imageFrames >= OFstatic_cast( Sint32, fromPixSeq->card() ) )
  {

    imageFrames = fromPixSeq->card() - 1;

  }
  if ( imageFrames < 1 )
  {

    imageFrames = 1;

  }

  Uint16 imageBitsStored = 0;
  if ( dataset->findAndGetUint16( DCM_BitsStored, imageBitsStored ).bad() )
  {

    return EC_TagNotFound;

  }

  Uint16 imageBitsAllocated = 0;
  if ( dataset->findAndGetUint16( DCM_BitsAllocated, 
                                  imageBitsAllocated ).bad() )
  {

    return EC_TagNotFound;

  }

  //we only support up to 16 bits per sample
  if ( ( imageBitsStored < 1 ) || ( imageBitsStored > 16 ) )
  {

    return EC_J2KUnsupportedBitDepth;

  }

  // determine the number of bytes per sample (bits allocated) 
  // for the de-compressed object.
  Uint16 bytesPerSample = 1;
  if ( imageBitsStored > 8 )
  {

    bytesPerSample = 2;

  }
  else if ( imageBitsAllocated > 8 )
  {
    
    bytesPerSample = 2;

  }

  result = decodeFrame( fromPixSeq, buffer, bufSize, 
                        imageSamplesPerPixel, bytesPerSample );

  if ( result.good() )
  {

    // retrieve color model from given dataset
    result = dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                          decompressedColorModel );

  }

  return result;

}


OFCondition DJ2KDecoderBase::decodeFrame( DcmPixelSequence* fromPixSeq,
                                          void* buffer,
                                          Uint32 bufSize,
                                          Uint16 imageSamplesPerPixel,
                                          Uint16 bytesPerSample )
{

  DcmPixelItem *pixItem = NULL;

  OFCondition result = fromPixSeq->getItem( pixItem, 1 );

  if ( result.good() )
  {

    Uint32 compressedLength = pixItem->getLength();
    Uint8* j2kData = NULL;

    result = pixItem->getUint8Array( j2kData );

    if ( result.good() && j2kData )
    {

      jas_stream_t* streamIn = jas_stream_memopen( (char*)j2kData, 
                                                   compressedLength );

      if ( !streamIn )
      {

        return EC_J2KJasperCannotOpenStream;

      }

      jas_image_t* image = jas_image_decode( streamIn, -1, 0 );

      jas_stream_close( streamIn );

      if ( !image )
      {

        return EC_J2KJasperDecodeFailure;

      }

      if ( ( imageSamplesPerPixel >= 3 ) && ( bytesPerSample <= 8 ) )
      {

        jas_cmprof_t* prof = jas_cmprof_createfromclrspc( JAS_CLRSPC_SRGB );

        if ( prof )
        {

          jas_image_t* altimage = jas_image_chclrspc( image, 
                                                      prof, 
                                                      JAS_CMXFORM_INTENT_PER );

          if ( altimage )
          {

            jas_image_destroy( image );
            image = altimage;

          }

          jas_cmprof_destroy( prof );

        }

      }

      if ( imageSamplesPerPixel == 1 )
      {

        jas_stream_t* streamOut = image->cmpts_[ 0 ]->stream_;

        jas_stream_rewind( streamOut );

        Uint32 streamLength = jas_stream_length( streamOut );
        Uint8* ptr = (Uint8*)buffer;

        if ( image->cmpts_[ 0 ]->cps_ == 2 )
        {

          while ( streamLength-- )
          {

            ptr[ 1 ] = jas_stream_getc( streamOut );
            *ptr++ = jas_stream_getc( streamOut );
            ptr++;
            streamLength--;

          }

        }
        else
        {

          while ( streamLength-- )
          {

            *ptr++ = jas_stream_getc( streamOut );

          }

        }

        if ( bytesPerSample == 1 )
        {

            result = swapIfNecessary( gLocalByteOrder, 
                                      EBO_LittleEndian, 
                                      buffer,
                                      bufSize, 
                                      sizeof( Uint16 ) );

        }

      }
      else
      {

        Sint32 i;
        jas_stream_t* streamOut[ 3 ];

        for ( i = 0; i < imageSamplesPerPixel; i++ )
        {

          streamOut[ i ] = image->cmpts_[ i ]->stream_;
          jas_stream_rewind( streamOut[ i ] );

        }

        Uint32 streamLength = jas_stream_length( streamOut[ 0 ] );
        Uint8* ptr = (Uint8*)buffer;

        while ( streamLength-- )
        {

          *ptr++ = jas_stream_getc( streamOut[ 0 ] );
          *ptr++ = jas_stream_getc( streamOut[ 1 ] );
          *ptr++ = jas_stream_getc( streamOut[ 2 ] );

        }

      }

    }

  }

  return result;

}


OFCondition DJ2KDecoderBase::encode(
                             const Uint16* /* pixelData */,
                             const Uint32 /* length */,
                             const DcmRepresentationParameter* /* toRepParam */,
                             DcmPixelSequence*& /* pixSeq */,
                             const DcmCodecParameter* /* cp */,
                             DcmStack& /* objStack */ ) const
{

  return EC_IllegalCall;

}


OFCondition DJ2KDecoderBase::encode(
                           const E_TransferSyntax /* fromRepType */,
                           const DcmRepresentationParameter* /* fromRepParam */,
                           DcmPixelSequence* /* fromPixSeq */,
                           const DcmRepresentationParameter* /* toRepParam */,
                           DcmPixelSequence*& /* toPixSeq */,
                           const DcmCodecParameter* /* cp */,
                           DcmStack& /* objStack */ ) const
{

  return EC_IllegalCall;

}


OFBool DJ2KDecoderBase::canChangeCoding(
                                       const E_TransferSyntax oldRepType,
                                       const E_TransferSyntax newRepType ) const
{

  // this codec only handles conversion from JPEG-2000 to uncompressed.

  DcmXfer newRep( newRepType );

  if ( newRep.isNotEncapsulated() &&
       ( ( oldRepType == EXS_JPEG2000LosslessOnly ) ||
         ( oldRepType == EXS_JPEG2000 ) ) )
  {

    return OFTrue;

  }

  return OFFalse;

}


OFCondition DJ2KDecoderBase::determineDecompressedColorModel(
                              const DcmRepresentationParameter* /* fromParam */,
                              DcmPixelSequence* /* fromPixSeq */,
                              const DcmCodecParameter* /* cp */,
                              DcmItem* dataset,
                              OFString& decompressedColorModel ) const
{

  OFCondition result = EC_IllegalParameter;

  if ( dataset != NULL )
  {

    result = dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                          decompressedColorModel );

  }

  return result;

}


E_TransferSyntax 
DJ2KImageCompressionLosslessDecoder::supportedTransferSyntax() const
{

  return EXS_JPEG2000LosslessOnly;

}


E_TransferSyntax 
DJ2KImageCompressionDecoder::supportedTransferSyntax() const
{

  return EXS_JPEG2000;

}

