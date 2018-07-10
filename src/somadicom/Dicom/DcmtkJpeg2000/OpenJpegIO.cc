#include <soma-io/Dicom/DcmtkJpeg2000/OpenJpegIO.h>

#include <cstring>
#include <cstdlib>


//
//  static functions
//


#if OPENJPEG_VERSION >= 200

typedef struct opj_buffer_info
{

  OPJ_BYTE* buf;
  OPJ_BYTE* cur;
  OPJ_SIZE_T len;

} opj_buffer_info_t;


static OPJ_SIZE_T opj_read_from_buffer( void* pdst, 
                                        OPJ_SIZE_T len,
                                        void* psrcIn )
{

  opj_buffer_info_t* psrc = (opj_buffer_info_t*)psrcIn;
  OPJ_SIZE_T n = psrc->buf + psrc->len - psrc->cur;

  if ( n )
  {

    if ( n > len )
    {

      n = len;

    }

    std::memcpy( pdst, psrc->cur, n );
    psrc->cur += n;

  }
  else
  {

    n = (OPJ_SIZE_T)-1;

  }

  return n;

}


static OPJ_SIZE_T opj_write_to_buffer( void* p_buffer,
                                       OPJ_SIZE_T p_nb_bytes,
                                       void* p_source_bufferIn )
{

  opj_buffer_info_t* p_source_buffer = (opj_buffer_info_t*)p_source_bufferIn;
  OPJ_BYTE* pbuf = p_source_buffer->buf;
  OPJ_BYTE* pcur = p_source_buffer->cur;
  OPJ_SIZE_T len = p_source_buffer->len;

  if ( len == 0 )
  {

    len = 1;

  }

  OPJ_SIZE_T dist = pcur - pbuf, n = len - dist;

  while ( n < p_nb_bytes )
  {

    len *= 2;
    n = len - dist;

  }

  if ( len != p_source_buffer->len )
  {

    pbuf = (OPJ_BYTE*)malloc( len );

    if ( pbuf == 0 )
    {

      return (OPJ_SIZE_T)-1;

    }

    if ( p_source_buffer->buf )
    {

      std::memcpy( pbuf, p_source_buffer->buf, dist );
      free( p_source_buffer->buf );

    }

    p_source_buffer->buf = pbuf;
    p_source_buffer->cur = pbuf + dist;
    p_source_buffer->len = len;

  }

  std::memcpy( p_source_buffer->cur, p_buffer, p_nb_bytes );
  p_source_buffer->cur += p_nb_bytes;

  return p_nb_bytes;

}


static OPJ_OFF_T opj_skip_from_buffer( OPJ_OFF_T len, void* psrcIn )
{

  opj_buffer_info_t* psrc = (opj_buffer_info_t*)psrcIn;
  OPJ_OFF_T n = psrc->buf + psrc->len - psrc->cur;

  if ( n )
  {
  
    if ( n > len )
    {

      n = len;

    }

    psrc->cur += len;

  }
  else
  {

    n = (OPJ_OFF_T)-1;

  }

  return n;

}


static OPJ_BOOL opj_seek_from_buffer( OPJ_OFF_T len, void* psrcIn )
{

  opj_buffer_info_t* psrc = (opj_buffer_info_t*)psrcIn;
  OPJ_SIZE_T n = psrc->len;

  if ( n > len )
  {

    n = len;

  }

  psrc->cur = psrc->buf + n;

  return OPJ_TRUE;

}


opj_stream_t* OPJ_CALLCONV opj_stream_create_buffer_stream( 
                                                        opj_buffer_info_t* psrc,
                                                        OPJ_BOOL input )
{

  if ( !psrc )
  {

    return 0;

  }

  opj_stream_t* ps = opj_stream_default_create( input );

  if ( ps == 0 )
  {

    return 0;

  }

  opj_stream_set_user_data( ps, psrc, 0 );
  opj_stream_set_user_data_length( ps, psrc->len );

  if ( input )
  {

    opj_stream_set_read_function( ps, opj_read_from_buffer );

  }
  else
  {

    opj_stream_set_write_function( ps, opj_write_to_buffer );

  }

  opj_stream_set_skip_function( ps, opj_skip_from_buffer );
  opj_stream_set_seek_function( ps, opj_seek_from_buffer );

  return ps;

}

#endif


//
//  class OpenJpegIO
//


dcm::OpenJpegIO::OpenJpegIO()
#if OPENJPEG_VERSION < 200
               : _format( CODEC_J2K )
#else
               : _format( OPJ_CODEC_J2K )
#endif
{
}


dcm::OpenJpegIO::~OpenJpegIO()
{
}


void* dcm::OpenJpegIO::getImage( uint8_t* buffer, int32_t length )
{

  opj_image_t* image = 0;
  opj_dparameters_t parameters;
  opj_set_default_decoder_parameters( &parameters );
  parameters.decod_format = _format;

#if OPENJPEG_VERSION < 200
  opj_dinfo_t* dinfo = opj_create_decompress( _format );

  if ( dinfo )
  {

    opj_setup_decoder( dinfo, &parameters );

    opj_cio_t* cio = opj_cio_open( (opj_common_ptr)dinfo, buffer, length );

    if ( cio )
    {

      image = opj_decode( dinfo, cio );
      opj_cio_close( cio );

    }

    opj_destroy_decompress( dinfo );

  }
#else
  opj_codec_t* dinfo = opj_create_decompress( _format );

  if ( dinfo )
  {

    if ( !opj_setup_decoder( dinfo, &parameters ) )
    {

      opj_destroy_codec( dinfo );

      return 0;

    }

    opj_buffer_info_t mem;

    mem.buf = (OPJ_UINT8*)buffer;
    mem.cur = (OPJ_UINT8*)buffer;
    mem.len = (OPJ_SIZE_T)length;

    opj_stream_t* cio = opj_stream_create_buffer_stream( &mem, OPJ_TRUE );

    if ( cio )
    {

      if ( opj_read_header( cio, dinfo, &image ) )
      {

        if ( opj_decode( dinfo, cio, image ) )
        {

          if ( !opj_end_decompress( dinfo, cio ) )
          {

            opj_image_destroy( image );
            image = 0;

          }

        }

      }

      opj_stream_destroy( cio );

    }

    opj_destroy_codec( dinfo );

  }
#endif

  return (void*)image;

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

    //if ( jptr )
    {

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

  }
  else if ( numcmpts == 3 )
  {

    uint8_t* ptr = buffer;
    uint32_t* jptr0 = (uint32_t*)image->comps[ 0 ].data;
    uint32_t* jptr1 = (uint32_t*)image->comps[ 1 ].data;
    uint32_t* jptr2 = (uint32_t*)image->comps[ 2 ].data;

    if ( jptr0 && jptr1 && jptr2 )
    {

      while ( streamLength-- )
      {

        *ptr++ = (uint8_t)*jptr0++;
        *ptr++ = (uint8_t)*jptr1++;
        *ptr++ = (uint8_t)*jptr2++;

      }

    }

  }
  else
  {

    status = false;

  }

  opj_image_destroy( image );

  return status;

}
