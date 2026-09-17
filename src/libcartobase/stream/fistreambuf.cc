//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <streambuf>

#include <cartobase/stream/fistreambuf.h>
#include <cartobase/stream/uncomment.h>
#include <cartobase/stream/counter.h>
#include <climits>
#include <cassert>
#include <cstdio>

using namespace std;

namespace carto {

//=============================================================================
//	M E T H O D S
//=============================================================================

template <class Extractor>
fistreambuf<Extractor>::fistreambuf(streambuf* source,
	const Extractor& extractor, bool d)
	: _source(source), _extractor(extractor), _delete(d)
{
}


template <class Extractor>
fistreambuf<Extractor>::fistreambuf(streambuf* source,
	bool d)
	: _source(source), _delete(d)
{
}


template <class Extractor>
fistreambuf<Extractor>::~fistreambuf()
{
	pubsync();
	if (_delete)
		delete _source;
}


template <class Extractor>
int
fistreambuf<Extractor>::underflow()
{
	int result = EOF;
	if (gptr() < egptr())
		result = *gptr();
	else if (_source != 0)
	{
		result = _extractor(*_source);
		if (result != EOF)
		{
			assert(result >= 0 && result <= UCHAR_MAX);
			_buffer = result;
			setg(&_buffer , &_buffer , &_buffer + 1);
		}
	}
	return result;
}


template <class Extractor>
int
fistreambuf<Extractor>::sync()
{
	int result = 0;
	if (_source != 0)
	{
		if (gptr() < egptr())
		{
			result = _source->sputbackc(*gptr());
			setg(0, 0, 0);
		}
		if (_source->pubsync() == EOF)
			result = EOF;
	}
	return result;
}


template <class Extractor>
streambuf*
fistreambuf<Extractor>::setbuf(char* p , std::streamsize len)
{
	return _source == 0 ? 0 : _source->pubsetbuf(p, len);
}


template <class Extractor>
streambuf::pos_type 
fistreambuf<Extractor>::seekoff( off_type type, 
                                 ios_base::seekdir dir, 
                                 ios_base::openmode mode )
{
  return _source->pubseekoff( type, dir, mode );
}

//=============================================================================
//	T E M P L A T E  I N S T A N C I A T I O N
//=============================================================================

template class fistreambuf<UncommentExtractor>;
template class fistreambuf<CounterExtractor>;

} // namespace carto
