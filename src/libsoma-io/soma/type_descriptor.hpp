#ifndef SOMA_TYPE_DESCRIPTOR
#define SOMA_TYPE_DESCRIPTOR

#include <string>

namespace soma {

enum TypeCode
{
  invalid_type,
  UInt8, SInt8, UInt16, SInt16, UInt32, SInt32, UInt64, SInt64,
  Float, Double,
  CFloat, CDouble,
  RGB, RGBA,
  Vector,
  Struct,
  Binary, 
  NumberOfTypes
};


typedef unsigned char  UInt8_t;
typedef signed char    SInt8_t;
typedef unsigned short UInt16_t;
typedef signed short   SInt16_t;
typedef unsigned int   UInt32_t;
typedef signed int     SInt32_t;
typedef unsigned long  UInt64_t;
typedef signed long    SInt64_t;


extern const std::string type_names[ NumberOfTypes ];

class TypeDescriptor
{
public:
  TypeDescriptor( TypeCode, TypeDescriptor * = NULL );
  ~TypeDescriptor();
  inline TypeDescriptor( const TypeDescriptor &other );
  TypeDescriptor &operator =( const TypeDescriptor &other );
  
  std::string name() const;
  
  inline TypeCode type_code() const { return _type_code; }
  inline const TypeDescriptor *subtype() const { return _subtype; }
  
private:
  TypeCode _type_code;
  TypeDescriptor *_subtype;
  
};


inline TypeDescriptor::TypeDescriptor( const TypeDescriptor &other )
{
  *this = other;
}


} // namespace soma

#endif // SOMA_TYPE_DESCRIPTOR
