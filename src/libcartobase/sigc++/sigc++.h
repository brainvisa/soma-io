#ifndef CARTOBASE_SIGC_SIGC_H
#define CARTOBASE_SIGC_SIGC_H

/*! \dir cartobase/sigc++

  Headers from this directory are used to replace all includes from the 
  libsigc++ library. Some macros from Qt library conflict with some functions 
  of libsigc++, so they are dedefined under different names:

  - in sigc++, emit is renamed sigcemit
  - in sigc++, slots is renamed sigcslots

  As a consequence, you must \b NEVER directly include headers from 
  \c <sigc++/...> or you will get into trouble.
*/

/*! \file cartobase/sigc++/sigc++.h

  Headers from this directory are used to replace all includes from the 
  libsigc++ library. Some macros from Qt library conflict with some functions 
  of libsigc++, so they are dedefined under different names:

  - in sigc++, emit is renamed sigcemit
  - in sigc++, slots is renamed sigcslots

  As a consequence, you must \b NEVER directly include headers from 
  \c <sigc++/...> or you will get into trouble.
*/

#include <cartobase/sigc++/cartosigcmacro.h>
#include <sigc++/sigc++.h>
#include <cartobase/sigc++/cartosigcmacroend.h>

#endif

