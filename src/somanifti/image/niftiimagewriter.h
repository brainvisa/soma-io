#ifndef SOMAIO_IMAGE_NIFTIIMAGEWRITER_H
#define SOMAIO_IMAGE_NIFTIIMAGEWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>
#include <soma-io/checker/niftistructwrapper.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;
  class NiftiApiHelpers;

  /** Abstract ImageWriter for NIFTI files.

      It is subclassed in Nifti1ImageWriter and Nifti2ImageWriter for complete
      implementations.

      Supported specific write options (in addition to generic ones such as
      "partial_writing"):

      * "nifti_output_4d_volumes" (int 0/1): allow or forbid 4D images. If not
        allowed, a series of 3D images (with numbers) will be written instead.
      * "force_disk_data_type "(int 0/1): bypass the scale factor/offset
        guessing which tries to write signed 16 bit values + scaling.
      * "override_extension" (int 0/1): if set the standard nifti extensions
        will not be appended if the specified file name is non-standard (not
        ending with .nii, .nii.gz, .img, .img.gz, .hdr, .hdr.gz)
      * "compressed" (int 0/1): if set, and if override_extension is also set,
        use compression in the output stream. It is normally guessed by the
        file extension, thus only used with non-standard extensions.
  */
  template<typename T>
  class NiftiImageWriter : public ImageWriter<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      NiftiImageWriter();
      virtual ~NiftiImageWriter() = 0;

      //======================================================================
      //   I M A G E W R I T E R
      //======================================================================
      virtual void write( const T * source, DataSourceInfo & dsi,
        const std::vector<int> & pos,  /* size 4 : x,y,z,t */
        const std::vector<int> & size, /* size 4 : x,y,z,t */
        const std::vector<long> & strides, /* size 4 : x,y,z,t */
        carto::Object options = carto::none() );

      virtual DataSourceInfo writeHeader( DataSourceInfo & dsi, 
                                          const T * source,
                                          const std::vector<int> & pos,
                                          const std::vector<int> & size,
                                          const std::vector<long> & strides,
                                          carto::Object options
                                            = carto::none() );

      virtual void updateParams( DataSourceInfo & dsi );
      virtual void resetParams();

    protected:
      void buildDSList( DataSourceList & dsl, carto::Object options,
                        bool write4d, int dimt, carto::Object header ) const;
      void fillNiftiHeader( DataSourceInfo & dsi, carto::Object options,
                            bool allow4d );
      void checkDiskDataType( carto::Object header,
                              const T * source,
                              const std::vector<long> & strides,
                              const std::vector<int> & size,
                              carto::Object options ) const;
      void _writeDiffusionVectors( DataSource* bvalfile, DataSource* bvecfile,
                                   carto::Object header );
      void setApi( NiftiApiHelpers *napi ) { api = napi; }
      znzFile writeNiftiHeader( carto::Object options ) const;

      virtual std::string formatName() const = 0;
      virtual bool checkDimsCompatibility() const = 0;
      virtual bool canWrite4d( carto::Object hdr ) const = 0;

      std::vector<std::vector<int> >  _sizes;
      carto::rc_ptr<NiftiStructWrapper> _nim;
      std::vector<carto::rc_ptr<NiftiFileWrapper> > _znzfiles;

      NiftiApiHelpers *api;
  };


  /// ImageWriter for NIFTI-1 files.
  template<typename T>
  class Nifti1ImageWriter : public NiftiImageWriter<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      Nifti1ImageWriter();
      virtual ~Nifti1ImageWriter();
      virtual std::string formatName() const { return "NIFTI-1"; }
      virtual bool checkDimsCompatibility() const;
      virtual bool canWrite4d( carto::Object hdr ) const;
  };


  /// ImageWriter for NIFTI-2 files.
  template<typename T>
  class Nifti2ImageWriter : public NiftiImageWriter<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      Nifti2ImageWriter();
      virtual ~Nifti2ImageWriter();
      virtual std::string formatName() const { return "NIFTI-2"; }
      virtual bool checkDimsCompatibility() const { return true; }
      virtual bool canWrite4d( carto::Object /* hdr */ ) const { return true; }
  };

}

#endif
