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

#ifndef SOMAIO_DATASOURCEINFO_DATASOURCEINFOLOADER_H
#define SOMAIO_DATASOURCEINFO_DATASOURCEINFOLOADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/syntax.h>                              // SyntaxSet
#include <cartobase/object/object.h>                        // usage of none()
//--- system -----------------------------------------------------------------
#include <vector>
#include <string>
//----------------------------------------------------------------------------

namespace soma
{
  class FormatChecker;
  class DataSource;
  class DataSourceInfo;
  
  /// Generic information retreiver / checker for all data sources and file
  /// formats
  ///
  /// It replaces carto::DataSourceInfo and aims::Finder.\n
  /// DataSourceInfoLoader provides a plug-in system for new formats and data
  /// types. Formats are hidden in the plugins and should never be accessed
  /// directly.\n
  /// Usage: check() the DataSourceInfo and then process according to the
  /// object type ("Volume of S16", "Mesh", "Texture", "Bucket", ...).\n
  /// Use the generic Reader once the data type is known, the right format 
  /// will be selected automatically.\n
  /// Avoiding manually switching on objects and data types is possible using
  /// the ReaderAlgorithm interface and presumably, template functions.\n
  /// Here is an example of how to use the DataSourceInfoLoader class:
  ///
  /// \code
  /// #include <soma-io/datasourceinfo/datasourceinfo.h>
  /// #include <soma-io/datasourceinfo/datasourceinfoloader.h>
  /// #include <soma-io/io/reader.h>
  /// #include <cartodata/volume/volume.h>
  /// #include <iostream>
  ///
  /// using namespace soma;
  /// using namespace carto;
  /// using namespace std;
  ///
  /// DataSourceInfo info( "toto.ima" );
  /// DataSourceInfoLoader f;
  /// info = f.check( info );
  /// if( !info.header().get() )
  ///   cerr << "could not load " << info.dataSource()->url() << endl;
  /// else
  /// {
  ///   string object_type = info.header()->getProperty( "object_type" )->getString();
  ///   if( object_type == DataTypeCode<Volume<int16_t> >::name() )
  ///   {
  ///       Reader<Volume<int16_t> >  vr( info );
  ///       VolumeRef<int16_t>        vol( vr.read() );
  ///       if( vol )
  ///         cout << "volume read" << endl;
  ///       else
  ///         cerr << "Error loading " << info.dataSource()->url() << endl;
  ///   }
  ///   else if( object_type == DataTypeCode<Mesh>::name() )
  ///   {
  ///       // do your thing
  ///   }
  ///   else
  ///     cerr << info.dataSource()->url() << " is of type " << object_type
  ///          << " which is not handled" << endl;
  /// }
  /// \endcode
  ///
  /// \see DataSourceInfo, Reader, ReaderAlgorithm
  class DataSourceInfoLoader
  {
    public:
      enum State { Unchecked, Ok, Error };
      DataSourceInfoLoader();
      ~DataSourceInfoLoader();

      /// \brief Registers a FormatChecker into the dictionary
      /// \param fmtid       Identification of the format. ex: "GIS"
      /// \param format      Specialized FormatChecker
      /// \param extensions  Extensions readable by the FormatChecker
      static void registerFormat( const std::string & fmtid, 
                                  FormatChecker* format, 
                                  const std::vector<std::string> & 
                                    extensions );
      /// \brief Returns the corresponding FormatChecker
      /// \param format  Identification of the format. ex: "GIS"
      static FormatChecker* formatInfo( const std::string & format );

      /// \brief Finds the right format checker
      /// \param dsi      DataSourceInfo containing header, DSlist and .
      ///                 capabilities. It allows us to have none, some or all
      ///                 information already computed.  It is for the 
      ///                 DSIloader to deal with the *all* case, and for the
      ///                 FormatCheckers to deal with *some* and *none* cases.
      /// \param options  A dictionary containing options. They may not be of
      ///                 any use to the checker, but soma are
      ///                 ( \c resolution_level ).
      /// \return  A DataSourceInfo object containing a header, a list of
      ///          DataSource and a list of Capabilities.
      /// It is possible to specify wich passes to process through
      /// \c passbegin and \c passend.
      /// - pass 1 : extension
      /// - pass 2 : empty extension
      /// - pass 3 : all writers
      /// \see DataSourceInfo DataSourceList DataSourceCapabilities
      DataSourceInfo check( DataSourceInfo dsi, 
                            carto::Object options = carto::none(),
                            int passbegin = 1, int passend = 3 );

      State state() const;
      std::string errorMessage() const;
      void launchException();

      /// \brief Called inter alia by format-specific FormatChecker to 
      /// complete header informations
      /// \param ds DataSource to minf file
      /// \param base If given, the function keeps information from the base
      /// header and adds that from the minf file.
      static carto::Object readMinf( DataSource & ds, 
                                     carto::Object base = carto::none(),
                                     carto::Object options = carto::none() );
      /// Syntax for minf dictionaries
      static carto::SyntaxSet & minfSyntax();

      /// Extensions matching a specified format
      static std::set<std::string> extensions( const std::string & format );

    protected:
      struct Private;
      Private *d;
  };
  
}

#endif
