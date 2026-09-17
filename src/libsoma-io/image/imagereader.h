#ifndef SOMAIO_IMAGE_IMAGEREADER_H
#define SOMAIO_IMAGE_IMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>                   // member
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                          // to use none()
//--- system -----------------------------------------------------------------
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{

  /// ImageReader is a low level Image reader.
  ///
  /// ImageReader is a base class for readers of data of type "volume of
  /// voxels" ( 2D or 3D plus a temporal dimension ).\n
  /// Format-specific readers ( GIS, Dicom, ... ) are derived from it. 
  /// They may allow partial reading if implemented, or any other specific
  /// option (multiresolution, ...).
  template<typename T>
  class ImageReader
  {
    public:
      ImageReader();
      virtual ~ImageReader();

      /// Reading a region of a Image/Volume at a given resolution to a 
      /// pre-allocated buffer. Positions are expressed in 4D (x,y,z,t). If  
      /// one or more of these dimensions are of no interest for the format, 
      /// they take the value 0 ( position ) or 1 ( size )
      /// \param dest    Pre-allocated buffer. Its size must be sufficient to
      ///                contain the region to read.
      /// \param pos     Position of the first voxel of the region to read, 
      ///                expressed in the referential of the chosen level
      ///                (if multiresolution enabled).
      /// \param size    Size of the region to read, expressed in the 
      ///                referential of the chosen level (if multiresolution
      ///                enabled).
      /// \param stride  May be used to specialize the reading. Indicates the
      ///                number of cells to skip to read in a given direction.
      /// \param options Communicates info to the reader (for example, the
      ///                chosen level if multiresolution is enabled)
      virtual void read( T * dest, DataSourceInfo & dsi,
                         std::vector<int> & pos,
                         std::vector<int> & size,
                         std::vector<long> & stride,
                         carto::Object options = carto::none() );

      /// Abstract : set specialized ImageReader's parameters.
      /// Since read() may be called several times for a sole volume (for
      /// example if the data is not contiguous on memory), it is useful
      /// to initialize only once file-specific parameters (as the size of the
      /// full volume) or to open only once the file in cases where
      /// opening may be expensive.
      virtual void updateParams( DataSourceInfo & dsi );
      /// Abstract : empty specialized ImageReader's parameters.
      virtual void resetParams();
      virtual ImageReader<T>* cloneReader() const = 0;
      /// Format identifier (abstract).
      /// Used to match checker and reader
      virtual std::string formatID() const = 0;

      virtual bool open( DataSourceInfo & dsi );
      virtual void close( DataSourceInfo & dsi );
      virtual bool isOpen( const DataSourceInfo & dsi ) const;
  };

}

#endif
