#ifndef _DcmtkJpeg2000_djrparam_h_
#define _DcmtkJpeg2000_djrparam_h_


#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcpixel.h>


class DJ2KRepresentationParameter: public DcmRepresentationParameter
{

  public:

    DJ2KRepresentationParameter( Uint16 quality,
                                 OFBool losslessProcess );
    DJ2KRepresentationParameter( const DJ2KRepresentationParameter& arg );

    virtual ~DJ2KRepresentationParameter();

    virtual DcmRepresentationParameter* clone() const;
    virtual const char* className() const;
    virtual OFBool operator==( const DcmRepresentationParameter& arg ) const;

    Uint16 getQuality() const;
    OFBool useLosslessProcess() const;

  private:

    Uint16 quality_;
    OFBool losslessProcess_;

};


#endif
