#include <soma-io/Dicom/DcmtkJpeg2000/OpenJpegIO.h>


dcm::OpenJpegIO::OpenJpegIO()
               : _format( CODEC_J2K )
{
}


dcm::OpenJpegIO::~OpenJpegIO()
{
}


void* dcm::OpenJpegIO::getImage( uint8_t* buffer, int32_t length )
{

  void* image = 0;
  opj_dparameters_t parameters;
  opj_set_default_decoder_parameters( &parameters );
  parameters.decod_format = _format;
  opj_dinfo_t* dinfo = opj_create_decompress( _format );

  if ( dinfo )
  {

    opj_setup_decoder( dinfo, &parameters );
    opj_cio_t* cio = opj_cio_open( (opj_common_ptr)dinfo, buffer, length );

    if ( cio )
    {

      image = (void*)opj_decode( dinfo, cio );
      opj_cio_close( cio );

    }

    opj_destroy_decompress( dinfo );

  }

  return image;

}


bool dcm::OpenJpegIO::decode( uint8_t* buffer, void* imageIn )
{

  bool status = true;
  opj_image_t* image = (opj_image_t*)imageIn;
  int32_t numcmpts = image->numcomps;
  uint32_t streamLength = image->comps[ 0 ].w * image->comps[ 0 ].h;

  if ( numcmpts == 1 )
  {

    uint32_t* jptr = (uint32_t*)image->comps[ 0 ].data;

    if ( image->comps[ 0 ].prec > 8 )
    {

      uint16_t* ptr = (uint16_t*)buffer;

      while ( streamLength-- )
      {

        *ptr++ = (uint16_t)*jptr++;

      }

    }
    else
    {

      uint8_t* ptr = buffer;

      while ( streamLength-- )
      {

        *ptr++ = (uint8_t)*jptr++;

      }

    }

  }
  else if ( numcmpts == 3 )
  {

    uint8_t* ptr = buffer;
    uint32_t* jptr0 = (uint32_t*)image->comps[ 0 ].data;
    uint32_t* jptr1 = (uint32_t*)image->comps[ 1 ].data;
    uint32_t* jptr2 = (uint32_t*)image->comps[ 2 ].data;

    while ( streamLength-- )
    {

      *ptr++ = (uint8_t)*jptr0++;
      *ptr++ = (uint8_t)*jptr1++;
      *ptr++ = (uint8_t)*jptr2++;

    }

  }
  else
  {

    status = false;

  }

  opj_image_destroy( image );

  return status;

}
