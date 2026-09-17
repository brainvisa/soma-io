#ifndef CARTOBASE_OBJECT_SPRINTER_H
#define CARTOBASE_OBJECT_SPRINTER_H

//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#ifndef CARTOBASE_OBJECT_SYNTAX_H
#include <cartobase/object/syntax.h>
#endif
#include <iostream>

//===========================================================================
//	F U N C T I O N
//===========================================================================

/**	For debugging: print a SyntaxSet on a stream
	@param  os stream to print on
	@param  rules SyntaxSet to print
	@return the stream passed as a parameter
*/
CARTOBASE_API std::ostream& operator << ( std::ostream& os, 
					  const carto::SyntaxSet& rules);


#endif
