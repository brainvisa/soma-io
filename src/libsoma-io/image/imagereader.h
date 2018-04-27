/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

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
      ///                (if multiresolution enbaled).
      /// \param size    Size of the region to read, expressed in the 
      ///                referential of the chosen level (if multiresolution
      ///                enbaled).
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
