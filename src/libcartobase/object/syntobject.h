#ifndef CARTOBASE_OBJECT_SYNTOBJECT_H
#define CARTOBASE_OBJECT_SYNTOBJECT_H

#include <cartobase/object/object.h>
#include <cartobase/object/syntax.h>
#include <cartobase/object/property.h>


namespace carto
{

  class PropertySet;

  namespace internal
  {

    template<typename T> class SyntaxedNonInterfaceObject 
      : public ValueObject<T>, public SyntaxedInterface
    {
    public:
      SyntaxedNonInterfaceObject( const std::string& s );
      SyntaxedNonInterfaceObject( const SyntaxedNonInterfaceObject<T> & );
      virtual ~SyntaxedNonInterfaceObject();

      virtual bool hasSyntax() const;
      virtual std::string getSyntax() const;
      virtual void setSyntax( const std::string& syntactic );

    private:
      std::string _syntactic;
    };


    template <typename T>
    class SyntaxedInterfaceType
      : public T, public SyntaxedInterface
    {
    public:
      SyntaxedInterfaceType( const std::string & s = "" );
      SyntaxedInterfaceType( const SyntaxedInterfaceType & );
      virtual ~SyntaxedInterfaceType();

      virtual bool hasSyntax() const;
      virtual std::string getSyntax() const;
      virtual void setSyntax( const std::string& syntactic );

    private:
      std::string _syntactic;
    };


    template <typename T>
    class SyntaxedInterfaceObject
      : public ValueObject<SyntaxedInterfaceType<T> >
    {
    public:
      SyntaxedInterfaceObject( const std::string & s ) 
        : ValueObject<SyntaxedInterfaceType<T> >()
      { this->getValue().setSyntax( s ); }
      virtual bool hasSyntax() const { return true; }
      virtual std::string getSyntax() const
      { return const_cast<SyntaxedInterfaceObject *>( this )->
          getValue().getSyntax(); }
      virtual void setSyntax( const std::string & s ) 
      { this->getValue().setSyntax( s ); }
    };


    template <typename T, bool x> 
    class SyntaxedObjectSwitch
    {
    };


    template <typename T> 
    class SyntaxedObjectSwitch<T, true>
    {
    public:
      typedef SyntaxedInterfaceObject<T> ObjectType;
      typedef SyntaxedInterfaceType<T> ContentType;
    };


    template <typename T> 
    class SyntaxedObjectSwitch<T, false>
    {
    public:
      typedef SyntaxedNonInterfaceObject<T> ObjectType;
      typedef T ContentType;
    };

  } // namespace internal


  template <typename T> 
  class SyntaxedObject 
    : public internal::SyntaxedObjectSwitch<T, SUPERSUBCLASS(Interface, T)>::
    ObjectType
  {
  public:
    SyntaxedObject( const std::string & s ) 
      : internal::SyntaxedObjectSwitch<T, SUPERSUBCLASS(Interface, T)>::
      ObjectType( s )
      {}
    SyntaxedObject( const SyntaxedObject & x ) 
      : internal::SyntaxedObjectSwitch<T, SUPERSUBCLASS(Interface, T)>::
      ObjectType( x )
      {}
    typedef typename internal::SyntaxedObjectSwitch<T, 
      SUPERSUBCLASS(Interface, T)>::ContentType ContentType;
  };


  // definitions

  namespace internal
  {

    template<typename T> inline 
    SyntaxedNonInterfaceObject<T>::SyntaxedNonInterfaceObject
    ( const std::string & s )
      : ValueObject<T>(), _syntactic( s )
    {
    }

    template<typename T> inline 
    SyntaxedNonInterfaceObject<T>::SyntaxedNonInterfaceObject
    ( const SyntaxedNonInterfaceObject & x )
      : RCObject(), 
        Interface(), 
        StringInterface(), 
        ScalarInterface(), 
        SizeInterface(), 
        IterableInterface(), 
        ArrayInterface(), 
        DynArrayInterface(), 
        DictionaryInterface(), 
        IteratorInterface(), 
        DictionaryIteratorInterface(), 
        ValueObject<T>( x ), 
        SyntaxedInterface(), 
        _syntactic( x._syntactic )
    {
    }

    template<typename T> inline 
    bool SyntaxedNonInterfaceObject<T>::hasSyntax() const
    {
      return true;
    }

    template<typename T> inline 
    std::string SyntaxedNonInterfaceObject<T>::getSyntax() const
    {
      return _syntactic;
    }

    template<typename T> inline 
    void SyntaxedNonInterfaceObject<T>::setSyntax
    ( const std::string& syntactic )
    {
      _syntactic = syntactic;
    }

    //

    template<typename T> inline 
    SyntaxedInterfaceType<T>::SyntaxedInterfaceType
    ( const SyntaxedInterfaceType< T > & x )
      : Interface(),
        T( x ),
        SyntaxedInterface(),
        _syntactic( x._syntactic )
    {
    }

    template <>
    inline
    SyntaxedInterfaceType<PropertySet>::SyntaxedInterfaceType(
                                const SyntaxedInterfaceType< PropertySet > & x )
      : RCObject(),
        Interface(),
        PropertySet( x ),
        SyntaxedInterface(),
        _syntactic( x._syntactic )
    {
    }

    template<typename T> inline 
    SyntaxedInterfaceType<T>::SyntaxedInterfaceType( const std::string & s )
      : T(), SyntaxedInterface(), _syntactic( s )
    {
    }

    template<typename T> inline 
    bool SyntaxedInterfaceType<T>::hasSyntax() const
    {
      return true;
    }

    template<typename T> inline 
    std::string SyntaxedInterfaceType<T>::getSyntax() const
    {
      return _syntactic;
    }

    template<typename T> inline 
    void SyntaxedInterfaceType<T>::setSyntax( const std::string& syntactic )
    {
      _syntactic = syntactic;
    }

    extern template class SyntaxedNonInterfaceObject<Dictionary>;
    extern template class SyntaxedInterfaceType<PropertySet>;
    extern template class SyntaxedInterfaceObject<PropertySet>;
  }

  DECLARE_GENERIC_OBJECT_TYPE( internal::SyntaxedInterfaceType<PropertySet> )
}

namespace carto
{

  template <>
  inline 
  SyntaxedObject<PropertySet>::SyntaxedObject(
                                         const SyntaxedObject<PropertySet> & x ) 
    : RCObject(),
      Interface(),
      StringInterface(),
      ScalarInterface(),
      SizeInterface(),
      IterableInterface(),
      ArrayInterface(),
      DynArrayInterface(),
      DictionaryInterface(),
      IteratorInterface(),
      KeyIteratorInterface(),
      DictionaryIteratorInterface(),
      IntKeyIteratorInterface(),
      NoneInterface(),
      internal::SyntaxedObjectSwitch<PropertySet,
                        SUPERSUBCLASS(Interface, PropertySet)>::ObjectType( x )
  {
  }

}

#endif
