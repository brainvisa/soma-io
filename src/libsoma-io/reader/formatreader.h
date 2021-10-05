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

#ifndef SOMAIO_READER_FORMATREADER_H
#define SOMAIO_READER_FORMATREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

namespace carto {
  template<typename T> class rc_ptr;
  class Object;
}

namespace soma
{
  class DataSource;
  class DataSourceInfo;
  class AllocatorContext;
  
  /// Low-level object IO reader specialized for a specific format
  ///
  /// To invoque such a format-specific reader, information about the source 
  /// is supposed to be already read, this is to say: the object header has 
  /// already been read (using DataSourceInfo).
  ///
  /// This is a low-level format reader, which is not intended to be used 
  /// alone: Reader will use it to achieve appropriate decoding, allocation,
  /// and reading.
  ///
  /// The job of a reader can be decomposed into several steps:
  /// - (the header has already been read by a FormatChecker)
  /// - determination of the correct allocator, matching with the stream
  ///   decoder characteristics: this means creating an AllocatorContext bound
  ///   to the specific DataSource decoder.
  /// - allocation or setup (reallocation, resizing) of the object to be read, 
  ///   according to reading options (partial reading of an already allocated 
  ///   object may not require a reallocation...). This is performed using a 
  ///   Creator.
  /// - reading of the data, or of a part of it, which can be done on demand, 
  ///   directly via the DataSource once it is setup and plugged
  ///
  /// \todo
  /// Needs to be added:
  /// - options description (mandatory / optional / unrecognized)
  /// - generic options to allocate a new object corresponding to the
  ///   datasource contents (allocate a volume with adhoc dimensions, with 
  ///   given borders...), done by Creator
  template<typename T>
  class FormatReader
  {
  public:
    virtual ~FormatReader();

    //========================================================================
    //   N E W   M E T H O D S
    //========================================================================
    /// Full reading procedure, for an already existing object
    virtual void setupAndRead( T & obj, carto::rc_ptr<DataSourceInfo> dsi,
                               const AllocatorContext & context,
                               carto::Object options );

    /// \brief Factory mode: creates an object and reads it.
    /// The returned object belongs to the calling layer and may be deleted by 
    /// the standard \c delete
    virtual T* createAndRead( carto::rc_ptr<DataSourceInfo> dsi,
                              const AllocatorContext & context,
                              carto::Object options );

    /// \brief Reads part or all of the object \c obj.
    /// This read() method should be called after everything is setup and bound
    /// For objects supporting allocators, \c context should be the same as 
    /// the context bound to the object, and reading is just a matter of 
    /// calling the read methods of the DataSource bound to the context.
    /// \param obj     object to read into
    /// \param dsi     DataSourceInfo containing pre-computed header, data
    ///                source list and capabilities.
    /// \param options may specify a sub-object for partial reading
    virtual void read( T & obj, 
                       carto::rc_ptr<DataSourceInfo> dsi, 
                       const AllocatorContext & context, 
                       carto::Object options );

    /// Duplicate the FormatReader.
    /// Some FormatReader can contain data describing the data source or
    /// storage buffer, so must be cloned for each read.
    /// Note that, for now, the cloned instance does not need to have the same
    /// bound internal data, since it is cloned precisely to work on a
    /// different source/buffer. It just needs to be another instance of the
    /// same exact type.
    virtual FormatReader<T>* clone() const = 0;

    /// Format identifier (abstract).
    /// Used to match checker and reader
    virtual std::string formatID() const = 0;

    /// Checks that dsi has been identified with the matching format checker.
    /// If not, instantiate a new DataSourceInfo and return it.
    /// If yes, the returned DataSourceInfo will be the input one.
    /// This method is called automatically from createAndRead() or
    /// setupAndRead().
    virtual carto::rc_ptr<DataSourceInfo> checkDataSourceInfo(
      carto::rc_ptr<DataSourceInfo> dsi, carto::Object options );

    //========================================================================
    //   S T I L L   U S E D
    //========================================================================
    /// create the object to be read, bind the allocation context.
    /// Either create() or setup() will be called, depending if we are 
    /// working on an existing object or have to allocate a new one.
    /// The default implementations just calls Creator<T>::create().
    virtual T* create( carto::Object header, 
                       const AllocatorContext & context, 
                       carto::Object options );
    /// setup an existing object (for resizing or reallocation for instance).
    /// Either create() or setup() will be called, depending if we are 
    /// working on an existing object or have to allocate a new one.
    /// The default implementations just calls Creator<T>::create().
    virtual void setup( T & obj, carto::Object header, 
                        const AllocatorContext & context, 
                        carto::Object options );

  };

  extern template class FormatReader<carto::GenericObject>;
  extern template class FormatReader<carto::Object>;
}

#endif

