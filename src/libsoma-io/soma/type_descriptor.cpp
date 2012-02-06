#include <soma/type_descriptor.hpp>

using namespace std;


namespace soma {

const std::string type_names[] =
{
  "Invalid type",
  "UInt8", "SInt8", "UInt16", "SInt16", "UInt32", "SInt32", "UInt64", "SInt64",
  "Float", "Double",
  "CFloat", "CDouble",
  "RGB", "RGBA",
  "Vector",
  "Struct",
  "Binary",
};


TypeDescriptor::TypeDescriptor( TypeCode type_code, TypeDescriptor *subtype ) : _type_code( type_code ), _subtype( subtype )
{
}


TypeDescriptor::~TypeDescriptor()
{
  if ( _subtype ) delete _subtype;
}


string TypeDescriptor::name() const
{
  if ( _subtype ) {
    return type_names[ _type_code ] + "<" + _subtype->name() + ">";
  } else {
    return type_names[ _type_code ];
  }
}


} // namespace soma
