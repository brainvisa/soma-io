#include <soma/object_stream.hpp>

namespace soma {

const std::string invalid_field_string = "<invalid field>";

FieldDefinition &FieldDefinition::operator =( const FieldDefinition &other )
{
  name = other.name;
  type_code = other.type_code;
  if ( other.type_descriptor ) {
    type_descriptor = new TypeDescriptor( *other.type_descriptor );
  } else {
    type_descriptor = NULL;
  }
  description = other.description;
  return *this;
}



FieldIndex FieldsDefinition::index( const std::string &name ) const
{
  std::map< std::string, FieldIndex >::const_iterator it;
  it = name_to_index.find( name );
  if ( it != name_to_index.end() ) {
    return it->second;
  }
  return invalid_field_index;
}


const FieldDefinition *FieldsDefinition::field( FieldIndex field_index ) const
{
  std::map< FieldIndex, FieldDefinition >::const_iterator it;
  it = index_to_field.find( field_index );
  if ( it != index_to_field.end() ) {
    return &it->second;
  }
  return NULL;
}


void FieldsDefinition::add_field( FieldIndex field_index, const std::string &name, 
                TypeCode type_code,
                const std::string &description )
{
  FieldDefinition field( name, type_code, NULL, description );
  index_to_field[ field_index ] = field;
  name_to_index[ name ] = field_index;
}


void FieldsDefinition::add_field( FieldIndex field_index, const std::string &name, 
                TypeCode type_code, const TypeDescriptor &sub_type,
                const std::string &description )
{
  FieldDefinition field( name, type_code, new TypeDescriptor( sub_type ), description );
  index_to_field[ field_index ] = field;
  name_to_index[ name ] = field_index;
}


FieldIndex FieldsDefinition::add_field( const std::string &name, 
                                        TypeCode type_code,
                                        const std::string &description )
{
  FieldIndex field_index = index_to_field.size();
  add_field( field_index, name, type_code, description );
  return field_index;
}


FieldIndex FieldsDefinition::add_field( const std::string &name, 
                      TypeCode type_code, const TypeDescriptor &sub_type,
                      const std::string &description )
{
  FieldIndex field_index = index_to_field.size();
  add_field( field_index, name, type_code, sub_type, description );
  return field_index;
}


size_t FieldsDefinition::import( const FieldsDefinition &fields_definition )
{
  for( const_iterator it = begin(); it != end(); ++it ) {
    index_to_field[ it->first ] = it->second;
    name_to_index[ it->second.name ] = it->first;
    
  }
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
