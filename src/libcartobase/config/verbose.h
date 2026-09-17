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

