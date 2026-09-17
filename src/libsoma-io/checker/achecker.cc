//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/achecker.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/syntobject.h>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;


//============================================================================
//	M E T H O D S
//============================================================================

AttributedChecker::AttributedChecker(const SyntaxSet& syntax)
  : _syntax(syntax)
{
}


AttributedChecker::~AttributedChecker()
{
}


set<string>
AttributedChecker::check(const GenericObject& object) const
{
	set<string> result;

        const SyntaxedInterface	*si 
          = object.getInterface<const SyntaxedInterface>();
        if( !si || !si->hasSyntax() )
	  return result;
	// get the syntactic attribute
	SyntaxSet::const_iterator s = _syntax.find(si->getSyntax());
	if(s == _syntax.end())
	  {
	    result.insert( "<syntax not found>" );
	    return result;
	  }

	// make sure all mandatory semantic attributes are here
	set<string> attributes;
        Object	it = object.objectIterator();
        for( it=object.objectIterator(); it->isValid(); it->next() )
          attributes.insert( it->key() );
	for (SemanticSet::const_iterator i = s->second.begin(); 
	     i != s->second.end(); ++i)
	{
		if (i->second.needed == true 
		    && attributes.find(i->first) == attributes.end())
		{
			result.insert(i->first);
		}
	}

	return result;
}
