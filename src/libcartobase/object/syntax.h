#ifndef CARTOBASE_OBJECT_SYNTAX_H
#define CARTOBASE_OBJECT_SYNTAX_H

//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/config/cartobase_config.h>
#include <string>
#include <map>

namespace carto
{

  //===========================================================================
  //	T Y P E D E F
  //===========================================================================

  /**	Specify properties for semantic attributes - mainly for I/O purposes
   */
  struct CARTOBASE_API Semantic
  {
    inline Semantic();
    inline Semantic( const std::string & t, bool need = false, 
		     bool intern = false );
    /**	type of semantic attribute, such as `\c float'
     */
    std::string type;

    /**	is this semantic attribute mandatory?
     */
    bool needed;
    ///	internal attributes are not written to disk
    bool internal;
  };


  /**	This lookup table associates a semantic attribute with its properties
   */
  typedef std::map<std::string,Semantic> SemanticSet;


  /**	Specify syntax for syntactic attributes
   */
  typedef SemanticSet Syntax;


  /**	This lookup table associates a syntactic attribute with its syntax
   */
  typedef std::map<std::string,Syntax> SyntaxSet;


  inline Semantic::Semantic() 
    : needed( false ), internal( false )
  {
  }


  inline Semantic::Semantic( const std::string & t, bool need, 
			     bool intern )
    : type( t ), needed( need ), internal( intern )
  {
  }

}

#endif
