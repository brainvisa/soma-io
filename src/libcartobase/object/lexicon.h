#ifndef CARTOBASE_OBJECT_LEXICON_H
#define CARTOBASE_OBJECT_LEXICON_H

//--- header files ------------------------------------------------------------

#include <string>

namespace carto
{

  //--- class declarations ----------------------------------------------------

  class CARTOBASE_API Lexicon
  {
  public:
    static const std::string begin() { return "*BEGIN"; };
    static const std::string end() { return "*END"; };
    static const std::string syntax() { return "SYNTAX"; };

    static const std::string properties() { return "ATTRIBUTES_TABLE"; };
    static const std::string rules() { return "RULES_TABLE"; };
    static const std::string rule() { return "RULE"; };

    static const std::string graph() { return "GRAPH"; };
    static const std::string vertex() { return "NODE"; };
    static const std::string uedge() { return "UEDGE"; };
    static const std::string dedge() { return "DEDGE"; };
    static const std::string tree() { return "TREE"; }
    static const std::string version() { return "1.0"; };
  };

}

#endif
