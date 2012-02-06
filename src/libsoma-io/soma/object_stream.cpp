#include <soma/object_stream.hpp>

namespace soma {

size_t FieldsDefinition::count() const
{
  // TODO
  return 0;
}


FieldIndex FieldsDefinition::index( const std::string &name ) const
{
  // TODO
  return 0;
}


const std::string &FieldsDefinition::name( FieldIndex field_index ) const
{
  // TODO
  static std::string x;
  return x;
}


FieldIndex FieldsDefinition::type_code( FieldIndex field_index ) const
{
  // TODO
  return 0;
}


const TypeDescriptor *FieldsDefinition::type_descriptor( FieldIndex field_index ) const
{
  // TODO
  return NULL;
}


const std::string &FieldsDefinition::description( FieldIndex field_index ) const
{
  // TODO
  static std::string x;
  return x;
}


void FieldsDefinition::add_field( FieldIndex, const std::string &name, 
                TypeCode type_code,
                const std::string &description )
{
  // TODO
}


void FieldsDefinition::add_field( FieldIndex, const std::string &name, 
                TypeCode type_code, const TypeDescriptor &sub_type,
                const std::string &description )
{
  // TODO
}


FieldIndex FieldsDefinition::add_field( const std::string &name, 
                      TypeCode type_code,
                      const std::string &description )
{
  // TODO
  return 0;
}


FieldIndex FieldsDefinition::add_field( const std::string &name, 
                      TypeCode type_code, const TypeDescriptor &sub_type,
                      const std::string &description )
{
  // TODO
  return 0;
}


size_t FieldsDefinition::import( const FieldsDefinition &fields_definition )
{
  // TODO
  return 0;
}



  
BinaryIStream::~BinaryIStream()
{
}


FieldIStream::~FieldIStream()
{
}


FieldOStream::~FieldOStream()
{
}


} // namespace soma
