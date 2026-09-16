//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/stream/fostreambuf.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>

using namespace carto;
using namespace std;


//=============================================================================
//	M E T H O D S
//=============================================================================

template <class Inserter>
fostreambuf<Inserter>::fostreambuf(streambuf* sink,
	const Inserter& inserter, bool d)
	: _sink(sink), _inserter(inserter), _delete(d)
{
}


template <class Inserter>
fostreambuf<Inserter>::fostreambuf(streambuf* sink,
	bool d)
	: _sink(sink), _delete(d)
{
}


template <class Inserter>
fostreambuf<Inserter>::~fostreambuf()
{
	if (_delete)
		delete _sink;
}


template <class Inserter>
int
fostreambuf<Inserter>::overflow(int c)
{
	int result = EOF;

	if (c == EOF)
		result = sync();
	else if (_sink != 0)
	{
		assert(c >= 0 && c <= UCHAR_MAX) ;
		result = _inserter(_sink , c);
	}
	return result;
}


template <class Inserter>
int
fostreambuf<Inserter>::sync()
{
	return _sink == 0 ? EOF : _sink->pubsync();
}


template <class Inserter>
streambuf*
fostreambuf<Inserter>::setbuf(char* p , int len)
{
	return _sink == 0 ? 0 : _sink->pubsetbuf(p, len);
}
