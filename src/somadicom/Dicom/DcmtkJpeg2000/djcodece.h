#ifndef _DcmtkJpeg2000_djcodece_h_
#define _DcmtkJpeg2000_djcodece_h_


#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dccodec.h>
#include <dcmtk/dcmdata/dcofsetl.h>
#include <dcmtk/ofstd/ofstring.h>


class DJ2KRepresentationParameter;
class DJ2KCodecParameter;


class  DJ2KEncoderBase : public DcmCodec
{

  public:

    DJ2KEncoderBase();
    virtual ~DJ2KEncoderBase();

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

    virtual E_TransferSyntax supportedTransferSyntax() const = 0;

    OFCondition losslessRawEncode( const Uint16 *pixelData,
                                   const Uint32 length,
                                   DcmItem *dataset,
                                   const DJ2KRepresentationParameter *djrp,
                                   DcmPixelSequence * & pixSeq,
                                   const DJ2KCodecParameter *djcp,
                                   double& compressionRatio ) const;

    OFCondition updateLossyCompressionRatio( DcmItem *dataset, 
                                             double ratio ) const;

    OFCondition updateDerivationDescription(
                                        DcmItem *dataset,
                                        const DJ2KRepresentationParameter *djrp,
                                        double ratio ) const;

    OFCondition compressRawFrame( const Uint8 *framePointer,
                                  Uint16 bitsAllocated,
                                  Uint16 columns,
                                  Uint16 rows,
                                  Uint16 samplesPerPixel,
                                  Uint16 planarConfiguration,
                                  const OFString& photometricInterpretation,
                                  DcmPixelSequence *pixelSequence,
                                  DcmOffsetList &offsetList,
                                  unsigned long &compressedSize,
                                  const DJ2KCodecParameter *djcp ) const;

};


class DJ2KLosslessEncoder : public DJ2KEncoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};

class DJ2KEncoder : public DJ2KEncoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};


#endif
