#ifndef SOMA_OBJECT_STREAM
#define SOMA_OBJECT_STREAM

#include <string>
#include <vector>
#include <map>
#include <limits>
#include <soma/type_descriptor.hpp>
#include <soma/memory_layout.hpp>

namespace soma {


class FieldDefinition
{
public:
  std::string name;
  TypeCode type_code;
  TypeDescriptor *type_descriptor;
  std::string description;
  
  inline FieldDefinition() : type_code( invalid_type ), type_descriptor( NULL ) {}
  inline FieldDefinition( const FieldDefinition &other ) { *this = other; }
  inline ~FieldDefinition() { if ( type_descriptor ) delete type_descriptor; }
  FieldDefinition &operator =( const FieldDefinition &other );
private:
  inline FieldDefinition( const std::string &n,
                TypeCode code, 
                TypeDescriptor *descriptor,
                const std::string &desc ) :
    name( n ), type_code( code ), type_descriptor( descriptor ),
    description( desc ) {}
    
  friend class FieldsDefinition;
};


typedef std:: size_t FieldIndex;
const FieldIndex invalid_field_index = std::numeric_limits<std::size_t>::max();
extern const std::string invalid_field_string;

  
class FieldsDefinition
{
private:  
  std::map< FieldIndex, FieldDefinition > index_to_field;
  std::map< std::string, FieldIndex > name_to_index;

public:
  inline size_t size() const { return index_to_field.size(); }
  typedef std::map< FieldIndex, FieldDefinition >::const_iterator const_iterator;
  inline const_iterator begin() const { return index_to_field.begin(); }
  inline const_iterator end() const { return index_to_field.end(); }
  
  FieldIndex index( const std::string &name ) const;
  const FieldDefinition *field( FieldIndex field_index ) const;
  
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
public:
  virtual ~BinaryIStream();
  
  virtual TypeCode type_code() const = 0;
  virtual const TypeDescriptor *type_descriptor() const = 0;
  
  virtual const std::vector<size_t> &size() const = 0;
  virtual const MemoryLayout &memory_layout() const = 0;

  virtual void copy_in_memory( void *memory ) const = 0;
  virtual void copy_in_memory( void *memory, const MemoryLayout &memory_layout ) const = 0;
  virtual bool allow_memory_mapping() const = 0;
  virtual std::pair< void *, size_t > memory_mapping() const = 0;
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
  virtual FieldIStream *get_field_stream( FieldIndex field_index ) const = 0;
  virtual BinaryIStream *get_binary_stream( FieldIndex field_index ) const = 0;
};



class BinaryOStream
{
public:
  virtual ~BinaryOStream();
  
  virtual void set_type_code( TypeCode ) const = 0;
  virtual void set_type_descriptor( const TypeDescriptor * ) const = 0;
  
  virtual void set_size( const std::vector<size_t> & ) const = 0;
//   virtual const MemoryLayout &memory_layout() const = 0;

  virtual void copy_from_memory( const void *memory, const size_t count ) const = 0;
  virtual void copy_from_memory( const void *memory, const MemoryLayout &memory_layout ) const = 0;
//   virtual bool allow_memory_mapping() const = 0;
//   virtual std::pair< void *, size_t > mmap() const = 0;

  inline void set_size( size_t size1 )
  {
    std::vector< size_t > size;
    size.push_back( size1 );
    set_size( size );
  }
  
  
  inline void set_size( size_t size1, size_t size2 )
  {
    std::vector< size_t > size;
    size.push_back( size1 );
    size.push_back( size2 );
    set_size( size );
  }
  
  
  inline void set_size( size_t size1, size_t size2, size_t size3 )
  {
    std::vector< size_t > size;
    size.push_back( size1 );
    size.push_back( size2 );
    size.push_back( size3 );
    set_size( size );
  }
  
  
  inline void set_size( size_t size1, size_t size2, size_t size3, size_t size4 )
  {
    std::vector< size_t > size;
    size.push_back( size1 );
    size.push_back( size2 );
    size.push_back( size3 );
    size.push_back( size4 );
    set_size( size );
  }
};



class FieldOStream
{
public:  
  virtual ~FieldOStream();
  
  virtual void set_int( FieldIndex field_index, int ) = 0;
  virtual void set_double( FieldIndex field_index, double ) = 0;
  virtual void set_string( FieldIndex field_index, const std::string & ) = 0;
  virtual void set_int_vector( FieldIndex field_index, const std::vector<int> & ) const = 0;
  virtual void set_double_vector( FieldIndex field_index, const std::vector<double> & ) const = 0;
  virtual FieldOStream *field_ostream( FieldIndex field_index ) const = 0;
  virtual BinaryOStream *binary_ostream( FieldIndex field_index ) const = 0;

  
};


} // namespace soma

#endif // SOMA_OBJECT_STREAM
