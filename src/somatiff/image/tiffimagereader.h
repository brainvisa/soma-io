#ifndef SOMAIO_IMAGE_TIFFIMAGEREADER_H
#define SOMAIO_IMAGE_TIFFIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasource/chaindatasource.h>                    // heritage
#include <soma-io/reader/itemreader.h>                      // read + byteswap
#include <soma-io/utilities/multifileformat.h>                     // soma::MultiFileFormatInfo
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// ImageReader for TIFF files.
  template<typename T>
  class TiffImageReader : public ImageReader<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      TiffImageReader();
      virtual ~TiffImageReader();

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
      virtual std::string formatID() const { return "TIFF"; }

    protected:
      template <typename U>
      void readType( T * dest, DataSourceInfo & dsi,
                     std::vector<int> & pos,
                     std::vector<int> & size,
                     std::vector<long> & stride,
                     carto::Object options = carto::none() );

    protected:
      std::vector<std::vector<int> >  _sizes;
      soma::MultiFileFormatInfo _mfi;
  };
}

#endif
