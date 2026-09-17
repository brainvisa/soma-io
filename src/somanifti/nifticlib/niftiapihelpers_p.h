#ifndef SOMAIO_NIFTICLIB_NIFTIAPIHELPERS_P_H
#define SOMAIO_NIFTICLIB_NIFTIAPIHELPERS_P_H

#include <soma-io/config/soma_config.h>
#include <nifti2_io.h>

namespace soma
{

  class NiftiApiHelpers
  {
  public:
    virtual ~NiftiApiHelpers() = 0;

    virtual nifti_image *nifti_image_read(
      const char *hname , int read_data ) = 0;
    virtual int is_nifti_file( const char *hname ) = 0;
    virtual znzFile nifti_image_write_hdr_img(
      nifti_image *nim, int write_data, const char* opts ) = 0;
  };


  class Nifti1ApiHelpers : public NiftiApiHelpers
  {
  public:
    virtual ~Nifti1ApiHelpers();

    virtual nifti_image *nifti_image_read(
      const char *hname , int read_data )
    {
      return ::nifti_image_read( hname, read_data );
    }

    virtual inline int is_nifti_file( const char *hname )
    {
      return ::is_nifti_file( hname );
    }

    virtual znzFile nifti_image_write_hdr_img(
      nifti_image *nim, int write_data, const char* opts )
    {
      return ::nifti_image_write_hdr_img( nim, write_data, opts );
    }
  };


  class Nifti2ApiHelpers : public NiftiApiHelpers
  {
  public:
    virtual ~Nifti2ApiHelpers();

    virtual inline nifti_image *nifti_image_read(
      const char *hname , int read_data )
    {
      return ::nifti2_image_read( hname, read_data );
    }

    virtual inline int is_nifti_file( const char *hname )
    {
      return ::is_nifti2_file( hname );
    }

    virtual znzFile nifti_image_write_hdr_img(
      nifti_image *nim, int write_data, const char* opts )
    {
      return ::nifti2_image_write_hdr_img( nim, write_data, opts );
    }
  };

}

#endif

