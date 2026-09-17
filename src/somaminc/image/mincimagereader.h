#ifndef SOMAIO_IMAGE_MINCIMAGEREADER_H
#define SOMAIO_IMAGE_MINCIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/config/somaminc_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasource/chaindatasource.h>                    // heritage
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// ImageReader for GIS files.
  template<typename T>
  class MincImageReader : public ImageReader<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      MincImageReader();
      virtual ~MincImageReader();

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
      virtual std::string formatID() const { return "MINC"; }

    protected:
      void readMinc1( T * dest, DataSourceInfo & dsi,
                      std::vector<int> & pos,
                      std::vector<int> & size,
                      std::vector<long> & stride,
                      carto::Object options );
      void readMinc1Whole( T * dest, DataSourceInfo & dsi,
                           std::vector<int> & pos,
                           std::vector<int> & size,
                           std::vector<long> & stride,
                           carto::Object options );
      void readMinc2( T * dest, DataSourceInfo & dsi,
                      std::vector<int> & pos,
                      std::vector<int> & size,
                      std::vector<long> & stride,
                      carto::Object options );

      std::vector<std::vector<int> >  _sizes;
  };


#ifdef MINC_MGH_SUPPORT
  template <typename T>
  class FreeSurferMincImageReader : public MincImageReader<T>
  {
  public:
      FreeSurferMincImageReader();
      virtual ~FreeSurferMincImageReader();
      virtual ImageReader<T>* cloneReader() const;
      virtual std::string formatID() const { return "FREESURFER-MINC"; }
  };
#endif

}

#endif
