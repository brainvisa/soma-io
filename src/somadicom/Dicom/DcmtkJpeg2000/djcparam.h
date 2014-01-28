#ifndef _DcmtkJpeg2000_djcparam_h_
#define _DcmtkJpeg2000_djcparam_h_


#include <soma-io/Dicom/DcmtkJpeg2000/dj2kutil.h>

#include <dcmtk/dcmdata/dccodec.h>


class DJ2KCodecParameter : public DcmCodecParameter
{

  public:

    DJ2KCodecParameter( J2K_UIDCreation uidCreation = EJ2KUC_default,
                        OFBool convertToSC = OFFalse );

    /// copy constructor
    DJ2KCodecParameter( const DJ2KCodecParameter& arg );

    /// destructor
    virtual ~DJ2KCodecParameter();

    virtual DcmCodecParameter* clone() const;

    virtual const char* className() const;

    J2K_UIDCreation getUIDCreation() const;
    OFBool getConvertToSC() const;

  private:

    DJ2KCodecParameter& operator=( const DJ2KCodecParameter& );

    /// mode for SOP Instance UID creation (used both for encoding and decoding)
    J2K_UIDCreation uidCreation_;

    /// flag indicating whether image should be converted to Secondary Capture 
    /// upon compression
    OFBool convertToSC_;

};


#endif
