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

#ifndef SOMAIO_DATASOURCEINFO_DATASOURCEINFO_H
#define SOMAIO_DATASOURCEINFO_DATASOURCEINFO_H
//--- somaio -----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourcecapabilities.h>           // member
#include <soma-io/datasource/datasourcelist.h>                       // member
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // member
#include <cartobase/smart/rcptr.h>                                   // member
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;

  /// Informative object used by IO system
  ///
  /// This object is used by FormatChecker, FormatReader or FormatWriter to
  /// describe a DataSource. It contains a DataSourceList which contains
  /// at first a single default DataSource, a Object header and a
  /// DataSourceCapabilities.
  ///  - The list is built by a FormatChecker and contains all the files
  ///    involved in the reading/writing process (header, data, ...)
  ///  - The header is built by a FormatChecker and contains meta information.
  ///  - The DSC contains properties dependant of the format,
  ///    the specific file, the reading process (partial reading), etc.
  /// It is possible to fix some or all of these three objects so that
  /// they are not recomputed by the FormatChecker.
  ///
  /// \see DataSourceInfoLoader DataSourceList DataSourceCapabilities
  class DataSourceInfo
  {
    public:
      /// Constructor from a header [+ capabilities + dslist]
      DataSourceInfo( const carto::Object & header = carto::none(), 
                      const DataSourceCapabilities & 
                        cap = DataSourceCapabilities(),
                      const DataSourceList & 
                        dsl = DataSourceList());
      /// Constructor from a datasource [+ dimensions (to build a header)].
      /// DSList is set as containing only ds pointed by "default" key.
      /// Capabilities are set uninitialized.
      /// If dim is given, a header is built with keys size[X,Y,Z,T].
      /// Else the header is none().
      DataSourceInfo( const carto::rc_ptr<DataSource> & ds, 
                      const std::vector<int> & dim = std::vector<int>() );
      /// Constructor from a datasource + header. It is useful with
      /// ReaderAlgorithm.
      DataSourceInfo( const carto::rc_ptr<DataSource> & ds, 
                      carto::Object header );
      /// Constructor from a filename.
      /// Eases the checking in high end classes
      DataSourceInfo( const std::string & fname );
      // Constructor from a buffer
      //DataSourceInfo( char* buffer );
      /// Constructor by copy
      DataSourceInfo( const DataSourceInfo & );
      virtual ~DataSourceInfo();

      const carto::Object & header() const { return _header; }
            carto::Object & header()       { return _header; }
      const DataSourceCapabilities & capabilities() const
        { return _capabilities; }
            DataSourceCapabilities & capabilities()
        { return _capabilities; }
      /** privateIOData is a generic object used for specific formats
          readers/writers to store data which may be useful for them.
          Typically, reading the header (by a FormatChecker) can allocate
          data structures for underlying IO libraries. Such structure has to
          be available in FormatReader and FormatWriter operations. Thus,
          the FormatChecker can store such data in this object.
      */
      const carto::Object privateIOData() const { return _privateiodata; }
            carto::Object privateIOData()       { return _privateiodata; }
      const DataSourceList & list() const { return _datasourcelist; }
            DataSourceList & list()       { return _datasourcelist; }
      std::string url() const { return _datasourcelist.dataSource()->url(); }

    protected:
      carto::Object           _header;
      DataSourceCapabilities  _capabilities;
      DataSourceList          _datasourcelist;
      carto::Object           _privateiodata;
  };

}

#endif
