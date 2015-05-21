#ifndef _DcmtkJpeg2000_djcparam_h_
#define _DcmtkJpeg2000_djcparam_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/dj2kutil.h>
#else
#include <Dicom/DcmtkJpeg2000/dj2kutil.h>
#endif

#include <dcmtk/dcmdata/dccodec.h>


class DJ2KCodecParameter : public DcmCodecParameter
{

  public:

    DJ2KCodecParameter( Uint32 fragmentSize = 0,
                        OFBool createOffsetTable = OFTrue,
                        J2K_UIDCreation uidCreation = EJ2KUC_default,
                        OFBool convertToSC = OFFalse );

    DJ2KCodecParameter( const DJ2KCodecParameter& arg );

    virtual ~DJ2KCodecParameter();

    virtual DcmCodecParameter* clone() const;

    virtual const char* className() const;

    Uint32 getFragmentSize() const;
    OFBool getCreateOffsetTable() const;
    J2K_UIDCreation getUIDCreation() const;
    OFBool getConvertToSC() const;

  private:

    DJ2KCodecParameter& operator=( const DJ2KCodecParameter& );

    Uint32 fragmentSize_;
    OFBool createOffsetTable_;
    J2K_UIDCreation uidCreation_;
    OFBool convertToSC_;

};


#endif
