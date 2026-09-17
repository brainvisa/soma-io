#ifndef SOMAIO_IMAGE_MIFIMAGEREADER_H
#define SOMAIO_IMAGE_MIFIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>
#include <soma-io/datasource/chaindatasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// ImageReader for MIF files.
  template<typename T>
  class MifImageReader : public ImageReader<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      MifImageReader( );
      virtual ~MifImageReader();

      //======================================================================
      //   I M A G E R E A D E R
      //======================================================================
      void read( T * dest, DataSourceInfo & dsi,
                 std::vector<int> & pos,
                 std::vector<int> & size,
                 std::vector<long> & stride,
                 carto::Object options = carto::none() ) override;

      void updateParams( DataSourceInfo & dsi ) override;
      void resetParams() override;
      ImageReader<T>* cloneReader() const override;
      std::string formatID() const { return "MIF"; }

    protected:
      template <typename U>
      void readType( T * dest, DataSourceInfo & dsi,
                     std::vector<int> & pos,
                     std::vector<int> & size,
                     std::vector<long> & stride,
                     carto::Object options = carto::none() );
      void readBitwise( uint8_t * dest, DataSourceInfo & dsi,
                        std::vector<int> & pos,
                        std::vector<int> & size,
                        std::vector<long> & stride,
                        carto::Object options = carto::none() );
      std::vector<int>  _dims ;
      std::vector<int> _data_layout;
      size_t _data_offset;
      bool _byteswap;
      bool _scaling;
      float _scale_factor;
      float _scale_offset;
  };

}

#endif
