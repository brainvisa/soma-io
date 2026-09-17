#include <soma-io/config/osversion.h>

#if (OPENSLIDE_HEX_VERSION < 0x30300)
	// This is a fix to enable use of openslide C external functions
	extern "C" { 
		#include <openslide.h>
	}

  #define openslide_get_level_count openslide_get_layer_count
  #define openslide_get_level_dimensions openslide_get_layer_dimensions
  
#else
	#include <openslide.h>
#endif
