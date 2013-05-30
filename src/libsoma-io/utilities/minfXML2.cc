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

//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/utilities/minfXML2.h>
//#include <soma-io/writer/pythonwriter.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/exception/ioexcept.h>
#include <cartobase/object/property.h>
#include <cartobase/object/object_factory.h>
//--- system -------------------------------------------------------------------
#include <libxml/xmlreader.h>
//------------------------------------------------------------------------------


using namespace std;

namespace soma {


//-----------------------------------------------------------------------------
// XML tags and attributes
//-----------------------------------------------------------------------------


const std::string minfTag( "minf" );
const std::string noneTag( "none" );
const std::string trueTag( "true" );
const std::string falseTag( "false" );
const std::string numberTag( "n" );
const std::string stringTag( "s" );
const std::string listTag( "l" );
const std::string dictionaryTag( "d" );
const std::string factoryTag( "f" );
const std::string xhtmlTag( "xhtml" );
const std::string referenceTag( "ref" );

const std::string expanderAttribute( "expander" );
const std::string lengthAttribute( "length" );
const std::string nameAttribute( "name" );
const std::string objectTypeAttribute( "type" );
const std::string identifierAttribute( "identifier" );


//-----------------------------------------------------------------------------
carto::Object readDictionaryMinfXML( const std::string &fileName, 
                                     carto::rc_ptr<carto::SyntaxSet> syntax )
{
  FileDataSource ds( fileName );
  ds.open( DataSource::Read );
  return readDictionaryMinfXML( ds, syntax );
}


carto::Object readDictionaryMinfXML( DataSource &ds, 
                                     carto::rc_ptr<carto::SyntaxSet> syntax )
{
  carto::Object dictionary( carto::Object::value( carto::PropertySet() ) );
  readDictionaryMinfXML( ds, dictionary, syntax );
  return dictionary;
}


void readDictionaryMinfXML( const std::string &fileName, 
                            carto::Object &dictionary, 
                            carto::rc_ptr<carto::SyntaxSet> syntax )
{
  FileDataSource ds( fileName );
  ds.open( DataSource::Read );
  readDictionaryMinfXML( ds, dictionary, syntax );
}


void readDictionaryMinfXML( DataSource &ds, carto::Object &dictionary, 
                            carto::rc_ptr<carto::SyntaxSet> syntax )
{
  MinfTreeExpander treeExpander;
  treeExpander.push( carto::rc_ptr<MinfNodeExpander>( new SingleDictionaryExpander( dictionary, syntax ) ) );
  MinfXMLReader reader( treeExpander, syntax );
  reader.read( ds );
}


namespace
{

  int readDataSourceCallback( void * context, 
                              char * buffer, 
                              int len )
  {
    // cout << "readDataSourceCallback " << (void *) buffer << endl;
    DataSource	*ds = (DataSource *) context;
    long x = ds->readBlock( buffer, len );
    // cout << "read " << x << " / " << len << " bytes\n";
    return (int) x;
  }


  int closeDataSourceCallback( void * )
  {
    // cout << "closeDataSourceCallback\n";
    return 0;
  }


  void errorxml( void *, const char *, xmlParserSeverities, 
                 xmlTextReaderLocatorPtr )
  {
    // don't print anything
  }

  string readTextContent( xmlTextReaderPtr reader )
  {
    string result;
    int ret = xmlTextReaderRead( reader );
    while ( ret == 1 ) {
      int type = xmlTextReaderNodeType( reader );
      if( type == XML_READER_TYPE_TEXT || type == XML_READER_TYPE_SIGNIFICANT_WHITESPACE ) {
        result += reinterpret_cast<const char *>( xmlTextReaderConstValue( reader ) );
      } else if( type == XML_READER_TYPE_END_ELEMENT ) {
        break;
      }
      ret = xmlTextReaderRead( reader );
    }
    return result;
  }


  bool ignoreElement( xmlTextReaderPtr reader, const string &elementName )
  {
    if ( xmlTextReaderIsEmptyElement( reader ) ) return false;
    while( xmlTextReaderRead( reader ) == 1 ) {
      int type = xmlTextReaderNodeType( reader );
      if( type == XML_READER_TYPE_END_ELEMENT ) {
        string endElementName( reinterpret_cast<const char *>( xmlTextReaderConstName( reader ) ) );
        if ( elementName == endElementName ) return false;
        return true;
      }
      if( type == XML_READER_TYPE_ELEMENT ) return true;
    }
    return false;
  }

} // namespace


//-----------------------------------------------------------------------------
ObjectListener::~ObjectListener() {}


//-----------------------------------------------------------------------------
MinfNodeExpander::~MinfNodeExpander() {}


//-----------------------------------------------------------------------------
MinfTreeExpander::MinfTreeExpander()
{}


MinfTreeExpander::~MinfTreeExpander()
{}


void MinfTreeExpander::push( carto::rc_ptr< MinfNodeExpander > expander )
{
  _stack.push_back( expander );
}


carto::rc_ptr< MinfNodeExpander > MinfTreeExpander::pop()
{
  carto::rc_ptr< MinfNodeExpander > result = _stack.back();
  _stack.pop_back();
  return result;
}


void MinfTreeExpander::addReference( const std::string &identifier, 
                                     carto::Object value )
{
  _references[ identifier ] = value;
}


carto::Object MinfTreeExpander::getReferencedObject( const std::string &identifier )
{
  return _references[ identifier ];
}


string MinfTreeExpander::startStructure( const std::string &nodeType,
  const std::string &identifier,
  carto::Object attributes,
  const string &parentSyntax, 
  const string & name )
{
  if( _stack.empty() )
    throw carto::parse_error( "XML parsing error", identifier, name, 0 );
  return _stack.back()->startStructure( *this, nodeType, identifier, attributes, parentSyntax, name );
}


void MinfTreeExpander::endStructure( const std::string &nodeType )
{
  _stack.back()->endStructure( *this, nodeType );
}


void MinfTreeExpander::atom( carto::Object o, const std::string &identifier  )
{
  _stack.back()->atom( *this, o, identifier );
}


void MinfTreeExpander::reference( const std::string &identifier )
{
  _stack.back()->reference( *this, identifier );
}


//-----------------------------------------------------------------------------
DefaultMinfNodeExpander::DefaultMinfNodeExpander( carto::rc_ptr< ObjectListener > ol, 
                                                  carto::rc_ptr<carto::SyntaxSet> syntax ) :
  _objectListener( ol ),
  _syntax( syntax )
{}


DefaultMinfNodeExpander::DefaultMinfNodeExpander( carto::rc_ptr<carto::SyntaxSet> syntax ) :
  _syntax( syntax )
{}


DefaultMinfNodeExpander::~DefaultMinfNodeExpander()
{}


string DefaultMinfNodeExpander::startStructure( MinfTreeExpander &expander,
                                                const std::string &nodeType,
                                                const std::string &,
                                                carto::Object /*attributes*/,
                                                const string &parentSyntax, 
                                                const string &name )
{
  carto::Object targetObject;
  string structureType;
  if ( !_syntax.isNull() ) {
    carto::SyntaxSet::const_iterator it( _syntax->find( parentSyntax ) );
    if ( it != _syntax->end() ) {
      carto::Syntax::const_iterator it2 = it->second.find( name );
      if ( it2 != it->second.end() ) {
        targetObject = carto::ObjectFactory::createObject( it2->second.type );
        structureType = it2->second.type;
        if ( ! targetObject.isNull() ) {
          carto::SyntaxedInterface *si( targetObject->getInterface<carto::SyntaxedInterface>() );
          if ( si ) {
            structureType = si->getSyntax();
          }
        }
      }
    }
  }

  if ( nodeType == listTag ) {
    carto::rc_ptr< ObjectListener > listBuilder;
    if ( targetObject.isNull() ) {
      structureType = "__generic__";
      listBuilder.reset( new ListBuilder( _objectListener ) );
    } else {
      listBuilder.reset( new ListBuilder( targetObject, _objectListener ) );
    }
    expander.push(
      carto::rc_ptr<MinfNodeExpander>(
        new DefaultMinfNodeExpander( 
          carto::rc_ptr< ObjectListener >( listBuilder )
    ) ) );
  }
  else if ( nodeType == dictionaryTag ) {
    carto::rc_ptr< ObjectListener > dictBuilder;
    if ( targetObject.isNull() ) {
      structureType = "__generic__";
      dictBuilder.reset( new DictionaryBuilder( _objectListener ) );
    } else {
      dictBuilder.reset( new DictionaryBuilder( targetObject, _objectListener ) );
    }
    expander.push(
      carto::rc_ptr<MinfNodeExpander>(
        new DefaultMinfNodeExpander( 
          carto::rc_ptr< ObjectListener >( dictBuilder )
    ) ) );
  }
  else if ( nodeType == factoryTag ) {
    expander.push(
      carto::rc_ptr<MinfNodeExpander>(
        new DefaultMinfNodeExpander( 
          carto::rc_ptr< ObjectListener >( new ListBuilder( _objectListener ) )
    ) ) );
  }
  else
  {
    throw carto::format_error( string( "unrecognized XML tag" ) + nodeType, "" );
  }

  return structureType;
}


void DefaultMinfNodeExpander::endStructure( MinfTreeExpander &expander, 
                                            const std::string & )
{
  if ( ! _objectListener.isNull() ) _objectListener->noMoreObject();
  expander.pop();
}


void DefaultMinfNodeExpander::atom( MinfTreeExpander &expander, 
                                    carto::Object o, 
                                    const std::string &identifier  )
{
  if ( ! identifier.empty() ) {
    expander.addReference( identifier, o );
  }
  if ( ! _objectListener.isNull() ) _objectListener->nextObject( o );
}


void DefaultMinfNodeExpander::reference( MinfTreeExpander &expander, 
                                         const std::string &identifier )
{
  if ( ! _objectListener.isNull() ) _objectListener->nextObject( expander.getReferencedObject( identifier ) );
}


//-----------------------------------------------------------------------------
SingleDictionaryExpander::SingleDictionaryExpander( carto::Object dictionary, 
                                                    carto::rc_ptr<carto::SyntaxSet> syntax ) : 
  DefaultMinfNodeExpander( syntax ),
  _dictionary( dictionary )
{}


SingleDictionaryExpander::~SingleDictionaryExpander()
{}


string SingleDictionaryExpander::startStructure( MinfTreeExpander &expander,
                                                 const std::string &nodeType,
                                                 const std::string &,
                                                 carto::Object,
                                                 const string &, 
                                                 const string & )
{
  if ( nodeType == dictionaryTag ) {
    expander.push(
      carto::rc_ptr<MinfNodeExpander>(
        new DefaultMinfNodeExpander( 
          carto::rc_ptr< ObjectListener >( new DictionaryBuilder( _dictionary ) ),
          _syntax
    ) ) );
  }
  return "__generic__";
}



//-----------------------------------------------------------------------------
ListBuilder::ListBuilder()
{
  result = carto::Object::value( vector< carto::Object >() );
}


ListBuilder::ListBuilder( carto::Object r ) : result( r )
{
}


ListBuilder::ListBuilder( carto::rc_ptr< ObjectListener > ol ) : _objectListener( ol )
{
  result = carto::Object::value( vector< carto::Object >() );
}


ListBuilder::ListBuilder( carto::Object r, carto::rc_ptr< ObjectListener > ol ) :
  result( r ),
  _objectListener( ol )
{
}


ListBuilder::~ListBuilder()
{}


void ListBuilder::nextObject( const carto::Object &o )
{
  result->insertArrayItem( result->size(), o );
}


void ListBuilder::noMoreObject()
{
  if ( ! _objectListener.isNull() ) _objectListener->nextObject( result );
}


//-----------------------------------------------------------------------------
DictionaryBuilder::DictionaryBuilder()
{
  result = carto::Object::value( carto::PropertySet() );
}


DictionaryBuilder::DictionaryBuilder( carto::Object r ) : result( r )
{
}


DictionaryBuilder::DictionaryBuilder( carto::rc_ptr< ObjectListener > ol ) : _objectListener( ol )
{
  result = carto::Object::value( carto::PropertySet() );
}


DictionaryBuilder::DictionaryBuilder( carto::Object r, carto::rc_ptr< ObjectListener > ol ) :
  result( r ),
  _objectListener( ol )
{
}


DictionaryBuilder::~DictionaryBuilder()
{}


void DictionaryBuilder::nextObject( const carto::Object &o )
{
  if ( _key.isNull() ) {
    _key = o;
  } else {
    result->setProperty( _key->getString(), o );
    _key.reset( NULL );
  }
}


void DictionaryBuilder::noMoreObject()
{
  if ( ! _objectListener.isNull() ) _objectListener->nextObject( result );
}



//-----------------------------------------------------------------------------
MinfXMLReader::MinfXMLReader( MinfTreeExpander &expander, carto::rc_ptr<carto::SyntaxSet> syntax ) :
  _expander( expander ),
  _syntax( syntax )
{}


void MinfXMLReader::read( const std::string &fileName )
{
  FileDataSource ds( fileName );
  ds.open( DataSource::Read );
  read( ds );
}


void MinfXMLReader::read( DataSource &ds )
{
  if ( ! ds.isOpen() ) {
    ds.open( DataSource::Read );
    if ( ! ds.isOpen() ) throw carto::open_error( "cannot open", ds.url() );
  }

  xmlTextReaderPtr reader;


  xmlParserInputBufferPtr buf = 
    xmlParserInputBufferCreateIO( readDataSourceCallback, 
                                  closeDataSourceCallback, 
                                  &ds, XML_CHAR_ENCODING_UTF8 );
  // "save" the random seed
  int rseed = rand();
  reader = xmlNewTextReader( buf, ds.url().c_str() );

  if( reader != NULL ) {
    xmlTextReaderSetErrorHandler( reader, errorxml, 0 );
    try {
      list< pair<bool,string> > stack;
      bool forceObjectNaming = false;
      int readResult = xmlTextReaderRead( reader ); 
      string parentSyntax;
      while ( readResult == 1 ) {
        int type = xmlTextReaderNodeType( reader );
        if ( type == XML_READER_TYPE_ELEMENT ) {
          string elementName( reinterpret_cast<const char *>( xmlTextReaderConstName( reader ) ) );
          carto::Object attributes = carto::Object::value( carto::PropertySet() );
          string identifier;
          string name;
          while( xmlTextReaderMoveToNextAttribute( reader ) ) {
            string attributeName( reinterpret_cast<const char *>( xmlTextReaderConstName( reader ) ) );
            string attributeValue( reinterpret_cast<const char *>( xmlTextReaderConstValue( reader ) ) );
            if ( attributeName == nameAttribute ) {
              name = attributeValue;
            } else if ( attributeName == identifierAttribute ) {
              identifier = attributeValue;
            } else {
              attributes->setProperty( attributeName, attributeValue );
            }
          }

          if ( name.empty() ) {
            if ( forceObjectNaming ) _expander.atom( carto::Object(), string() );
          } else {
            _expander.atom( carto::Object::value( name ), string() );
          }

          carto::Object targetObject;
          string structureType;
          if ( !_syntax.isNull() ) {
            carto::SyntaxSet::const_iterator it( _syntax->find( parentSyntax ) );
            if ( it != _syntax->end() ) {
              carto::Syntax::const_iterator it2 = it->second.find( name );
              if ( it2 != it->second.end() ) {
                targetObject = carto::ObjectFactory::createObject( it2->second.type );
              }
            }
          }


          if ( elementName == stringTag ) {
            _expander.atom( carto::Object::value( readTextContent( reader ) ), identifier );

          } else if ( elementName == numberTag ) {
            double value;
            carto::stringTo( readTextContent( reader ), value );
            if( targetObject.isNull() ) {
              targetObject = carto::Object::value( value );
            } else {
              targetObject->setScalar( value );
            }
            _expander.atom( targetObject, identifier );

          } else if ( elementName == noneTag ) {
            _expander.atom( carto::Object(), identifier );
            if ( ignoreElement( reader, elementName ) ) continue;

          } else if ( elementName == trueTag ) {
            _expander.atom( carto::Object::value( true ), identifier );
            if ( ignoreElement( reader, elementName ) ) continue;

          } else if ( elementName == falseTag ) {
            _expander.atom( carto::Object::value( false ), identifier );
            if ( ignoreElement( reader, elementName ) ) continue;

          } else if ( elementName == referenceTag ) {
            _expander.reference( attributes->getProperty( identifierAttribute )->getString() );
            if ( ignoreElement( reader, elementName ) ) continue;

          } else if ( elementName == factoryTag ) {
            stack.push_back( pair<bool, string>( forceObjectNaming, parentSyntax ) );
            forceObjectNaming = true;
            parentSyntax = 
            _expander.startStructure( elementName, identifier, attributes, parentSyntax, name );

          } else {
            stack.push_back( pair<bool, string>( forceObjectNaming, parentSyntax ) );
            forceObjectNaming = false;
            parentSyntax = _expander.startStructure( elementName, identifier, attributes, parentSyntax, name );
          }
        } else if ( type == XML_READER_TYPE_END_ELEMENT ) {
          string elementName( reinterpret_cast<const char *>( xmlTextReaderConstName( reader ) ) );
          _expander.endStructure( elementName );
          forceObjectNaming = stack.back().first;
          parentSyntax = stack.back().second;
          stack.pop_back();
        }
        readResult = xmlTextReaderRead( reader );
      }
      xmlFreeTextReader( reader );
      xmlFreeParserInputBuffer( buf );
      // "restore" the random seed
      srand( rseed );
    }
    catch( carto::format_error & e )
    {
      xmlFreeTextReader( reader );
      xmlFreeParserInputBuffer( buf );
      // "restore" the random seed
      srand( rseed );
      ostringstream s;
      s << e.what() << " at position " << ds.at();
      throw carto::format_error( s.str(), ds.url() );
    }
    catch( ... ) {
      xmlFreeTextReader( reader );
      xmlFreeParserInputBuffer( buf );
      // "restore" the random seed
      srand( rseed );
      throw;
    }
  } else {
    xmlFreeParserInputBuffer( buf );
    // "restore" the random seed
    srand( rseed );
    throw carto::open_error( "unable to open", ds.url() );
  }
}


//-----------------------------------------------------------------------------
// TestMinfTreeExpander::TestMinfTreeExpander() : _level( 0 ) {}
// 
// 
// TestMinfTreeExpander::~TestMinfTreeExpander()  {}
// 
// 
// void TestMinfTreeExpander::startStructure( MinfTreeExpander &,
//   const std::string &nodeType,
//   const std::string &,
//   carto::Object attributes )
// {
//   string indent;
//   for( int i = _level; i > 0; indent += "  ", --i );
//   cout << indent << "<" << nodeType;
//   if ( attributes->size() ) {
//     for( carto::Object it = attributes->objectIterator(); it->isValid(); it->next() ) {
//       cout << " " << it->key() << "=\"" << it->currentValue()->getString() << "\"";
//     }
//   }
//   cout << ">" << endl;
//   ++_level;
// }
// 
// void TestMinfTreeExpander::endStructure( MinfTreeExpander &, const std::string &nodeType )
// {
//   --_level;
//   string indent;
//   for( int i = _level; i > 0; indent += "  ", --i );
//   cout << indent << "</" << nodeType << ">" << endl;
// }
// void TestMinfTreeExpander::atom( MinfTreeExpander &, carto::Object o, const std::string &  )
// {
//   string indent;
//   for( int i = _level; i > 0; indent += "  ", --i );
//   cout << indent << "atom ";
//   if ( o.isNull() ) {
//     cout << ": None" << endl;
//   } else {
//     cout << "(" << o->type() << ") : ";
//     PythonWriter pw;
//     pw.attach( cout );
//     pw.write( o, false, false );
//   }
// }
// 
// void TestMinfTreeExpander::reference( MinfTreeExpander &, const std::string &identifier )
// {
//   string indent;
//   for( int i = _level; i > 0; indent += "  ", --i );
//   cout << indent << "reference : " << identifier << endl;
// }

void init_libXML()
{
  const char* text = "<?xml version=\"1.0\" encoding=\"utf-8\" ?><toto></toto>";
  xmlTextReaderPtr reader;
  reader = xmlReaderForMemory( text, 52, "", 0, 0 );
  xmlFreeTextReader( reader );
}


} // namespace soma
