/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

#ifndef CARTOBASE_OBJECT_OBJECT_D_H
#error This header file must not be included directly, use <cartobase/object/object_d.h> instead.
#endif

#ifndef CARTOBASE_OBJECT_OBJECT_INTERNALS_H
#define CARTOBASE_OBJECT_OBJECT_INTERNALS_H


#ifndef DOXYGEN_HIDE_INTERNAL_CLASSES

namespace interface_internal
{
  //---------------------------------------------------------------------------
  template <typename T, bool B>
  struct GenericInterface {
    static inline Interface *get( TypedObject<T> & );
  };


  //---------------------------------------------------------------------------
  template <typename T>
  struct GenericInterface<T,true> {
    static inline Interface *get( TypedObject<T> &to )
    {
      return static_cast< Interface * >( &to.getValue() );
    }
  };

  //---------------------------------------------------------------------------
  template <typename T>
  struct GenericInterface<T,false> {
    static inline Interface *get( TypedObject<T> &to )
    {
      return  &to;
    }
  };

  
  //---------------------------------------------------------------------------
  // ScalarInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class ScalarImpl
  {
  public:

    static inline bool isScalar( const TypedObject<T> & )
    {
      return false;
    }

    static inline double getScalar( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "Cannot convert object of "
                                             "type " ) 
                                + DataTypeCode<T>::name() + " to scalar" );
      return 0.0;
    }
  
    static inline void setScalar( TypedObject<T> &, double )
    {
      throw std::runtime_error( std::string( "Cannot convert scalar to "
                                             "object of type " ) + 
                                DataTypeCode<T>::name() );
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      throw std::runtime_error( std::string( "Cannot convert scalar to "
                                             "object of type " ) +
                                DataTypeCode<T>::name() );
    }
  };

  template <typename T>
  class ScalarImpl< T, true >
  {
  public:

    static inline bool isScalar( const TypedObject<T> & object )
    {
      return object.getValue().isScalar();
    }

    static inline double getScalar( const TypedObject<T> &object )
    {
      return object.getValue().getScalar();
    }
  
    static inline void setScalar( TypedObject<T> &object, double value )
    {
      object.getValue().setScalar( value );
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue().getScalar() == o2.getScalar();
    }
  };


  //---------------------------------------------------------------------------
  // StringInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class StringImpl
  {
  public:

    static inline bool isString( const TypedObject<T> & )
    {
      return false;
    }

    static inline std::string getString( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "Cannot convert object of type " )
                                + DataTypeCode<T>::name() + " to string" );
      return std::string();
    }
  
    static inline void setString( TypedObject<T> &, const std::string & )
    {
      throw std::runtime_error( std::string( "Cannot convert string to "
                                             "object of type " ) + 
                                DataTypeCode<T>::name() );
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
       throw std::runtime_error( std::string( "Cannot convert string to "
                                             "object of type " ) +
                                DataTypeCode<T>::name() );
    }
  };

  template <typename T>
  class StringImpl< T, true >
  {
  public:

    static inline bool isString( const TypedObject<T> & object )
    {
      return object.getValue().isString();
    }

    static inline std::string getString( const TypedObject<T> &object )
    {
      return object.getValue().getString();
    }
  
    static inline void setString( TypedObject<T> &object, 
                                  const std::string &value )
    {
      object.getValue().setString( value );
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      return o2.isString() && o1.getValue().getString() == o2.getString();
    }
  };


  //---------------------------------------------------------------------------
  // SizeInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class SizeImpl
  {
  public:
    static inline size_t size( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "cannot get size of object of "
                                             "type "
                                             ) + DataTypeCode<T>::name() );
      return 0;
    }
  };

  template <typename T>
  class SizeImpl<T,true>
  {
  public:
    static inline size_t size( const TypedObject<T> &object )
    {
      return object.getValue().size();
    }
  };

  
  //---------------------------------------------------------------------------
  // ArrayInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class ArrayImpl
  {
  public:

    static inline bool isArray( const TypedObject<T> & )
    {
      return false;
    }

    static inline Object getArrayItem( const TypedObject<T> &, int )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not an Array" );
      return Object();
    }

    static inline void setArrayItem( TypedObject<T> &, int, Object )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not an Array" );
    }
  };

  template <typename T>
  class ArrayImpl< T, true >
  {
  public:

    static inline bool isArray( const TypedObject<T> & object )
    {
      return object.getValue().isArray();
    }

    static inline Object getArrayItem( const TypedObject<T> &object,
                                       int index )
    {
      return object.getValue().getArrayItem( index );
    }

    static inline void setArrayItem( TypedObject<T> &object, int index,
                                     Object value )
    {
      object.getValue().setArrayItem( index, value );
    }
  };



  //---------------------------------------------------------------------------
  // DynArrayInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class DynArrayImpl
  {
  public:

    static inline bool isDynArray( const TypedObject<T> & )
    {
      return false;
    }

    static inline void reserveArray( TypedObject<T> &, size_t )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not a DynArray" );
    }

    static inline void resizeArray( TypedObject<T> &, size_t )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not a DynArray" );
    }

    
    static inline void removeArrayItem( TypedObject<T> &, int )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not a DynArray" );
    }

    
    static inline void insertArrayItem( TypedObject<T> &,
                                        int, Object )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not a DynArray" );
    }
  };

  template <typename T>
  class DynArrayImpl< T, true >
  {
  public:

    static inline bool isDynArray( const TypedObject<T> & object )
    {
      return object.getValue().isDynArray();
    }

    static inline void reserveArray( TypedObject<T> &object,
                                     size_t size )
    {
      object.getValue().reserveArray( size ); 
    }

    static inline void resizeArray( TypedObject<T> &object,
                                    size_t size )
    {
      object.getValue().resizeArray( size ); 
    }

    
    static inline void removeArrayItem( TypedObject<T> &object,
                                        int index )
    {
      object.getValue().removeArrayItem( index ); 
    }

    
    static inline void insertArrayItem( TypedObject<T> &object,
                                        int index, Object value )
    {
      object.getValue().insertArrayItem( index, value ); 
    }
  };




  //---------------------------------------------------------------------------
  // DictionaryInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class DictionaryImpl
  {
  public:

    static inline bool isDictionary( const TypedObject<T> & )
    {
      return false;
    }

    static inline bool getProperty( const TypedObject<T> &, 
                                    const std::string &, Object & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not a "
                                "Dictionary" );
      return false;
    }

    static inline void setProperty( TypedObject<T> &,
                                    const std::string &, Object )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not a "
                                "Dictionary" );
    }

    static inline bool removeProperty( TypedObject<T> &, const std::string & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not a "
                                "Dictionary" );
      return false;
    }

    static inline bool hasProperty( const TypedObject<T> &, const std::string & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not a "
                                "Dictionary" );
      return false;
    }

    static inline void clearProperties( TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + " is not a "
                                "Dictionary" );
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      throw std::runtime_error( std::string( "object of type " ) +
                                DataTypeCode<T>::name() + " is not a "
                                "Dictionary" );
    }

  };

  template <typename T>
  class DictionaryImpl< T, true >
  {
  public:

    static inline bool isDictionary( const TypedObject<T> & object )
    {
      return object.getValue().isDictionary();
    }

    static inline bool getProperty( const TypedObject<T> &object, 
                                    const std::string &key, Object & result )
    {
      return object.getValue().getProperty( key, result );
    }

    static inline void setProperty( TypedObject<T> &object,
                                          const std::string &key,
                                          Object value )
    {
      object.getValue().setProperty( key, value );
    }

    static inline bool removeProperty(  TypedObject<T> &object,
                                        const std::string &key )
    {
      return object.getValue().removeProperty( key );
    }

    static inline bool hasProperty( const TypedObject<T> & object, 
                               const std::string & key )
    {
      return object.getValue().hasProperty( key );
    }

    static inline void clearProperties( TypedObject<T> &object )
    {
      object.getValue().clearProperties();
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      if( !o2.isDictionary() )
        return false;
      const T & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      Object it, other_value;
      for( it=self.objectIterator(); it->isValid(); it->next() )
      {
        try
        {
          other_value = o2.getProperty( it->key() );
          if( it->currentValue() != other_value )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }

  };



  //---------------------------------------------------------------------------
  // IterableInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class IterableImpl
  {
  public:

    static inline bool isIterable( const TypedObject<T> & )
    {
      return false;
    }

    static inline Object objectIterator( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not iterable" );
      return Object();
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      throw std::runtime_error( std::string( "object of type " ) +
                                DataTypeCode<T>::name() +
                                " is not iterable" );
    }
  };

  template <typename T>
  class IterableImpl< T, true >
  {
  public:

    static inline bool isIterable( const TypedObject<T> & object )
    {
      return object.getValue().isIterable();
    }

    static inline Object objectIterator( const TypedObject<T> &object )
    {
      return object.getValue().objectIterator();
    }

    static inline bool equals( const TypedObject<T> & o1,
                               const GenericObject & o2 )
    {
      if( !o2.isIterable() )
        return false;
      const T & self = o1.getValue();
      if( o1.size() != o2.size() )
        return false;
      Object it, it2;
      for( it=self.objectIterator(), it2=o2.objectIterator();
           it->isValid(); it->next(), it2->next() )
      {
        if( !it2->isValid() || it->currentValue() != it2->currentValue() )
          return false;
      }
      return true;
    }

  };



  //---------------------------------------------------------------------------
  // IteratorInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class IteratorImpl
  {
  public:

    static inline bool isIterator( const TypedObject<T> & )
    {
      return false;
    }

    static inline bool isValid( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not an iterator" );
      return false;
    }

    static inline Object currentValue( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not an iterator" );
      return Object();
    }

    static inline void next( TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not an iterator" );
    }
  };


  template <typename T>
  class IteratorImpl< T, true >
  {
  public:

    static inline bool isIterator( const TypedObject<T> & object )
    {
      return object.getValue().isIterator();
    }

    static inline bool isValid( const TypedObject<T> &object )
    {
      return object.getValue().isValid();
    }

    static inline Object currentValue( const TypedObject<T> &object )
    {
      return object.getValue().currentValue();
    }

    static inline void next( TypedObject<T> &object )
    {
      return object.getValue().next();
    }
  };


  //---------------------------------------------------------------------------
  // KeyIteratorInterface default implementation
  //---------------------------------------------------------------------------
  template <typename T, bool B>
  class KeyIteratorImpl
  {
  public:

    static inline bool isKeyIterator( const TypedObject<T> & )
    {
      return false;
    }

    static inline Object keyObject( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) +
                                DataTypeCode<T>::name() +
                                " is not a key iterator" );
      return Object();
    }
  };

  template <typename T>
  class KeyIteratorImpl< T, true >
  {
  public:

    static inline bool isKeyIterator( const TypedObject<T> & object )
    {
      return object.getValue().isKeyIterator();
    }

    static inline Object keyObject( const TypedObject<T> &object )
    {
      return object.getValue().keyObject();
    }
  };


  //---------------------------------------------------------------------------
  // DictionaryIteratorInterface default implementation
  //---------------------------------------------------------------------------
  template <typename T, bool B>
  class DictionaryIteratorImpl
  {
  public:

    static inline bool isDictionaryIterator( const TypedObject<T> & )
    {
      return false;
    }

    static inline std::string key( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) + 
                                DataTypeCode<T>::name() + 
                                " is not a dictionary iterator" );
      return std::string();
    }
  };

  template <typename T>
  class DictionaryIteratorImpl< T, true >
  {
  public:

    static inline bool isDictionaryIterator( const TypedObject<T> & object )
    {
      return object.getValue().isDictionaryIterator();
    }

    static inline std::string key( const TypedObject<T> &object )
    {
      return object.getValue().key();
    }
  };


  //---------------------------------------------------------------------------
  // IntKeyIteratorInterface default implementation
  //---------------------------------------------------------------------------
  template <typename T, bool B>
  class IntDictionaryIteratorImpl
  {
  public:

    static inline bool isIntKeyIterator( const TypedObject<T> & )
    {
      return false;
    }

    static inline long intKey( const TypedObject<T> & )
    {
      throw std::runtime_error( std::string( "object of type " ) +
                                DataTypeCode<T>::name() +
                                " is not an int dictionary iterator" );
      return 0;
    }
  };

  template <typename T>
  class IntDictionaryIteratorImpl< T, true >
  {
  public:

    static inline bool isIntKeyIterator( const TypedObject<T> & object )
    {
      return object.getValue().isIntKeyIterator();
    }

    static inline long intKey( const TypedObject<T> &object )
    {
      return object.getValue().intKey();
    }
  };


  //---------------------------------------------------------------------------
  // NoneInterface default implementation
  //---------------------------------------------------------------------------


  template <typename T, bool B>
  class NoneImpl
  {
  public:
    static inline bool isNone( const TypedObject<T> & )
    {
      return false;
    }
  };


     //-----------------------------------------------//
    // Build a ValueObject if T is not a pointer to  //
   //  a subclass of GenericObject                  //
  //-----------------------------------------------//

  template <typename T, bool B>
  struct ValueObjectImpl
  {
    static Object object( const T & );
  };

  template <typename T, bool B>
  Object ValueObjectImpl<T,B>::object( const T & x )
  {
    return Object( static_cast<GenericObject *>( new ValueObject<T>( x ) ) );
  }
  
  template <typename T>
  struct ValueObjectImpl<T,true>
  {
    static Object object( const T & );
  };

  template <typename T>
  Object ValueObjectImpl<T,true>::object( const T & x )
  {
    return Object( static_cast<GenericObject *> ( x ) );
  }



    //--------//
   //  char  //
  //--------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< char, false >
  {
  public:
    static inline bool isScalar( const TypedObject<char> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<char> &to )
    {
      return to.getValue();
    }

    static inline void setScalar( TypedObject<char> &to, double value )
    {
      to.getValue() = static_cast<char>( value );
    }

    static inline bool equals( const TypedObject<char> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //-----------------//
   //  unsigned char  //
  //-----------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< unsigned char, false >
  {
  public:
    static inline bool isScalar( const TypedObject<unsigned char> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<unsigned char> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<unsigned char> &to, 
                                  double value )
    {
      to.getValue() = static_cast<unsigned char>( value );
    }

    static inline bool equals( const TypedObject<unsigned char> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //---------------//
   //  signed char  //
  //---------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< signed char, false >
  {
  public:
    static inline bool isScalar( const TypedObject<signed char> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<signed char> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<signed char> &to, double value )
    {
      to.getValue() = static_cast<signed char>( value );
    }

    static inline bool equals( const TypedObject<signed char> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };

    //--------//
   //  bool  //
  //--------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< bool, false >
  {
  public:
    static inline bool isScalar( const TypedObject<bool> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<bool> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<bool> &to, double value )
    {
      to.getValue() = static_cast<bool>( value );
    }

    static inline bool equals( const TypedObject<bool> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //------------------//
   //  unsigned short  //
  //------------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< unsigned short, false >
  {
  public:
    static inline bool isScalar( const TypedObject<unsigned short> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<unsigned short> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<unsigned short> &to, double value )
    {
      to.getValue() = static_cast<unsigned short>( value );
    }

    static inline bool equals( const TypedObject<unsigned short> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //---------//
   //  short  //
  //---------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< short, false >
  {
  public:
    static inline bool isScalar( const TypedObject<short> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<short> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<short> &to, double value )
    {
      to.getValue() = static_cast<short>( value );
    }

    static inline bool equals( const TypedObject<short> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };

    //------------//
   //  unsigned  //
  //------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< unsigned, false >
  {
  public:
    static inline bool isScalar( const TypedObject<unsigned> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<unsigned> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<unsigned> &to, double value )
    {
      to.getValue() = static_cast<unsigned>( value );
    }

    static inline bool equals( const TypedObject<unsigned> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //-------//
   //  int  //
  //-------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< int, false >
  {
  public:
    static inline bool isScalar( const TypedObject<int> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<int> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<int> &to, double value )
    {
      to.getValue() = static_cast<int>( value );
    }

    static inline bool equals( const TypedObject<int> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //--------//
   //  long  //
  //--------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< long, false >
  {
  public:
    static inline bool isScalar( const TypedObject<long> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<long> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<long> &to, double value )
    {
      to.getValue() = static_cast<long>( value );
    }

    static inline bool equals( const TypedObject<long> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //-----------------//
   //  unsigned long  //
  //-----------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< unsigned long, false >
  {
  public:
    static inline bool isScalar( const TypedObject<unsigned long> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<unsigned long> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<unsigned long> &to, 
                                  double value )
    {
      to.getValue() = static_cast<unsigned long>( value );
    }

    static inline bool equals( const TypedObject<unsigned long> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //------------//
   //  long long //
  //------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< long long, false >
  {
  public:
    static inline bool isScalar( const TypedObject<long long> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<long long> &to )
    {
      return to.getValue();
    }

    static inline void setScalar( TypedObject<long long> &to, double value )
    {
      to.getValue() = static_cast<long long>( value );
    }

    static inline bool equals( const TypedObject<long long> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //---------------------//
   //  unsigned long long //
  //---------------------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< unsigned long long, false >
  {
  public:
    static inline bool isScalar( const TypedObject<unsigned long long> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<unsigned long long> &to )
    {
      return to.getValue();
    }

    static inline void setScalar( TypedObject<unsigned long long> &to,
                                  double value )
    {
      to.getValue() = static_cast<unsigned long long>( value );
    }

    static inline bool equals( const TypedObject<unsigned long long> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //---------//
   //  float  //
  //---------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< float, false >
  {
  public:
    static inline bool isScalar( const TypedObject<float> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<float> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<float> &to, double value )
    {
      to.getValue() = static_cast<float>( value );
    }

    static inline bool equals( const TypedObject<float> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };

    //----------//
   //  double  //
  //----------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< double, false >
  {
  public:
    static inline bool isScalar( const TypedObject<double> & )
    {
      return true;
    }

    static inline double getScalar( const TypedObject<double> &to )
    {
      return to.getValue();
    }
  
    static inline void setScalar( TypedObject<double> &to, double value )
    {
      to.getValue() = value;
    }

    static inline bool equals( const TypedObject<double> & o1,
                               const GenericObject & o2 )
    {
      return o2.isScalar() && o1.getValue() == o2.getScalar();
    }
  };


    //----------//
   //  string  //
  //----------//

  //---------------------------------------------------------------------------
  template <>
  class ScalarImpl< std::string, false >
  {
  public:
    static inline bool isScalar( const TypedObject<std::string> & )
    {
      return false;
    }

    static inline double getScalar( const TypedObject<std::string> &to )
    {
      double	x;
      stringTo( to.getValue(), x );
      return x;
    }

    static inline void setScalar( TypedObject<std::string> &to, double value )
    {
      to.getValue() = toString( value );
    }

    static inline bool equals( const TypedObject<std::string> & o1,
                               const GenericObject & o2 )
    {
      return o2.isString() && o1.getValue() == o2.getString();
    }
  };


    //----------//
   //  string  //
  //----------//

  //---------------------------------------------------------------------------
  template <>
  class StringImpl< std::string, false >
  {
  public:
    static inline bool isString( const TypedObject< std::string> & )
    {
      return true;
    }

    static inline std::string getString( const TypedObject<std::string> &to )
    {
      return to.getValue();
    }
  
    static inline void setString( TypedObject<std::string> &to,
                                  const std::string &value )
    {
      to.getValue() = value;
    }

    static inline bool equals( const TypedObject<std::string> & o1,
                               const GenericObject & o2 )
    {
      return o2.isString() && o1.getValue() == o2.getString();
    }
  };

  //---------------------------------------------------------------------------
  template <>
  inline std::string 
  StringImpl< char, false >::getString( const TypedObject<char> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< char, false >::setString( TypedObject<char> &to,
                                        const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< signed char, false >::getString( const TypedObject<signed char> 
                                               &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< signed char, false >::setString( TypedObject<signed char> &to,
                                               const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< unsigned char, false >::getString( const 
                                                 TypedObject<unsigned char> 
                                                 &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< unsigned char, false >::setString( TypedObject<unsigned char> 
                                                 &to,
                                                 const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< short, false >::getString( const TypedObject<short> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< short, false >::setString( TypedObject<short> &to,
                                        const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< unsigned short, false >::getString( const 
                                                  TypedObject<unsigned short> 
                                                  &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< unsigned short, false >::setString( TypedObject<unsigned short> &
                                                  to,
                                                  const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< int, false >::getString( const TypedObject<int> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< int, false >::setString( TypedObject<int> &to,
                                       const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< unsigned, false >::getString( const TypedObject<unsigned> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< unsigned, false >::setString( TypedObject<unsigned> &to,
                                            const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< long, false >::getString( const TypedObject<long> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< long, false >::setString( TypedObject<long> &to,
                                       const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< unsigned long, false >::getString( const 
                                                 TypedObject<unsigned long> &
                                                 to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void
  StringImpl< unsigned long, false >::setString( TypedObject<unsigned long> & 
                                                 to,
                                                 const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< float, false >::getString( const TypedObject<float> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void 
  StringImpl< float, false >::setString( TypedObject<float> &to,
                                         const std::string &value )
  {
    stringTo( value, to.getValue() );
  }

  template <>
  inline std::string 
  StringImpl< double, false >::getString( const TypedObject<double> &to )
  {
    return toString( to.getValue() );
  }
  
  template <>
  inline void 
  StringImpl< double, false >::setString( TypedObject<double> &to,
                                          const std::string &value )
  {
    stringTo( value, to.getValue() );
  }


    //-------------//
   //  vector<T>  //
  //-------------//


  // SizeInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class SizeImpl<std::vector<T>,false>
  {
  public:
    static inline size_t 
    size( const TypedObject<std::vector<T> > &object )
    {
      return object.getValue().size();
    }
  };


  // ArrayInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class ArrayImpl< std::vector<T>, false >
  {
  public:

    static inline bool isArray( const TypedObject< std::vector<T> > & )
    {
      return true;
    }

    static inline Object 
    getArrayItem( const TypedObject<std::vector<T> > &object,
                  int index )
    {
      // WARNING: const_cast is dangerous here !
      return Object::reference( const_cast<T &>
                                ( object.getValue()[ index ] ) );
    }

    static inline void setArrayItem( TypedObject<std::vector<T> > &object, 
                                     int index, Object value )
    {
      object.getValue()[ index ] = value->GenericObject::value<T>();
    }

  };


  // DynArrayInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class DynArrayImpl< std::vector<T>, false >
  {
  public:
    static inline bool isDynArray( const TypedObject< std::vector<T> > & )
    {
      return true;
    }

    static inline void 
    reserveArray( TypedObject< std::vector<T> > &object,
                  size_t size )
    {
      object.getValue().reserve( size ); 
    }

    static inline void
    resizeArray( TypedObject< std::vector<T> > &object,
                 size_t size )
    {
      object.getValue().resize( size ); 
    }

    static inline void 
    removeArrayItem( TypedObject< std::vector<T> > &object,
                     int index )
    {
      object.getValue().erase( object.getValue().begin() + index ); 
    }

    
    static inline void 
    insertArrayItem( TypedObject< std::vector<T> > &object,
                     int index, Object value )
    {
      if( index >= 0 )
        object.getValue().insert( object.getValue().begin() + index,
                                  value->GenericObject::value<T>() ); 
      else
        object.getValue().insert( object.getValue().end() + ( index + 1 ),
                                  value->GenericObject::value<T>() );
    }
  };


  // IterableInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class IterableImpl<std::vector<T>, false>
  {
  public:
    static inline bool isIterable( const TypedObject< std::vector<T> > & )
    {
      return true;
    }

    static inline Object 
    objectIterator( const TypedObject<std::vector<T> > & object );

    static inline bool equals( const TypedObject< std::vector<T> > & o1,
                               const GenericObject & o2 )
    {
      if( !o2.isIterable() )
        return false;
      const std::vector<T> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      Object it, it2;
      for( it=o1.objectIterator(), it2=o2.objectIterator();
           it->isValid(); it->next(), it2->next() )
      {
        if( !it2->isValid() || it->currentValue() != it2->currentValue() )
          return false;
      }
      return true;
    }

  };
  
  
  template <typename V>
  class VectorIterator : public IteratorInterface
  {
  public:
    inline VectorIterator() {};
    // DictionaryIteratorInterface methods
    bool isValid() const;
    Object currentValue() const;
    void next();

    inline 
    VectorIterator( const typename V::const_iterator &begin,
                    const typename V::const_iterator &end ) :
      _iterator( begin ),
      _end( end ) {}

    typename V::const_iterator _iterator; 
    typename V::const_iterator _end; 
  };

} // namespace interface_internal

template <typename V>
struct GenericObjectTypeDeclared< typename interface_internal::VectorIterator< V > >
{
  static inline void check() {};
};

namespace interface_internal {


  template <typename V>
  bool VectorIterator< V >::isValid() const
  {
    return _iterator != _end;
  }

  template <typename V>
  Object VectorIterator< V >::currentValue() const
  {
    // WARNING: const_cast is dangerous here !
    return Object::reference( const_cast<typename V::reference>( *_iterator ) );
  }

  template <>
  inline Object VectorIterator<std::vector<Object> >::currentValue() const
  {
    return *_iterator;
  }

  template <typename V>
  inline void VectorIterator< V >::next()
  {
    ++_iterator;
  }

  template <typename T>
  inline Object IterableImpl<std::vector<T>, false>::objectIterator
  ( const TypedObject<std::vector<T> > & object )
  {
    return Object::value( VectorIterator<std::vector< T > >
                          ( object.getValue().begin(), 
                            object.getValue().end() ) );
  }


    //------------------//
   //  vector<Object>  //
  //------------------//

  // ArrayInterface
  //---------------------------------------------------------------------------
  template<>
  inline Object 
  ArrayImpl< std::vector<Object>, 
             false >::getArrayItem( const TypedObject<std::vector<Object> > &
                                    object, int index )
  {
    return object.getValue()[ index ];
  }

  template<>
  inline void 
  ArrayImpl< std::vector<Object>, 
             false >::setArrayItem( TypedObject<std::vector<Object> > &
                                    object, int index, Object value )
  {
    object.getValue()[ index ] = value;
  }


  // DynArrayInterface
  //---------------------------------------------------------------------------
  template<>
  inline void 
  DynArrayImpl< std::vector<Object>, false >::
  insertArrayItem( TypedObject< std::vector<Object> > &object,
                   int index, Object value )
  {
    if( index >= 0 )
      object.getValue().insert( object.getValue().begin() + index, value );
    else
      object.getValue().insert( object.getValue().end() + (index + 1), value );
  }




    //----------//
   //  set<T>  //
  //----------//


  // SizeInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class SizeImpl<std::set<T>,false>
  {
  public:
    static inline size_t 
    size( const TypedObject<std::set<T> > &object )
    {
      return object.getValue().size();
    }
  };


  // IterableInterface
  //---------------------------------------------------------------------------
  template <typename S>
  class SetIterator : public IteratorInterface
  {
  public:
    inline SetIterator(): IteratorInterface() {};
    inline bool isValid() const
    {
      return _iterator != _end;
    };
    inline Object currentValue() const
    {
      return Object::reference( *_iterator );
    }
    inline void next()
    {
      ++_iterator;
    }

    inline 
    SetIterator( const typename S::const_iterator &begin,
                  const typename S::const_iterator &end )
     : IteratorInterface(), _iterator( begin ), _end( end ) {}


    typename S::const_iterator _iterator; 
    typename S::const_iterator _end; 
  };

} // namespace interface_internal

template <typename T>
struct GenericObjectTypeDeclared< typename interface_internal::SetIterator<std::set<T> > >
{
  static inline void check() {};
};

namespace interface_internal {

    
  template <typename T>
  class IterableImpl<std::set<T>, false>
  {
  public:

    static inline bool isIterable( const TypedObject< std::set<T> > & )
    {
      return true;
    }

    static inline Object 
    objectIterator( const TypedObject<std::set<T> > & object );

    static inline bool equals(
      const TypedObject< std::set<T> > & o1,
      const GenericObject & o2 )
    {
      if( !o2.isIterable() )
        return false;
      const std::set<T> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      Object it2, value;
      for( it2=o2.objectIterator(); it2->isValid(); it2->next() )
      {
        value = it2->currentValue();
        if( !value.get() )
          return false;
        try
        {
          const T & tval = value->value<T>();
          if( self.find( tval ) == self.end() )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }

  };


  template <typename T> inline
  Object 
  IterableImpl<std::set<T>, false>::objectIterator
  ( const TypedObject<std::set<T> > & object )
  {
    return Object::value( SetIterator< std::set<T> >( object.getValue().begin(), 
                                                      object.getValue().end() ) );
  }

    //---------------//
   //  set<Object>  //
  //---------------//

  template <>
  class SetIterator< std::set<Object> > : public IteratorInterface
  {
  public:
    inline SetIterator(): IteratorInterface() {};
    inline bool isValid() const
    {
      return _iterator != _end;
    };
    inline Object currentValue() const
    {
      return *_iterator;
    }
    inline void next()
    {
      ++_iterator;
    }

    inline 
    SetIterator( const std::set<Object>::const_iterator &begin,
                 const std::set<Object>::const_iterator &end )
     : IteratorInterface(), _iterator( begin ), _end( end ) {}


    std::set<Object>::const_iterator _iterator; 
    std::set<Object>::const_iterator _end; 
  };

  
    //------------//
   //  map<T,U>  //
  //------------//


  // SizeInterface
  //---------------------------------------------------------------------------
  template <typename T, typename U>
  class SizeImpl<std::map<T, U>,false>
  {
  public:
    static inline size_t 
    size( const TypedObject<std::map<T, U> > &object )
    {
      return object.getValue().size();
    }
  };


  // IterableInterface
  //---------------------------------------------------------------------------
  template <typename T, typename U>
  class IterableImpl<std::map<T, U>, false>
  {
  public:

    static inline bool isIterable( const 
                                   TypedObject< std::map<T, U> > & )
    {
      return true;
    }

    static inline Object 
    objectIterator( const TypedObject<std::map<T, U> > & object );

    static inline bool equals(
      const TypedObject< std::map<T, U> > & o1,
      const GenericObject & o2 )
    {
      if( !o2.isDictionary() )
        return false;
      const std::map<T, U> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      Object it, value;
      for( it=o1.objectIterator(); it->isValid(); it->next() )
      {
        try
        {
          value = o2.getProperty( it->key() );
          if( it->currentValue() != value )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }

  };

  template <typename M>
  class MapIterator : public IteratorInterface
  {
  public:
    inline MapIterator() {_iterator = _end; };
    bool isValid() const
    {
      return _iterator != _end;
    }
    Object currentValue() const
    {
      return Object::reference( _iterator->second );
    }
    void next()
    {
      ++_iterator;
    }

    inline 
    MapIterator( const typename M::const_iterator 
                  &begin,
                  const typename M::const_iterator 
                  &end ) : IteratorInterface(), 
      _iterator( begin ), _end( end )
    {
    }

    typename M::const_iterator _iterator; 
    typename M::const_iterator _end; 
  };
} // namespace interface_internal

template <typename T,typename U>
struct GenericObjectTypeDeclared< typename interface_internal::MapIterator<std::map<T,U> > >
{
  static inline void check() {};
};

namespace interface_internal {

  template <typename T, typename U> inline
  Object 
  IterableImpl<std::map<T, U>, false>::objectIterator
  ( const TypedObject<std::map<T, U> > & object )
  {
    return Object::value( MapIterator< std::map<T, U> >
                          ( object.getValue().begin(), 
                            object.getValue().end() ) );
  }


    //-----------------//
   //  map<T,Object>  //
  //-----------------//

  template <typename T>
  class IterableImpl<std::map<T, Object>, false>
  {
  public:

    static inline bool isIterable( const 
                                   TypedObject< std::map<T, Object> > & )
    {
      return true;
    }

    static inline Object 
    objectIterator( const TypedObject<std::map<T, Object> > & object );

    static inline bool equals(
      const TypedObject< std::map<T, Object> > & o1,
      const GenericObject & o2 )
    {
      if( !o2.isIterable() )
        return false;
      const std::map<T, Object> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      typename std::map<T, Object>::const_iterator it;
      Object it2, value;
      // the key T cannot be checked in o2. The only thing we can do here is
      // to check values in same order.
      // this is incomplete, agreed.
      for( it=self.begin(), it2=o2.objectIterator(); it!=self.end();
           ++it, it2->next() )
      {
        if( it->second != it2->currentValue() )
          return false;
      }
      return true;
    }
  };


  template <typename T>
  class MapIterator< std::map< T, Object > > : public IteratorInterface
  {
  public:
    inline MapIterator() { _iterator = _end; }
    bool isValid() const
    {
      return _iterator != _end;
    }
    Object currentValue() const
    {
      return _iterator->second;
    }
    void next()
    {
      ++_iterator;
    }

    inline 
    MapIterator( const typename std::map< T, Object >::const_iterator &begin,
                  const typename std::map< T, Object >::const_iterator &end ) :
     IteratorInterface(), 
     _iterator( begin ),
     _end( end ) {}

    typename std::map< T, Object >::const_iterator _iterator; 
    typename std::map< T, Object >::const_iterator _end; 
  };


  template <typename T> inline
  Object 
  IterableImpl<std::map<T, Object>, false>::objectIterator
  ( const TypedObject<std::map<T, Object> > & object )
  {
    return Object::value( MapIterator<std::map<T, Object> >( object.getValue().begin(), 
                                       object.getValue().end() ) );
  }


    //-----------------//
   //  map<string,U>  //
  //-----------------//


  // DictionaryInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class DictionaryImpl<std::map<std::string, T>, false>
  {
  public:

    static inline bool isDictionary( const 
                                     TypedObject< std::map<std::string, T> > &)
    {
      return true;
    }

    static inline bool 
    getProperty( const TypedObject<std::map<std::string, T> > & object, 
                 const std::string & key, Object & result )
    {
      typename std::map<std::string, T>::const_iterator 
        i = object.getValue().find( key );
      if( i == object.getValue().end() )
        return false;
      // WARNING: const_cast is dangerous here !
      result = Object::reference( const_cast<T &>( i->second ) );
      return true;
    }

    static inline void 
    setProperty( TypedObject<std::map<std::string, T> > & object,
                       const std::string & key, Object value )
    {
      T	& val = value->GenericObject::value<T>();
      object.getValue()[ key ] = val;
    }

    static inline bool 
    removeProperty( TypedObject<std::map<std::string, T> > & object, 
                    const std::string & key )
    {
      return object.getValue().erase( key );
    }

    static inline void clearProperties( TypedObject<std::map<std::string, T> > 
                                        & object )
    {
      object.getValue().clear();
    }

    static inline 
    bool hasProperty( const TypedObject<std::map<std::string, T> > & 
                      object, const std::string & key )
    {
      return object.getValue().find( key ) != object.getValue().end();
    }

    static bool equals( const TypedObject<std::map<std::string, T> > &
                        object, const GenericObject & other )
    {
      if( !other.isDictionary() )
        return false;
      const std::map<std::string, T> & self = object.getValue();
      if( self.size() != other.size() )
        return false;
      Object it, other_value;
      for( it=object.objectIterator(); it->isValid(); it->next() )
      {
        try
        {
          other_value = other.getProperty( it->key() );
          if( it->currentValue() != other_value )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }
  };


  // IterableInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class MapIterator< std::map<std::string, T > > :
    public DictionaryIteratorInterface
  {
  public:
    inline MapIterator() { _iterator = _end; }

    bool isValid() const
    {
      return _iterator != _end;
    }

    Object currentValue() const
    {
      return Object::reference( _iterator->second );
    }
    
    void next()
    {
      ++_iterator;
    }
    
    std::string key() const
    {
      return _iterator->first;
    }

    inline 
    MapIterator( const typename 
                 std::map< std::string, T >::const_iterator &begin,
                 const typename 
                 std::map< std::string, T >::const_iterator &end ) :
     IteratorInterface(), 
     _iterator( begin ),
     _end( end ) {}

    typename std::map< std::string, T >::const_iterator _iterator; 
    typename std::map< std::string, T >::const_iterator _end; 
  };

  template <typename T>
  class IterableImpl<std::map<std::string, T>, false>
  {
  public:

    static inline bool isIterable( const 
                                   TypedObject< std::map<std::string, T> > & )
    {
      return true;
    }

    static inline Object 
    objectIterator( const TypedObject<std::map<std::string, T> > & object );

    static inline bool equals(
      const TypedObject< std::map<std::string, T> > & o1,
      const GenericObject & o2 )
    {
      if( !o2.isDictionary() )
        return false;
      const std::map<std::string, T> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      Object it, value;
      for( it=o1.objectIterator(); it->isValid(); it->next() )
      {
        try
        {
          value = o2.getProperty( it->key() );
          if( it->currentValue() != value )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }

  };


  template <typename T> inline
  Object 
  IterableImpl<std::map<std::string, T>, false>::objectIterator
  ( const TypedObject<std::map<std::string, T> > & object )
  {
    return Object::value( MapIterator<std::map<std::string, T> >
                          ( object.getValue().begin(), 
                            object.getValue().end() ) );
  }


    //----------------------//
   //  map<string,Object>  //
  //----------------------//

  // IterableInterface
  //---------------------------------------------------------------------------

  template <>
  class MapIterator< std::map<std::string, Object > > :
    public DictionaryIteratorInterface
  {
  public:
    inline MapIterator() { _iterator = _end; }

    bool isValid() const
    {
      return _iterator != _end;
    }

    Object currentValue() const
    {
      return _iterator->second;
    }
    
    void next()
    {
      ++_iterator;
    }
    
    std::string key() const
    {
      return _iterator->first;
    }

    inline 
    MapIterator( const std::map< std::string, Object >::const_iterator &begin,
                 const std::map< std::string, Object >::const_iterator &end ) :
      IteratorInterface(), DictionaryIteratorInterface(),
     _iterator( begin ),
     _end( end ) {}

    std::map< std::string, Object >::const_iterator _iterator; 
    std::map< std::string, Object >::const_iterator _end; 
  };

  template <>
  class IterableImpl<std::map<std::string, Object>, false>
  {
  public:


    static inline bool isIterable( const 
                                   TypedObject< std::map<std::string, 
                                   Object> > & )
    {
      return true;
    }

    static inline Object 
    objectIterator( const 
                    TypedObject<std::map<std::string, Object> > & object );

    static inline bool equals(
      const TypedObject< std::map<std::string, Object> > & o1,
      const GenericObject & o2 )
    {
      if( !o2.isDictionary() )
        return false;
      const std::map<std::string, Object> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      std::map<std::string, Object>::const_iterator it;
      Object value;
      for( it=self.begin(); it!=self.end(); ++it )
      {
        try
        {
          value = o2.getProperty( it->first );
          if( it->second != value )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }

  };


  inline
  Object 
  IterableImpl<std::map<std::string, Object>, false>::objectIterator
  ( const TypedObject<std::map<std::string, Object> > & object )
  {
    return Object::value( MapIterator<std::map<std::string, Object> >
                          ( object.getValue().begin(), 
                            object.getValue().end() ) );
  }


  // DictionaryInterface
  //---------------------------------------------------------------------------

  template<>
  inline bool 
  DictionaryImpl<std::map<std::string, Object>, false>::getProperty
  ( const TypedObject<std::map<std::string, Object> > & object, 
    const std::string & key, Object & result )
  {
    std::map<std::string, Object>::const_iterator 
      i = object.getValue().find( key );
    if( i == object.getValue().end() )
      return false;
    result = i->second;
    return true;
  }

  template<>
  inline void 
  DictionaryImpl<std::map<std::string, Object>, false>::setProperty
  ( TypedObject<std::map<std::string, Object> > & object,
    const std::string & key, Object value )
  {
    object.getValue()[ key ] = value;
  }


    //--------------//
   //  map<int,T>  //
  //--------------//


  // ArrayInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class ArrayImpl< std::map<int,T>, false >
  {
  public:

    static inline bool isArray( const TypedObject< std::map<int,T> > & )
    {
      return true;
    }

    static inline Object 
    getArrayItem( const TypedObject<std::map<int,T> > &object,
                  int index )
    {
      const std::map<int,T>	& val = object.getValue();
      typename std::map<int,T>::const_iterator	i = val.find( index );
      if( i == val.end() )
        return none(); // or throw ?
      // WARNING: const_cast is dangerous here !
      return Object::reference( const_cast<T &>( i->second ) );
    }

    static inline void setArrayItem( TypedObject<std::map<int,T> > &object, 
                                     int index, Object value )
    {
      object.getValue()[ index ] = value->GenericObject::value<T>();
    }

  };


  // DynArrayInterface
  //---------------------------------------------------------------------------
  template <typename T>
  class DynArrayImpl< std::map<int,T>, false >
  {
  public:
    static inline bool isDynArray( const TypedObject< std::map<int,T> > & )
    {
      return true;
    }

    static inline void 
    reserveArray( TypedObject< std::map<int,T> > &, size_t )
    {
    }

    static inline void
    resizeArray( TypedObject< std::map<int,T> > &, size_t )
    {
    }

    static inline void 
    removeArrayItem( TypedObject< std::map<int,T> > &object,
                     int index )
    {
      object.getValue().erase( index ); 
    }

    
    static inline void 
    insertArrayItem( TypedObject< std::map<int,T> > &object,
                     int index, Object value )
    {
      object.getValue()[ index ] = value->GenericObject::value<T>();
    }
  };


    //------------------//
   //  map<int,Object>  //
  //------------------//

  // ArrayInterface
  //---------------------------------------------------------------------------
  template<>
  inline Object 
  ArrayImpl< std::map<int,Object>, 
             false >::getArrayItem( const TypedObject<std::map<int,Object> > &
                                    object, int index )
  {
    const std::map<int,Object>	& val = object.getValue();
    std::map<int,Object>::const_iterator	i = val.find( index );
    if( i == val.end() )
      return none(); // or throw ?
    return i->second;
  }

  template<>
  inline void 
  ArrayImpl< std::map<int,Object>, 
             false >::setArrayItem( TypedObject<std::map<int,Object> > &
                                    object, int index, Object value )
  {
    object.getValue()[ index ] = value;
  }


  // DynArrayInterface
  //---------------------------------------------------------------------------
  template<>
  inline void 
  DynArrayImpl< std::map<int,Object>, false >::
  insertArrayItem( TypedObject< std::map<int,Object> > &object,
                   int index, Object value )
  {
    object.getValue()[ index ] = value;
  }


  // IterableInterface
  //---------------------------------------------------------------------------

  template <>
  class MapIterator< std::map<int, Object > > :
    public IntKeyIteratorInterface
  {
  public:
    inline MapIterator() { _iterator = _end; }

    bool isValid() const
    {
      return _iterator != _end;
    }

    Object currentValue() const
    {
      return _iterator->second;
    }

    void next()
    {
      ++_iterator;
    }

    long intKey() const
    {
      return _iterator->first;
    }

    inline
    MapIterator( const std::map< int, Object >::const_iterator &begin,
                 const std::map< int, Object >::const_iterator &end ) :
      IteratorInterface(), IntKeyIteratorInterface(),
     _iterator( begin ),
     _end( end ) {}

    std::map< int, Object >::const_iterator _iterator;
    std::map< int, Object >::const_iterator _end;
  };

  template <>
  class IterableImpl<std::map<int, Object>, false>
  {
  public:


    static inline bool isIterable( const
                                   TypedObject< std::map<int,
                                   Object> > & )
    {
      return true;
    }

    static inline Object
    objectIterator( const
                    TypedObject<std::map<int, Object> > & object );

    static inline bool equals(
      const TypedObject< std::map<int, Object> > & o1,
      const GenericObject & o2 )
    {
      const std::map<int, Object> & self = o1.getValue();
      if( self.size() != o2.size() )
        return false;
      std::map<int, Object>::const_iterator it;
      Object o2it = o2.objectIterator();
      if( !o2it || !o2it->isValid() || !o2it->isIntKeyIterator() )
        return false;
      for( it=self.begin(); it!=self.end(); ++it, o2it->next() )
      {
        if( !o2it || !o2it->isValid() )
          return false;
        try
        {
          if( it->first != o2it->intKey() )
            return false;
          if( it->second != o2it->currentValue() )
            return false;
        }
        catch( ... )
        {
          return false;
        }
      }
      return true;
    }

  };


  inline
  Object
  IterableImpl<std::map<int, Object>, false>::objectIterator
  ( const TypedObject<std::map<int, Object> > & object )
  {
    return Object::value( MapIterator<std::map<int, Object> >
                          ( object.getValue().begin(),
                            object.getValue().end() ) );
  }

/*

  template <typename T>
  class DictionaryImpl<std::map<int, T>, false>
  {
  public:

    static inline bool 
    getProperty( const TypedObject<std::map<int, T> > & object, 
                 const std::string & key, Object & result )
    {
      int kval;
      stringTo( key, kval );
      typename std::map<int, T>::const_iterator 
        i = object.getValue().find( kval );
      if( i == object.getValue().end() )
        return false;
      result = Object::reference( i->second );
      return true;
    }

    static inline void 
    setProperty( TypedObject<std::map<int, T> > & object,
                       const std::string & key, Object value )
    {
      T	& val = value->GenericObject::value<T>();
      int kval;
      stringTo( key, kval );
      object.getValue()[ kval ] = val;
    }

    static inline bool 
    removeProperty( TypedObject<std::map<int, T> > & object, 
                    const std::string & key )
    {
      int kval;
      stringTo( key, kval );
      return object.getValue().erase( kval );
    }

    static inline 
    bool hasProperty( const TypedObject<std::map<int, T> > & object, 
                      const std::string & key )
    {
      int kval;
      stringTo( key, kval );
      return object.getValue().find( kval ) != object.getValue().end();
    }

    static inline 
    void clearProperties( TypedObject<std::map<int, T> > & object )
    {
      object.getValue().clear();
    }
  };


    //-------------------//
   //  map<int,Object>  //
  //-------------------//


  template<>
  inline void 
  DictionaryImpl<std::map<int, Object>, false>::setProperty
  ( TypedObject<std::map<int, Object> > & object, 
    const std::string & key, Object value )
  {
    int kval;
    stringTo( key, kval );
    object.getValue()[ kval ] = value;
  }

*/
} // namespace interface_internal

#endif // DOXYGEN_HIDE_INTERNAL_CLASSES




#endif // ifndef CARTOBASE_OBJECT_OBJECT_INTERNALS_H
