#ifndef _DcmtkJpeg2000_dj2kcode_h_
#define _DcmtkJpeg2000_dj2kcode_h_


#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dccodec.h>
#include <dcmtk/dcmdata/dcofsetl.h>
#include <dcmtk/dcmimgle/diutils.h>
#include <dcmtk/ofstd/ofstring.h>


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

    EP_Interpretation getPhotometricInterpretation( DcmItem *item );

    virtual E_TransferSyntax supportedTransferSyntax() const = 0;

};


class DJ2KImageCompressionLosslessEncoder : public DJ2KEncoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};

class DJ2KImageCompressionEncoder : public DJ2KEncoderBase
{

  public:

    virtual E_TransferSyntax supportedTransferSyntax() const;

};


#endif
