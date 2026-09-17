//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/object/swriter.h>
#include <cartobase/object/lexicon.h>
#include <cartobase/exception/file.h>
#include <cartobase/exception/parse.h>

using namespace carto;
using namespace std;

//=============================================================================
//	M E T H O D S
//=============================================================================

SyntaxWriter::SyntaxWriter(const string& filename)
	: _filename(filename), _stream(filename.c_str())
{
	if (!_stream)
		throw file_error(_filename);
}


SyntaxWriter::~SyntaxWriter()
{
}


void
SyntaxWriter::write(const SyntaxSet& rules)
{
	_stream << "# syntax 1.0" << endl;
	for (SyntaxSet::const_iterator r = rules.begin(); r != rules.end(); ++r)
	{
		_stream << '\n'
		        << Lexicon::begin() << ' '
		        << Lexicon::syntax() << ' '
		        << r->first << endl;

		// separate mandatory and optional attributes
		SemanticSet set1;
		SemanticSet set2;
	  	string::size_type tab1 = 0;
	  	string::size_type tab2 = 0;
		for (SemanticSet::const_iterator s = r->second.begin(); s !=r->second.end(); ++s)
		{
			if (s->second.needed)
				set1[s->first] = s->second;
			else
				set2[s->first] = s->second;
			if (s->first.size() > tab1)
				tab1 = s->first.size();
			if (s->second.type.size() > tab2)
				tab2 = s->second.type.size();
		}

		// write mandatory then optional attributes
		write(set1, tab1, tab2);
		write(set2, tab1, tab2);

		_stream << Lexicon::end() << endl;
	}
}


void
SyntaxWriter::write(const SemanticSet& set,
	string::size_type tab1, string::size_type tab2)
{
	for (SemanticSet::const_iterator s = set.begin(); s != set.end(); ++s)
	{
		_stream << s->first;
		for (string::size_type n = 0; n <= tab1 - s->first.size(); ++n)
			_stream << ' ';
		_stream << s->second.type;
		if (s->second.needed)
		{
			for (string::size_type n = 0; n <= tab2 - s->second.type.size(); ++n)
				_stream << ' ';
			_stream << '!';
		}
		_stream << endl;
	}
}


//=============================================================================
//	F U N C T I O N
//=============================================================================

SyntaxWriter&
operator<<(SyntaxWriter& writer, const SyntaxSet& rules)
{
	writer.write(rules);
	return writer;
}
