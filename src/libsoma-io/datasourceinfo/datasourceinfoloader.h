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
//--- cartobase -----------------------------------------------------------------
#include <cartobase/object/syntax.h>                                // SyntaxSet
#include <cartobase/object/object.h>                          // usage of none()
//--- system -------------------------------------------------------------------
#include <vector>
#include <string>
//------------------------------------------------------------------------------

namespace soma
{
  class FormatChecker;
  class DataSource;
  class DataSourceInfo;
  
  /// \todo doc
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
      /// \param dsi DataSourceInfo containing header, DSlist and capabilities.
      /// It allows us to have none, some or all information already computed.
      /// It is for the DSIloader to deal with the *all* case, and for the
      /// FormatCheckers to deal with *some* and *none* cases.
      /// \return a DataSourceInfo object containing a header, a list of 
      /// DataSource and a list of capabilities.
      /// \see DataSourceInfo DataSourceList DataSourceCapabilities
      DataSourceInfo check( DataSourceInfo dsi );
      State state() const;
      
      std::string errorMessage() const;
      void launchException();
      
      /// \brief Called inter alia by format-specific FormatChecker to complete
      /// header informations
      /// \param ds DataSource to minf file
      /// \param base If given, the function keeps information from the base
      /// header and adds that from the minf file.
      /// \remarks Should it be let there ?
      //static carto::Object readMinf( DataSource & ds, 
      //                               carto::Object base = carto::none() );
      static carto::SyntaxSet & minfSyntax();
      
    protected:
      struct Private;
      Private *d;
  };
  
}

#endif

