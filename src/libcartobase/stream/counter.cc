//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/stream/counter.h>

using namespace carto;
using namespace std;


//=============================================================================
//	M E T H O D S
//=============================================================================

CounterExtractor::CounterExtractor(int line)
	: _line(line), _newline(false)
{
}


int
CounterExtractor::operator()(streambuf& source)
{
	int c = source.sbumpc();
	if (_newline)
	{
		++_line;
		_newline = false;
	}
	if (c == '\n')
	{
		_newline = true;
	}
	return c;
}


int
CounterExtractor::reset(int line)
{
	int tmp = _line;
	_line = line;
	return tmp;
}
