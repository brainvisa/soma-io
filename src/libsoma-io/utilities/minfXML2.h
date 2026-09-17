#ifndef SOMAIO_UTILITIES_MINFXML2_H
#define SOMAIO_UTILITIES_MINFXML2_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/filedatasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
#include <cartobase/object/syntax.h>
#include <cartobase/type/string_conversion.h>
//----------------------------------------------------------------------------

namespace soma {


//----------------------------------------------------------------------------
// XML tags and attributes
//----------------------------------------------------------------------------


extern const std::string minfTag;
extern const std::string noneTag;
extern const std::string trueTag;
extern const std::string falseTag;
extern const std::string numberTag;
extern const std::string stringTag;
extern const std::string listTag;
extern const std::string dictionaryTag;
extern const std::string factoryTag;
extern const std::string xhtmlTag;
extern const std::string referenceTag;

extern const std::string expanderAttribute;
extern const std::string lengthAttribute;
extern const std::string nameAttribute;
extern const std::string objectTypeAttribute;
extern const std::string identifierAttribute;


//-----------------------------------------------------------------------------
carto::Object readDictionaryMinfXML( const std::string &fileName, 
                                     carto::rc_ptr<carto::SyntaxSet> syntax 
                                       = carto::rc_ptr<carto::SyntaxSet>() );
carto::Object readDictionaryMinfXML( DataSource &source, 
                                     carto::rc_ptr<carto::SyntaxSet> syntax 
                                       = carto::rc_ptr<carto::SyntaxSet>() );
void readDictionaryMinfXML( const std::string &fileName, 
                            carto::Object &dictionary, 
                            carto::rc_ptr<carto::SyntaxSet> syntax 
                              = carto::rc_ptr<carto::SyntaxSet>() );
void readDictionaryMinfXML( DataSource &source, 
                            carto::Object &dictionary, 
                            carto::rc_ptr<carto::SyntaxSet> syntax 
                              = carto::rc_ptr<carto::SyntaxSet>() );
/// just to force initializing libxml2, needs to be called once from the
/// main thread in a multi-threaded environment.
/// Called from PluginLoader::load
void init_libXML();


//----------------------------------------------------------------------------
class ObjectListener : public virtual carto::RCObject
{
public:
  virtual ~ObjectListener();

  virtual void nextObject( const carto::Object & ) = 0;
  virtual void noMoreObject() = 0;
};


//----------------------------------------------------------------------------
class MinfTreeExpander;
class MinfNodeExpander : public virtual carto::RCObject
{
public:
  virtual ~MinfNodeExpander();

  virtual std::string startStructure( MinfTreeExpander &,
                               const std::string &nodeType,
                               const std::string &identifier,
                               carto::Object attributes,
                               const std::string &parentSyntax=std::string(), 
                               const std::string &name=std::string() ) = 0;
  virtual void endStructure( MinfTreeExpander &, const std::string &nodeType ) = 0;
  virtual void atom( MinfTreeExpander &, carto::Object, const std::string &identifier  ) = 0;
  virtual void reference( MinfTreeExpander &, const std::string &identifier ) = 0;
};


//----------------------------------------------------------------------------
class MinfTreeExpander : public virtual carto::RCObject
{
public:
  MinfTreeExpander();
  virtual ~MinfTreeExpander();

  void push( carto::rc_ptr< MinfNodeExpander > );
  carto::rc_ptr< MinfNodeExpander > pop();

  void addReference( const std::string &identifier, carto::Object value );
  carto::Object getReferencedObject( const std::string &identifier );

  virtual std::string startStructure( const std::string &nodeType,
                               const std::string &identifier,
                               carto::Object attributes,
                               const std::string &parentSyntax=std::string(), 
                               const std::string &name=std::string() );
  virtual void endStructure( const std::string &nodeType );
  virtual void atom( carto::Object, const std::string &identifier  );
  virtual void reference( const std::string &identifier );

private:
  std::list< carto::rc_ptr< MinfNodeExpander > > _stack;
  std::map< std::string, carto::Object > _references;
};


//----------------------------------------------------------------------------
class DefaultMinfNodeExpander : public MinfNodeExpander
{
public:
  DefaultMinfNodeExpander( carto::rc_ptr< ObjectListener > ol 
                             = carto::rc_ptr< ObjectListener >(),
                           carto::rc_ptr<carto::SyntaxSet> syntax 
                             = carto::rc_ptr<carto::SyntaxSet>() );
  DefaultMinfNodeExpander( carto::rc_ptr<carto::SyntaxSet> syntax );
  virtual ~DefaultMinfNodeExpander();

  virtual std::string startStructure( MinfTreeExpander &,
                               const std::string &nodeType,
                               const std::string &identifier,
                               carto::Object attributes,
                               const std::string &parentSyntax=std::string(), 
                               const std::string &name=std::string() );
  virtual void endStructure( MinfTreeExpander &, const std::string &nodeType );
  virtual void atom( MinfTreeExpander &, carto::Object, const std::string &identifier  );
  virtual void reference( MinfTreeExpander &, const std::string &identifier );

private:

  carto::rc_ptr< ObjectListener > _objectListener;

protected:

  carto::rc_ptr<carto::SyntaxSet> _syntax;
};


//----------------------------------------------------------------------------
class SingleDictionaryExpander : public DefaultMinfNodeExpander
{
public:
  SingleDictionaryExpander( carto::Object dictionary, 
                            carto::rc_ptr<carto::SyntaxSet> syntax );
  virtual ~SingleDictionaryExpander();

  virtual std::string startStructure( MinfTreeExpander &,
                               const std::string &nodeType,
                               const std::string &identifier,
                               carto::Object attributes,
                               const std::string &parentSyntax=std::string(), 
                               const std::string &name=std::string() );

private:

  carto::Object _dictionary;
};


//----------------------------------------------------------------------------
class ListBuilder : public ObjectListener
{
public:
  ListBuilder();
  ListBuilder( carto::Object );
  ListBuilder( carto::rc_ptr< ObjectListener > );
  ListBuilder( carto::Object, carto::rc_ptr< ObjectListener > );
  virtual ~ListBuilder();

  virtual void nextObject( const carto::Object & );
  virtual void noMoreObject();

  carto::Object result;
private:

  carto::rc_ptr< ObjectListener > _objectListener;
};


//----------------------------------------------------------------------------
class DictionaryBuilder : public ObjectListener
{
public:
  DictionaryBuilder();
  DictionaryBuilder( carto::Object );
  DictionaryBuilder( carto::rc_ptr< ObjectListener > );
  DictionaryBuilder( carto::Object, carto::rc_ptr< ObjectListener > );
  virtual ~DictionaryBuilder();

  virtual void nextObject( const carto::Object & );
  virtual void noMoreObject();

  carto::Object result;

private:

  carto::Object _key;
  carto::rc_ptr< ObjectListener > _objectListener;
};



//----------------------------------------------------------------------------
class MinfXMLReader : public virtual carto::RCObject
{
public:
  MinfXMLReader( MinfTreeExpander &, carto::rc_ptr<carto::SyntaxSet> syntax );
  void read( const std::string & );
  void read( DataSource & );

private:

  MinfTreeExpander &_expander;
  carto::rc_ptr< carto::SyntaxSet > _syntax;
};


//----------------------------------------------------------------------------
// class TestMinfTreeExpander : public virtual MinfNodeExpander
// {
// public:
//   TestMinfTreeExpander();
//   virtual ~TestMinfTreeExpander();
// 
//   virtual void startStructure( MinfTreeExpander &,
//                                const std::string &nodeType,
//                                const std::string &identifier,
//                                carto::Object attributes );
//   virtual void endStructure( MinfTreeExpander &, const std::string &nodeType );
//   virtual void atom( MinfTreeExpander &, carto::Object, const std::string &identifier  );
//   virtual void reference( MinfTreeExpander &, const std::string &identifier );
// 
// private:
// 
//   int _level;
// };

} // namespace soma

#endif // ifndef SOMAIO_UTILITIES_MINFXML2_H
