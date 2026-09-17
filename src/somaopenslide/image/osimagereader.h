#ifndef SOMAIO_IMAGE_OSIMAGEREADER_H
#define SOMAIO_IMAGE_OSIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/config/os_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;
  
  /// ImageReader for virtual microscopy images using OpenSlide
  template<typename T>
  class OSImageReader : public ImageReader<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      OSImageReader();
      virtual ~OSImageReader();
      
      //======================================================================
      //   I M A G E R E A D E R
      //======================================================================
      virtual void read( T * dest, DataSourceInfo & dsi,
                         std::vector<int> & pos,
                         std::vector<int> & size,
                         std::vector<long> & stride,
                         carto::Object options = carto::none() );
      
      virtual void updateParams( DataSourceInfo & dsi );
      virtual void resetParams();
      virtual ImageReader<T>* cloneReader() const;
      virtual std::string formatID() const { return "OpenSlide"; }

    protected:
      void swapVoxels( T* buffer, int64_t size, const bool & byteswap );
      std::vector<std::vector<int> >  _sizes;
      openslide_t * _osimage;
  };
  
}

#endif
