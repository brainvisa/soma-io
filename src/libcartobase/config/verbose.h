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

#ifndef CARTOBASE_CONFIG_MESSAGES_H
#define CARTOBASE_CONFIG_MESSAGES_H

#include <cartobase/config/cartobase_config.h>

#ifdef CARTO_DEBUG
  #include <cartobase/type/string_conversion.h>
  #include <iostream>
#endif

namespace carto {

extern int verbose;

extern int debugMessageLevel;

} // namespace carto


#define cartoDbgMsg( level, message ) if ( (level) <= ::carto::debugMessageLevel ) ::std::cerr << "!! " << (message) << ::std::endl;

#define cartoDbgMsgVerb( level, message ) if ( (level) <= ::carto::debugMessageLevel ) ::std::cerr << "!! " << __FILE__ << " (" << __LINE__ << ")" << ::std::endl << "  " << (message) << ::std::endl;

#define catroDbgVar( level, variable ) if ( (level) <= ::carto::debugMessageLevel ) ::std::cerr << #variable " = " << toString( variable ) << :std::endl;

#define cartoDbgVarVerb( level, variable ) if ( (level) <= ::carto::debugMessageLevel ) ::std::cerr << "!! " << __FILE__ << " (" << __LINE__ << ")" << ::std::endl << "  " #variable " = " << toString( variable ) << ::std::endl;

#define cartoMsg( level, message, space ) if( (level) <= ::carto::debugMessageLevel ) ::std::cout << (space) << ":: " << (message) << ::std::endl;

/* conditional debug output: only active if CARTO_DEBUG is defined */

#ifdef CARTO_DEBUG

#define cartoCondDbgMsg( level, message ) cartoDbgMsg( level, message )

#define cartoCondDbgMsgVerb( level, message ) cartoDbgMsgVerb( level, message )

#define cartoCondDbgVar( level, variable ) cartoDbgVar( level, variable )

#define cartoCondDbgVarVerb( level, variable ) cartoDbgVarVerb( level, variable )

#define cartoCondMsg( level, message, space ) cartoMsg( level, message, space )

#else

#define cartoCondDbgMsg( level, message )

#define cartoCondDbgMsgVerb( level, message )

#define cartoCondDbgVar( level, message )

#define cartoCondDbgVarVerb( level, message )

#define cartoCondMsg( level, message, space )

#endif

#endif

