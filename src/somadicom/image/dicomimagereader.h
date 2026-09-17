#ifndef SOMAIO_IMAGE_DICOMIMAGEREADER_H
#define SOMAIO_IMAGE_DICOMIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
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

  /// ImageReader for DICOM files.
  template<typename T>
  class DicomImageReader : public ImageReader<T>
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      DicomImageReader();
      virtual ~DicomImageReader();

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
      virtual std::string formatID() const { return "DICOM"; }

    protected:
      std::vector<std::vector<int> > _sizes;
  };

}

#endif
