#include <dcmtk/config/osconfig.h>

#include <cstdlib>

#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djcodecd.h>
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#else
#include <Dicom/DcmtkJpeg2000/djcodecd.h>
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
    //return EC_J2KJasperInitializationFailure;
    return OFCondition( OFM_dcmjp2k, 7, OF_error,
                        "Jasper: initialization failed" );

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

    //return EC_J2KUnsupportedBitDepth;
    return OFCondition( OFM_dcmjp2k, 4, OF_error,
                        "Unsupported bit depth in JPEG-2000 transfer syntax");

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
  const DJ2KCodecParameter *djcp = OFreinterpret_cast( 
                                                const DJ2KCodecParameter*, cp );

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
  Uint32 currentItem = 1; // item 0 contains the offset table
  OFBool done = OFFalse;

  while ( result.good() && !done )
  {

      result = decodeFrame( pixSeq, djcp, dataset, currentFrame, currentItem,
                            pixeldata8, frameSize, imageFrames, imageColumns,
                            imageRows, imageSamplesPerPixel, bytesPerSample );

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
                              const DcmCodecParameter* cp,
                              DcmItem* dataset,
                              Uint32 frameNo,
                              Uint32& startFragment,
                              void* buffer,
                              Uint32 bufSize,
                              OFString& decompressedColorModel ) const
{

  OFCondition result = EC_Normal;

  if ( jas_init() )
  {

    //return EC_J2KJasperInitializationFailure;
    return OFCondition( OFM_dcmjp2k, 7, OF_error,
                        "Jasper: initialization failed" );

  }

  const DJ2KCodecParameter *djcp = OFreinterpret_cast( 
                                                const DJ2KCodecParameter*, cp );

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

    //return EC_J2KUnsupportedBitDepth;
    return OFCondition( OFM_dcmjp2k, 4, OF_error,
                        "Unsupported bit depth in JPEG-2000 transfer syntax");

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

  // number of frames is an optional attribute - 
  // we don't mind if it isn't present.
  Sint32 imageFrames = 0;
  if ( dataset->findAndGetSint32( DCM_NumberOfFrames, imageFrames ).bad() )
  {

    return EC_TagNotFound;

  }

  if ( imageFrames >= OFstatic_cast( Sint32, fromPixSeq->card() ) )
  {

    imageFrames = fromPixSeq->card() - 1;

  }

  if ( imageFrames < 1 )
  {

    imageFrames = 1;

  }

  if ( startFragment == 0 )
  {

    result = determineStartFragment( frameNo, 
                                     imageFrames, 
                                     fromPixSeq, 
                                     startFragment );
  }

  if ( result.good() )
  {

    result = decodeFrame( fromPixSeq, djcp, dataset, frameNo, startFragment,
                          buffer, bufSize, imageFrames, imageColumns,
                          imageRows, imageSamplesPerPixel, bytesPerSample );

  }

  if ( result.good() )
  {

    // retrieve color model from given dataset
    result = dataset->findAndGetOFString( DCM_PhotometricInterpretation, 
                                          decompressedColorModel );

  }

  return result;

}


OFCondition DJ2KDecoderBase::decodeFrame( DcmPixelSequence* fromPixSeq,
                                          const DJ2KCodecParameter* /* cp */,
                                          DcmItem* /* dataset */,
                                          Uint32 frameNo,
                                          Uint32& startFragment,
                                          void* buffer,
                                          Uint32 bufSize,
                                          Sint32 imageFrames,
                                          Uint16 /* imageColumns */,
                                          Uint16 /* imageRows */,
                                          Uint16 imageSamplesPerPixel,
                                          Uint16 bytesPerSample )
{

  DcmPixelItem *pixItem = NULL;
  Uint8* j2kData = NULL;
  Uint8* j2kFragmentData = NULL;
  Uint32 fragmentLength = 0;
  size_t compressedSize = 0;
  OFCondition result = EC_Normal;
  Uint32 fragmentsForThisFrame = computeNumberOfFragments( imageFrames, 
                                                           frameNo, 
                                                           startFragment, 
                                                           OFFalse,
                                                           fromPixSeq );

  if ( fragmentsForThisFrame == 0 )
  {

    //result = EC_J2KCannotComputeNumberOfFragments;
    result = OFCondition(
                    OFM_dcmjp2k, 10, OF_error,
                    "Cannot compute number of fragments for JPEG-2000 frame" );

  }

  if ( result.good() )
  {

    Uint32 fragmentsForThisFrame2 = fragmentsForThisFrame;
    Uint32 startFragment2 = startFragment;

    while ( result.good() && fragmentsForThisFrame2-- )
    {

      result = fromPixSeq->getItem( pixItem, startFragment2++ );

      if ( result.good() && pixItem )
      {

        fragmentLength = pixItem->getLength();

        if ( result.good() )
        {

          compressedSize += fragmentLength;

        }

      }

    }

  }

  if ( result.good() )
  {

    Uint32 offset = 0;
    j2kData = new Uint8[ compressedSize ];

    while ( result.good() && fragmentsForThisFrame-- )
    {

      result = fromPixSeq->getItem( pixItem, startFragment++ );

      if ( result.good() && pixItem )
      {

        fragmentLength = pixItem->getLength();
        result = pixItem->getUint8Array( j2kFragmentData );

        if ( result.good() && j2kFragmentData )
        {

          std::memcpy( &j2kData[ offset ], j2kFragmentData, fragmentLength );
          offset += fragmentLength;

        }

      }

    }

  }

  if ( result.good() && j2kData )
  {

    jas_stream_t* streamIn = jas_stream_memopen( (char*)j2kData, 
                                                 compressedSize );

    if ( !streamIn )
    {

      //return EC_J2KJasperCannotOpenStream;
      return OFCondition( OFM_dcmjp2k, 8, OF_error,
                          "Jasper: cannot open stream" );

    }

    jas_image_t* image = jas_image_decode( streamIn, -1, 0 );

    jas_stream_close( streamIn );

    if ( !image )
    {

      //return EC_J2KJasperDecodeFailure;
      return OFCondition( OFM_dcmjp2k, 9, OF_error,
                          "Jasper: decoding failed" );

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
    else if(imageSamplesPerPixel == 3)
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

    } else {
      // we only handle one or three samples per pixel
      delete[] j2kData;
      return EC_InvalidTag;
    }

    delete[] j2kData;

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


Uint32 DJ2KDecoderBase::computeNumberOfFragments( Sint32 numberOfFrames,
                                                  Uint32 currentFrame,
                                                  Uint32 startItem,
                                                  OFBool ignoreOffsetTable,
                                                  DcmPixelSequence* pixSeq )
{

  unsigned long numItems = pixSeq->card();
  DcmPixelItem *pixItem = NULL;

  if ( ( numberOfFrames <= 1 ) || 
       ( currentFrame + 1 == OFstatic_cast( Uint32, numberOfFrames ) ) )
  {

    return ( numItems - startItem );

  }

  if ( OFstatic_cast( Uint32, numberOfFrames + 1 ) == numItems )
  {

    return 1;

  }

  OFCondition result = EC_Normal;

  if ( !ignoreOffsetTable )
  {

    result = pixSeq->getItem( pixItem, 0 );

    if ( result.good() && pixItem )
    {

      Uint32 offsetTableLength = pixItem->getLength();

      if ( offsetTableLength == 
           ( OFstatic_cast( Uint32, numberOfFrames ) * 4 ) )
      {

        Uint8* offsetData = NULL;

        result = pixItem->getUint8Array( offsetData );

        if ( result.good() && offsetData )
        {

          Uint32* offsetData32 = OFreinterpret_cast( Uint32*, offsetData );
          Uint32 offset = offsetData32[ currentFrame + 1 ];

          swapIfNecessary( gLocalByteOrder, EBO_LittleEndian, 
                           &offset, sizeof( Uint32 ), sizeof( Uint32 ) );

          Uint32 byteCount = 0;
          Uint32 fragmentIndex = 1;

          while ( ( byteCount < offset ) && ( fragmentIndex < numItems ) )
          {

            pixItem = NULL;
            result = pixSeq->getItem( pixItem, fragmentIndex++ );

            if ( result.good() && pixItem )
            {

              byteCount += pixItem->getLength() + 8;

              if ( ( byteCount == offset ) && ( fragmentIndex > startItem ) )
              {

                return fragmentIndex - startItem;

              }

            }
            else
            {

              break;

            }

          }

        }

      }

    }

  }

  // We're bust. No way to determine the number of fragments per frame.
  return 0;

}


E_TransferSyntax DJ2KLosslessDecoder::supportedTransferSyntax() const
{

  return EXS_JPEG2000LosslessOnly;

}


E_TransferSyntax DJ2KDecoder::supportedTransferSyntax() const
{

  return EXS_JPEG2000;

}
