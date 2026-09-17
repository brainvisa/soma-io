#ifndef SOMAIO_IMAGE_DICOMIMAGEWRITER_H
#define SOMAIO_IMAGE_DICOMIMAGEWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// abstract ImageWriter for DICOM files.
  template<typename T>
  class DicomImageWriter : public ImageWriter<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      DicomImageWriter();
      virtual ~DicomImageWriter();

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
      void buildDSList( DataSourceList & dsl ) const;

      std::vector<std::vector<int> >  _sizes;
      std::vector<float> _voxel_size;
  };

}

#endif
