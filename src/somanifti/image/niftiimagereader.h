#ifndef SOMAIO_IMAGE_NIFTIIMAGEREADER_H
#define SOMAIO_IMAGE_NIFTIIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>
#include <soma-io/datasource/chaindatasource.h>
#include <soma-io/checker/niftistructwrapper.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// ImageReader for NIFTI files.
  template<typename T>
  class NiftiImageReader : public ImageReader<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      /// format_id may be "NIFTI-1" (default) or "NIFTI-2"
      NiftiImageReader( const std::string & format_id = "NIFTI-1" );
      virtual ~NiftiImageReader();

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
      virtual std::string formatID() const { return _format_id; }

    protected:
      template <typename U>
      void readType( T * dest, DataSourceInfo & dsi,
                     std::vector<int> & pos,
                     std::vector<int> & size,
                     std::vector<long> & stride,
                     carto::Object options = carto::none() );
      std::vector<std::vector<int> >  _sizes;
      carto::rc_ptr<NiftiStructWrapper> _nim;
      std::string _format_id;
  };

}

#endif
