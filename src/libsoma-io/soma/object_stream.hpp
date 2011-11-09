#ifndef SOMA_OBJECT_STREAM
#define SOMA_OBJECT_STREAM

#include <string>
#include <soma/type_descriptor.hpp>

namespace soma {

class ObjectStream
{
public:
  virtual ~ObjectStream();  
};


class StructureStream : public ObjectStream {
public:  
  virtual ~StructureStream();
  
  virtual size_t field_count() const = 0;
  virtual size_t current_field_index() const = 0;
  virtual std::string current_field_name() const = 0;
  virtual TypeCode current_field_type_code() const = 0;
  virtual const TypeDescriptor *current_field_type_descriptor() const = 0;
  
  virtual size_t goto_field_index( std::size_t ) = 0;
  virtual size_t goto_field_name( const std::string & ) = 0;
  virtual size_t goto_next_field() = 0;
  
  virtual int current_integer() const = 0;
  virtual double current_double() const = 0;
  virtual std::string current_string() const = 0;
  virtual ObjectStream *current_object_stream() const = 0;
};


} // namespace soma

#endif // SOMA_OBJECT_STREAM
