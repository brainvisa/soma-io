#include <soma-io/Dicom/DcmtkJpeg2000/JasperIO.h>


dcm::JasperIO::JasperIO()
{

  _jasInitialized = jas_init() ? false : true;

}


dcm::JasperIO::~JasperIO()
{
}


void* dcm::JasperIO::getImage( uint8_t* buffer, int32_t length )
{

  jas_image_t* image = 0;
  jas_stream_t* streamIn = jas_stream_memopen( (char*)buffer, length );

  if ( streamIn )
  {

    image = jas_image_decode( streamIn, -1, 0 );

    jas_stream_close( streamIn );

    if ( image )
    {

      int32_t numcmpts = jas_image_numcmpts( image );
      int32_t depth = jas_image_cmptprec( image, 0 );

      if ( ( numcmpts >= 3 ) && ( depth <= 8 ) )
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

    }

  }

  return (void*)image;

}


bool dcm::JasperIO::decode( uint8_t* buffer, void* imageIn )
{

  bool status = true;
  jas_image_t* image = (jas_image_t*)imageIn;
  int32_t numcmpts = jas_image_numcmpts( image );

  if ( numcmpts == 1 )
  {

    jas_stream_t* streamOut = image->cmpts_[ 0 ]->stream_;

    jas_stream_rewind( streamOut );

    uint32_t streamLength = jas_stream_length( streamOut );
    uint8_t* ptr = buffer;

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

  }
  else if ( numcmpts == 3 )
  {

    int32_t i;
    jas_stream_t* streamOut[ 3 ];

    for ( i = 0; i < numcmpts; i++ )
    {

      streamOut[ i ] = image->cmpts_[ i ]->stream_;
      jas_stream_rewind( streamOut[ i ] );

    }

    uint32_t streamLength = jas_stream_length( streamOut[ 0 ] );
    uint8_t* ptr = buffer;

    while ( streamLength-- )
    {

      *ptr++ = jas_stream_getc( streamOut[ 0 ] );
      *ptr++ = jas_stream_getc( streamOut[ 1 ] );
      *ptr++ = jas_stream_getc( streamOut[ 2 ] );

    }

  }
  else
  {

    status = false;

  }

  jas_image_destroy( image );

  return status;

}
