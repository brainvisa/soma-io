#ifndef CARTOBASE_OBJECT_ATTRIBUTED_H
#define CARTOBASE_OBJECT_ATTRIBUTED_H

#include <cartobase/object/syntobject.h>
#include <cartobase/object/property.h>

namespace carto
{
  // typedef SyntaxedObject<Dictionary> AttributedObject;
  typedef SyntaxedObject<PropertySet> AttributedObject;

  template <> inline std::string DataTypeCode<AttributedObject>::objectType()
  { return "AttributedObject"; }
  template <> inline std::string DataTypeCode<AttributedObject>::dataType()
  { return "VOID"; }
  template <> inline std::string DataTypeCode<AttributedObject>::name()
  { return "AttributedObject"; }

  DECLARE_GENERIC_OBJECT_TYPE( AttributedObject * )
}

#endif
