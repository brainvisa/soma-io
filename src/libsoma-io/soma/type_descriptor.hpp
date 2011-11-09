#ifndef SOMA_TYPE_DESCRIPTOR
#define SOMA_TYPE_DESCRIPTOR

#include <string>

namespace soma {

enum TypeCode
{
  NotAType,
  UInt8, SInt8, UInt16, SInt16, UInt32, SInt32, UInt64, SInt64,
  Float, Double,
  CFloat, CDouble,
  RGB, RGBA,
  Vector,
  Image, 
  NumberOfTypes
};

extern const std::string type_names[ NumberOfTypes ];

class TypeDescriptor
{
public:
  TypeDescriptor( TypeCode, TypeDescriptor * = NULL );
  ~TypeDescriptor();
  
  std::string name() const;
  
  inline TypeCode type_code() const { return _type_code; }
  inline const TypeDescriptor *subtype() const { return _subtype; }
  
private:
  TypeCode _type_code;
  TypeDescriptor *_subtype;
  
};


} // namespace soma

#endif // SOMA_TYPE_DESCRIPTOR
