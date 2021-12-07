#ifndef _DcmtkJpeg2000_djcodecd_h_
#define _DcmtkJpeg2000_djcodecd_h_


#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dccodec.h>
#include <dcmtk/ofstd/ofstring.h>


class DJ2KCodecParameter;


class DJ2KDecoderBase : public DcmCodec
{

  public:

    DJ2KDecoderBase();
    virtual ~DJ2KDecoderBase();

// the API changed in dcmtk 3.6.6
#if PACKAGE_VERSION_NUMBER >= 366
    virtual OFCondition decode( const DcmRepresentationParameter* fromRepParam,
                                DcmPixelSequence* pixSeq,
                                DcmPolymorphOBOW& uncompressedPixelData,
                                const DcmCodecParameter* cp,
                                const DcmStack& objStack,
                                OFBool & removeOldRep ) const;
#else
    virtual OFCondition decode( const DcmRepresentationParameter* fromRepParam,
                                DcmPixelSequence* pixSeq,
                                DcmPolymorphOBOW& uncompressedPixelData,
                                const DcmCodecParameter* cp,
                                const DcmStack& objStack ) const;
#endif

    virtual OFCondition decodeFrame(
                                    const DcmRepresentationParameter* fromParam,
                                    DcmPixelSequence* fromPixSeq,
                                    const DcmCodecParameter* cp,
                                    DcmItem* dataset,
                                    Uint32 frameNo,
                                    Uint32& startFragment,
                                    void* buffer,
                                    Uint32 bufSize,
                                    OFString& decompressedColorModel ) const;

// the API changed in dcmtk 3.6.6
#if PACKAGE_VERSION_NUMBER >= 366
    virtual OFCondition encode( const Uint16* pixelData,
                                const Uint32 length,
                                const DcmRepresentationParameter* toRepParam,
                                DcmPixelSequence*& pixSeq,
                                const DcmCodecParameter* cp,
                                DcmStack& objStack,
                                OFBool & removeOldRep ) const;

    virtual OFCondition encode( const E_TransferSyntax fromRepType,
                                const DcmRepresentationParameter* fromRepParam,
                                DcmPixelSequence* fromPixSeq,
                                const DcmRepresentationParameter* toRepParam,
                                DcmPixelSequence*& toPixSeq,
                                const DcmCodecParameter* cp,
                                DcmStack& objStack,
                                OFBool & removeOldRep ) const;
#else
    virtual OFCondition encode( const Uint16* pixelData,
                                const Uint32 length,
                                const DcmRepresentationParameter* toRepParam,
                                DcmPixelSequence*& pixSeq,
                                const DcmCodecParameter* cp,
                                DcmStack& objStack ) const;

    virtual OFCondition encode( const E_TransferSyntax fromRepType,
                                const DcmRepresentationParameter* fromRepParam,
                                DcmPixelSequence* fromPixSeq,
                                const DcmRepresentationParameter* toRepParam,
                                DcmPixelSequence*& toPixSeq,
                                const DcmCodecParameter* cp,
                                DcmStack& objStack ) const;
#endif

    virtual OFBool canChangeCoding( const E_TransferSyntax oldRepType,
                                    const E_TransferSyntax newRepType ) const;

    virtual OFCondition determineDecompressedColorModel(
                                    const DcmRepresentationParameter* fromParam,
                                    DcmPixelSequence* fromPixSeq,
                                    const DcmCodecParameter* cp,
                                    DcmItem* dataset,
                                    OFString& decompressedColorModel ) const;

  private:

    static OFCondition decodeFrame( DcmPixelSequence* fromPixSeq,
                                    const DJ2KCodecParameter *cp,
                                    DcmItem *dataset,
                                    Uint32 frameNo,
                                    Uint32& startFragment,
                                    void* buffer,
                                    Uint32 bufSize,
                                    Sint32 imageFrames,
                                    Uint16 imageColumns,
                                    Uint16 imageRows,
                                    Uint16 imageSamplesPerPixel,
                                    Uint16 bytesPerSample );

    static Uint32 computeNumberOfFragments( Sint32 numberOfFrames,
                                            Uint32 currentFrame,
                                            Uint32 startItem,
                                            OFBool ignoreOffsetTable,
                                            DcmPixelSequence *pixSeq );

};


class DJ2KLosslessDecoder : public DJ2KDecoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};


class DJ2KDecoder : public DJ2KDecoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};


#endif
