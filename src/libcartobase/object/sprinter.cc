//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/object/sprinter.h>
#include <cartobase/object/lexicon.h>

using namespace carto;
using namespace std;


//=============================================================================
//	F U N C T I O N
//=============================================================================

ostream&
operator<<(ostream& os, const SyntaxSet& rules)
{
	for (SyntaxSet::const_iterator r = rules.begin(); r != rules.end(); ++r)
	{
		os << '\n' << Lexicon::begin() << ' ' << Lexicon::syntax()
		   << ' ' << r->first << '\n';
		for (SemanticSet::const_iterator s = r->second.begin(); s != r->second.end(); ++s)
		{
			os << s->first << ' ' << s->second.type;
			if (s->second.needed)
			{
				os << " !";
			}
			os << endl;
		}
		os << Lexicon::end() << endl;
	}

	return os;
}
