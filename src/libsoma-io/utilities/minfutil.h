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

#ifndef SOMAIO_UTILITIES_MINFUTIL_H
#define SOMAIO_UTILITIES_MINFUTIL_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{

  /// Meta informations utilities
  ///
  class MinfUtil
  {
  public:
    virtual ~MinfUtil() {}

    /// \c header is filtered by checking existing properties.
    /// Properties 'uuid' and 'resolutions_dimension' are removed
    /// unless options specify to keep them. \n
    /// \n
    /// Available options: \n
    /// "remove_uuid", if set to false, "uuid" property is kept (default is true) \n
    /// "remove_resolutions_dimension", if set to false, "resolutions_dimension" 
    ///                             property is kept (default is true) 
    static void filter(carto::Object header,
                       carto::Object options = carto::none());
    

    /// \c header is updated from a data source.
    /// Properties 'uuid' is updated unless options specify to skip it. \n
    /// \n
    /// Available options: \n
    /// "update_uuid", if set to false, "uuid" property update from \c source is skipped (default is true) \n
    static void updateFromSource(soma::DataSource * source,
                                 carto::Object header,
                                 carto::Object options = carto::none());
  };

}

#endif

