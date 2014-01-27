#ifndef _DcmtkJpeg2000_dj2kcodd_h_
#define _DcmtkJpeg2000_dj2kcodd_h_


#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dccodec.h>
#include <dcmtk/ofstd/ofstring.h>


class DJ2KCodecParameter;


class DJ2KDecoderBase : public DcmCodec
{

  public:

    DJ2KDecoderBase();
    virtual ~DJ2KDecoderBase();

    virtual OFCondition decode( const DcmRepresentationParameter* fromRepParam,
                                DcmPixelSequence* pixSeq,
                                DcmPolymorphOBOW& uncompressedPixelData,
                                const DcmCodecParameter* cp,
                                const DcmStack& objStack ) const;

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
                                    void* buffer,
                                    Uint32 bufSize,
                                    Uint16 imageSamplesPerPixel,
                                    Uint16 bytesPerSample );

};


class DJ2KImageCompressionLosslessDecoder : public DJ2KDecoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};


class DJ2KImageCompressionDecoder : public DJ2KDecoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};



#endif
