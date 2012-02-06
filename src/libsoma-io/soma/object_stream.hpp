#ifndef SOMA_OBJECT_STREAM
#define SOMA_OBJECT_STREAM

#include <string>
#include <vector>
#include <soma/type_descriptor.hpp>
#include <soma/memory_layout.hpp>

namespace soma {


typedef std:: size_t FieldIndex;


class FieldsDefinition
{
public:
  size_t count() const;
  FieldIndex index( const std::string &name ) const;
  const std::string &name( FieldIndex field_index ) const;
  FieldIndex type_code( FieldIndex field_index ) const;
  const TypeDescriptor *type_descriptor( FieldIndex field_index ) const;
  const std::string &description( FieldIndex field_index ) const;
  
  void add_field( FieldIndex, const std::string &name, 
                  TypeCode type_code,
                  const std::string &description );
  void add_field( FieldIndex, const std::string &name, 
                  TypeCode type_code, const TypeDescriptor &sub_type,
                  const std::string &description );
  FieldIndex add_field( const std::string &name, 
                        TypeCode type_code,
                        const std::string &description );
  FieldIndex add_field( const std::string &name, 
                        TypeCode type_code, const TypeDescriptor &sub_type,
                        const std::string &description );
  size_t import( const FieldsDefinition &fields_definition );
};


class BinaryIStream
{
  virtual ~BinaryIStream();
  
  virtual TypeCode type_code() const = 0;
  virtual const TypeDescriptor *type_descriptor() const = 0;
  
  virtual const std::vector<size_t> &size() const = 0;
  virtual const MemoryLayout &memory_layout() const = 0;

  virtual void copy_in_memory( void *memory ) const = 0;
  virtual void copy_in_memory( void *memory, const MemoryLayout &memory_layout ) const = 0;
  virtual bool allow_memory_mapping() const = 0;
  virtual std::pair< void *, size_t > mmap() const = 0;
};

class FieldIStream
{
public:
  virtual ~FieldIStream();  
  
  virtual std::vector< FieldIndex > fields() const = 0;
    
  virtual int get_int( FieldIndex field_index ) const = 0;
  virtual double get_double( FieldIndex field_index ) const = 0;
  virtual std::string get_string( FieldIndex field_index ) const = 0;
  virtual void get_int_vector( FieldIndex field_index, std::vector<int> & ) const = 0;
  virtual void get_double_vector( FieldIndex field_index, std::vector<double> & ) const = 0;
  virtual const FieldIStream *get_field_stream( FieldIndex field_index ) const = 0;
  virtual const BinaryIStream *get_binary_stream( FieldIndex field_index ) const = 0;
  
  FieldsDefinition fields_definition;
};


// 
class FieldOStream
{
public:  
  virtual ~FieldOStream();
  
  virtual void set_int( FieldIndex field_index, int ) = 0;
  virtual void set_double( FieldIndex field_index, double ) = 0;
  virtual void set_string( FieldIndex field_index, const std::string & ) = 0;
  virtual void set_int_vector( FieldIndex field_index, const std::vector<int> & ) const = 0;
  virtual void set_double_vector( FieldIndex field_index, const std::vector<double> & ) const = 0;
  virtual const FieldOStream *set_field_stream( FieldIndex field_index ) const = 0;
//   virtual const BinaryOStream *set_binary_stream( FieldIndex field_index ) const = 0;

  
};


} // namespace soma

#endif // SOMA_OBJECT_STREAM
