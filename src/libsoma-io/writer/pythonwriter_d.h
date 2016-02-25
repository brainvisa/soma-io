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

#ifndef SOMAIO_WRITER_PYTHONWRITER_D_H
#define SOMAIO_WRITER_PYTHONWRITER_D_H

//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
#include <soma-io/writer/pythonwriter.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//----------------------------------------------------------------------------

namespace {

template<typename T> 
void 
genericHelper( const carto::GenericObject & obj, soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  // using carto::GenericObject:: avoids a bug in gcc-2.96
  const T	& x = obj.GenericObject::value<T>();
  soma::AsciiDataSourceTraits<T>::write( ds, x );
}


template<>
void genericHelper<char>( const carto::GenericObject & obj, soma::PythonWriter & w, int,
                          bool )
{
  soma::DataSource	& ds = *w.dataSource();
  // using carto::GenericObject:: avoids a bug in gcc-2.96
  const char	& x = obj.GenericObject::value<char>();
  soma::AsciiDataSourceTraits<int>::write( ds, x );
}


template<>
void genericHelper<unsigned char>( const carto::GenericObject & obj,
                                   soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  // using carto::GenericObject:: avoids a bug in gcc-2.96
  const unsigned char	& x = obj.GenericObject::value<unsigned char>();
  soma::AsciiDataSourceTraits<unsigned>::write( ds, x );
}


template<>
void genericHelper<std::string>( const carto::GenericObject & obj,
                                 soma::PythonWriter & w, int, bool )
{
  w.writeString( *w.dataSource(), obj.getString() );
}


template<>
void genericHelper<double>( const carto::GenericObject & obj,
                            soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  soma::AsciiDataSourceTraits<double>::write( ds, obj.getScalar() );
}


template<>
void genericHelper<float>( const carto::GenericObject & obj,
                           soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  soma::AsciiDataSourceTraits<float>::write( ds, float( obj.getScalar() ) );
}


template<>
void genericHelper<int16_t>( const carto::GenericObject & obj,
                             soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  soma::AsciiDataSourceTraits<int16_t>::write(
    ds, int16_t( obj.getScalar() ) );
}


template<>
void genericHelper<uint16_t>( const carto::GenericObject & obj,
                              soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  soma::AsciiDataSourceTraits<uint16_t>::write(
    ds, uint16_t( obj.getScalar() ) );
}


template<>
void genericHelper<int32_t>( const carto::GenericObject & obj,
                             soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  soma::AsciiDataSourceTraits<int32_t>::write(
    ds, int32_t( obj.getScalar() ) );
}


template<>
void genericHelper<uint32_t>( const carto::GenericObject & obj,
                              soma::PythonWriter & w, int, bool )
{
  soma::DataSource	& ds = *w.dataSource();
  soma::AsciiDataSourceTraits<uint32_t>::write(
    ds, uint32_t( obj.getScalar() ) );
}


template<typename T> 
void dictHelper( const carto::GenericObject & obj, soma::PythonWriter & w, int indent,
		 bool writeInternals )
{
  // cout << "dictHelper<" << DataTypeCode<T>::name() << ">\n";

  soma::DataSource				& ds = *w.dataSource();
  const std::map<T, carto::Object>
    & x = obj.GenericObject::value<std::map<T, carto::Object> >();
  typename std::map<T, carto::Object>::const_iterator	im, em = x.end();
  bool					first = true, hassyntax = false;
  std::string				ind, ind2;
  int					i;
  char					sep = '\n';

  if( w.singleLine() )
    sep = ' ';
  else
    {
      for( i=0; i<indent-1; ++i )
	ind += "    ";
      ind2 = ind + "  ";
      if( indent > 0 )
        ind += "    ";
      ++indent;
    }

  ds.putch( '{' );
  ds.putch( sep );
  soma::AsciiDataSourceTraits<std::string>::write( ds, ind );

  std::string	synt;
  const carto::SyntaxedInterface *si
    = obj.GenericObject::getInterface<carto::SyntaxedInterface>();
  if( si && si->hasSyntax() )
    {
      synt = si->getSyntax();
      hassyntax = true;
    }
  else
    {
      try
        {
          carto::Object	sx = obj.getProperty( "__syntax__" );
          if( sx.get() )
            {
              synt = sx->GenericObject::value<std::string>();
              hassyntax = true;
            }
        }
      catch( ... )
        {
        }
    }

  if( hassyntax )
    {
      soma::AsciiDataSourceTraits<std::string>::write( ds, "'__syntax__' : " );
      w.writeString( ds, synt );
      first = false;
    }

  for( im=x.begin(); im!=em; ++im )
    {
      if( first )
	first = false;
      else
        {
          ds.putch( ',' );
          ds.putch( sep );
          soma::AsciiDataSourceTraits<std::string>::write( ds, ind );
        }
      // const_cast: just to avoid referencing a const T
      carto::Object key = carto::Object::reference( const_cast<T &>( im->first ) );
      w.write( *key, indent, "", "", writeInternals );
      soma::AsciiDataSourceTraits<std::string>::write( ds, " : " );
      w.write( *im->second, indent, "", "", writeInternals );
    }
  ds.putch( sep );
  soma::AsciiDataSourceTraits<std::string>::write( ds, ind );
  ds.putch( '}' );
}


template<> 
void dictHelper<std::string>( const carto::GenericObject & obj, soma::PythonWriter & w,
			      int indent, bool writeInternals )
{
  soma::DataSource				& ds = *w.dataSource();
  const carto::IterableInterface 
    & y = *obj.getInterface<carto::IterableInterface>();
  carto::Object				im;

  bool					first = true, hassyntax = false;
  std::string				ind, ind2;
  int					i;
  char					sep = '\n';

  if( w.singleLine() )
    sep = ' ';
  else
    {
      for( i=0; i<indent-1; ++i )
	ind += "    ";
      ind2 = ind + "  ";
      if( indent > 0 )
        ind += "    ";
      ++indent;
    }

  ds.putch( '{' );
  ds.putch( sep );
  soma::AsciiDataSourceTraits<std::string>::write( ds, ind );

  std::string	synt;
  const carto::SyntaxedInterface	*si = obj.getInterface<carto::SyntaxedInterface>();
  if( si && si->hasSyntax() )
    {
      synt = si->getSyntax();
      hassyntax = true;
    }
  else
    {
      try
        {
          carto::Object	sx = obj.getProperty( "__syntax__" );
          if( sx.get() )
            {
              synt = sx->GenericObject::value<std::string>();
              hassyntax = true;
            }
        }
      catch( ... )
        {
        }
    }

  if( hassyntax )
    {
      soma::AsciiDataSourceTraits<std::string>::write( ds, "'__syntax__' : " );
      w.writeString( ds, synt );
      first = false;
    }

  std::string	key;
  im=y.objectIterator();
  while( im->isValid() )
    {
      key = im->key();
      if( ( writeInternals || !w.isInternal( "", key ) ) 
          && ( key != "__syntax__" || !hassyntax ) )
        {
          if( first )
            first = false;
          else
            {
              ds.putch( ',' );
              ds.putch( sep );
              soma::AsciiDataSourceTraits<std::string>::write( ds, ind );
            }
          w.writeString( ds, key );
          soma::AsciiDataSourceTraits<std::string>::write( ds, " : " );
          w.write( *im->currentValue(), indent, "", key, writeInternals );
        }
      im->next();
    }

  ds.putch( sep );
  soma::AsciiDataSourceTraits<std::string>::write( ds, ind2 );
  ds.putch( '}' );
}


void rcDictHelper( const carto::GenericObject & obj, soma::PythonWriter & w, int indent,
		   bool writeInternals )
{
  soma::DataSource				& ds = *w.dataSource();
  const std::map<carto::Object, carto::Object>	& x
    = obj.value<std::map<carto::Object, carto::Object> >();
  std::map<carto::Object, carto::Object>::const_iterator	im, em = x.end();
  bool					first = true, hassyntax = false;
  std::string				ind, ind2;
  int					i;
  char					sep = '\n';

  if( w.singleLine() )
    sep = ' ';
  else
    {
      for( i=0; i<indent-1; ++i )
	ind += "    ";
      ind2 = ind + "  ";
      if( indent > 0 )
        ind += "    ";
      ++indent;
    }

  ds.putch( '{' );
  ds.putch( sep );
  soma::AsciiDataSourceTraits<std::string>::write( ds, ind );

  std::string	synt;
  const carto::SyntaxedInterface	*si = obj.getInterface<carto::SyntaxedInterface>();
  if( si && si->hasSyntax() )
    {
      synt = si->getSyntax();
      hassyntax = true;
    }
  else
    {
      try
        {
          carto::Object	sx = obj.getProperty( "__syntax__" );
          if( sx.get() )
            {
              synt = sx->GenericObject::value<std::string>();
              hassyntax = true;
            }
        }
      catch( ... )
        {
        }
    }

  if( hassyntax )
    {
      soma::AsciiDataSourceTraits<std::string>::write( ds, "'__syntax__' : " );
      w.writeString( ds, synt );
      first = false;
    }

  for( im=x.begin(); im!=em; ++im )
    {
      if( first )
	first = false;
      else
        {
          ds.putch( ',' );
          ds.putch( sep );
          soma::AsciiDataSourceTraits<std::string>::write( ds, ind );
        }
      w.write( *im->first, indent, "", "", writeInternals );
      soma::AsciiDataSourceTraits<std::string>::write( ds, " : " );
      w.write( *im->second, indent, "", "", writeInternals );
    }

  ds.putch( sep );
  soma::AsciiDataSourceTraits<std::string>::write( ds, ind2 );
  ds.putch( '}' );
}


void listHelper( const carto::GenericObject & obj, soma::PythonWriter & w,
                 int indent, bool writeInternals )
{
  soma::DataSource			& ds = *w.dataSource();
  carto::Object			it;
  bool				first = true;

  ds.putch( '[' );
  ++indent;
  for( it=obj.objectIterator(); it->isValid(); it->next() )
    {
      if( first )
        first = false;
      else
        ds.putch( ',' );
      ds.putch( ' ' );
      w.write( *it->currentValue(), indent, "", "", writeInternals );
    }
  ds.putch( ' ' );
  ds.putch( ']' );
}

} // namespace (internal linkage)

template<typename T> 
void 
soma::PythonWriter::genericSequenceHelper( const carto::GenericObject & obj,
                                           soma::PythonWriter & w,
                                           int ind,
                                           bool writeInternals )
{
  soma::DataSource    & ds = *w.dataSource();
  carto::Object ix = obj.objectIterator();
  carto::Object de;
  bool              first = true;

  ds.putch( '[' );
  for( ix=obj.objectIterator(); ix->isValid(); ix->next() )
  {
    if( first )
    {
      first = false;
          ds.putch( ' ' );
    }
    else
    {
      ds.putch( ',' );
      ds.putch( ' ' );
    }
    de = ix->currentValue();
    genericHelper<typename T::value_type>( *de, w, ind, writeInternals );
  }
  ds.putch( ' ' );
  ds.putch( ']' );
}

#endif

