//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/stream/uncomment.h>
#include <stdio.h>

using namespace carto;
using namespace std;

//=============================================================================
//	M E T H O D S
//=============================================================================

UncommentExtractor::UncommentExtractor(char c)
	: _comment(c)
{
}


int
UncommentExtractor::operator()(streambuf& source)
{
	int c = source.sbumpc();
	if (c == _comment)
		while (c != '\n' && c != EOF)
			c = source.sbumpc();
	return c;
}
