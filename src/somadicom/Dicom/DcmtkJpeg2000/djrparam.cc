#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djrparam.h>
#else
#include <Dicom/DcmtkJpeg2000/djrparam.h>
#endif

#include <dcmtk/ofstd/ofstd.h>


DJ2KRepresentationParameter::DJ2KRepresentationParameter( 
                                                        Uint16 quality,
                                                        OFBool losslessProcess )
                           : DcmRepresentationParameter(),
                             quality_( quality ),
                             losslessProcess_( losslessProcess )
{
}


DJ2KRepresentationParameter::DJ2KRepresentationParameter( 
                                        const DJ2KRepresentationParameter& arg )
                           : DcmRepresentationParameter( arg ),
                             quality_( arg.quality_ ),
                             losslessProcess_( arg.losslessProcess_ )
{
}


DJ2KRepresentationParameter::~DJ2KRepresentationParameter()
{
}


DcmRepresentationParameter* DJ2KRepresentationParameter::clone() const
{

  return new DJ2KRepresentationParameter( *this );

}


const char* DJ2KRepresentationParameter::className() const
{

  return "DJ2KRepresentationParameter";

}


OFBool DJ2KRepresentationParameter::operator==( 
                                   const DcmRepresentationParameter& arg ) const
{

  const char *argname = arg.className();

  if ( argname )
  {

    OFString argstring( argname );

    if ( argstring == className() )
    {

      const DJ2KRepresentationParameter& argll = 
                  OFreinterpret_cast( const DJ2KRepresentationParameter&, arg );

      if ( losslessProcess_ && argll.losslessProcess_ )
      {

        return OFTrue;

      }
      else if ( losslessProcess_ != argll.losslessProcess_ )
      {

        return OFFalse;

      }
      else if ( quality_ != argll.quality_)
      {

        return OFFalse;

      }

      return OFTrue;

    }	

  }

  return OFFalse;

}


Uint16 DJ2KRepresentationParameter::getQuality() const
{

  return quality_;

}


OFBool DJ2KRepresentationParameter::useLosslessProcess() const
{

  return losslessProcess_;

}
