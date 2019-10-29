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

//--- soma-io ----------------------------------------------------------------
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/utilities/minfutil.h>
//--- std --------------------------------------------------------------------
#include <exception>                                      // launch exceptions
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "MINFUTIL" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

namespace soma
{
    ///
    /// Filters properties that are not relevant in a minf header
    ///
    void MinfUtil::filter(Object header, 
                            Object options)
    {
        bool removeUuid = true, 
            removeResolutionsDimension = true;
        
        if (!options.isNone()) {
            
            try {
                Object p = options->getProperty("remove_uuid");
                removeUuid = p->getScalar();
                
//                 std::cout << "MinfUtil::filter, option to remove uuid is: " 
//                           << carto::toString(removeUuid) << std::endl;
            }
            catch(exception & e)
            {}        
            
            try {
                Object p = options->getProperty("remove_resolutions_dimension");
                removeResolutionsDimension = p->getScalar();
                
//                 std::cout << "MinfUtil::filter, option to remove resolutions_dimension is: " 
//                           << carto::toString(removeResolutionsDimension) << std::endl;
            }
            catch(exception & e)
            {}
        }

        if (removeUuid && header->hasProperty("uuid")) {
            localMsg("removing uuid property from header");
            header->removeProperty("uuid");
        }
            
        if (removeResolutionsDimension && header->hasProperty("resolutions_dimension")) {
            localMsg("removing resolutions_dimension property from header");
            header->removeProperty("resolutions_dimension");
        }
        
    }

  
    ///
    /// Update some header properties from source
    ///
    void MinfUtil::updateFromSource(DataSource * source,
                                    Object header,
                                    Object options) {
        
        Object reloadedheader = carto::none();
               
        // UUID handling: try first to reload an existing .minf
        // in order to preserve existing uuid 
        bool update_uuid = true;
        try {
            Object u = options->getProperty("update_uuid");
            update_uuid = u->getScalar();
        }
        catch(exception &) {
        }
        
        if (update_uuid) {
            string uuid;
            
            try {
                Object u = header->getProperty("uuid");
                uuid = u->getString();
                localMsg("uuid property found in header");
            }
            catch( exception & ) {
            }
            
            if(uuid.empty())
                try {
                    
                    if(reloadedheader.isNone()) {
                        localMsg("try to reload header from data source");
                        reloadedheader = DataSourceInfoLoader::readMinf(*source);
                    }
                    
                    Object u = reloadedheader->getProperty("uuid");
                    uuid = u->getString();
                    localMsg("uuid property found in reloaded header");
                }
                catch(exception &) {
                }
                
            if(!uuid.empty()) {
                localMsg("updating uuid property in header to " + uuid);
                header->setProperty("uuid", uuid);
            }
        }
    }
}

#undef localMsg
