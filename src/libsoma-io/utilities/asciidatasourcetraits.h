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

#ifndef SOMAIO_UTILITIES_ASCIIDATASOURCETRAITS_H
#define SOMAIO_UTILITIES_ASCIIDATASOURCETRAITS_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/exception/ioexcept.h>
#include <cartobase/stream/sstream.h>
#include <cartobase/type/string_conversion.h>
#include <cartobase/type/limits.h>
#include <cartobase/type/voxelvalue.h>
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
//--- system -----------------------------------------------------------------
#include <cstring>
#include <iostream>
#include <limits>
//----------------------------------------------------------------------------

namespace soma
{

  template<typename T>
  class AsciiDataSourceTraits
  {
  public:
    static bool read( DataSource & ds, T & item );
    static bool write( DataSource & ds, const T & item );
  };


  class StreamUtil
  {
  public:
    static std::string readWhile( DataSource &, 
                                  const std::string & chars = " \t\n\r", 
                                  bool ascii = true );
    static std::string readUntil( DataSource &, 
                                  const std::string & chars = " \t\n\r", 
                                  bool ascii = true );
    static bool skip( DataSource &, const std::string & chars = " \t\n\r", 
                      bool ascii = true );
    static bool skipUntil( DataSource &, 
                           const std::string & chars = " \t\n\r", 
                           bool ascii = true );
    static bool getline( DataSource & ds, std::string & );

    static bool skip( std::istream &, const std::string & chars = " \t\n\r", 
                      bool ascii = true );
  };

  // -----------

#ifndef DOXYGEN_HIDE_INTERNAL_CLASSES

  template<typename T>
  inline
  bool AsciiDataSourceTraits<T>::read( DataSource &, T & x )
  {
    make_compil_error( x, "Instanciation on an unexpected type. "
                          "This function must be specialized." );

    /*
    throw carto::datatype_format_error
      ( std::string( "cannot read an ASCII element of type " )
        + DataTypeCode<T>::name() );
    */
    return false;
  }


  template<typename T>
  inline
  bool AsciiDataSourceTraits<T>::write( DataSource &, const T & x )
  {
    make_compil_error( x, "Instanciation on an unexpected type. "
                          "This function must be specialized." );

    /*
    throw carto::datatype_format_error
      ( std::string( "cannot write an ASCII element of type " )
        + DataTypeCode<T>::name() );
    */
    return false;
  }


  namespace internal
  {

    template <typename T>
    class AsciiIntReadTraits
    {
    public:
      static bool read( DataSource &, T & );
      static bool write( DataSource &, const T & );
    };

    template <typename T>
    class AsciiFloatReadTraits
    {
    public:
      static bool read( DataSource &, T & );
      static bool write( DataSource &, const T & );
    };

  } // namespace internal


  template<>
  inline
  bool AsciiDataSourceTraits<bool>::read( DataSource & ds, bool & item )
  {
    return internal::AsciiIntReadTraits<bool>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<bool>::write( DataSource & ds, 
                                           const bool & item )
  {
    return internal::AsciiIntReadTraits<bool>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<uint8_t>::read( DataSource & ds, uint8_t & item )
  {
    return internal::AsciiIntReadTraits<uint8_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<uint8_t>::write( DataSource & ds, 
                                              const uint8_t & item )
  {
    return internal::AsciiIntReadTraits<uint8_t>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int8_t>::read( DataSource & ds, int8_t & item )
  {
    return internal::AsciiIntReadTraits<int8_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int8_t>::write( DataSource & ds, 
                                             const int8_t & item )
  {
    return internal::AsciiIntReadTraits<int8_t>::write( ds, item );
  }

#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
  template<>
  inline
  bool AsciiDataSourceTraits<char>::read( DataSource & ds, char & item )
  {
    return internal::AsciiIntReadTraits<char>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<char>::write( DataSource & ds, 
                                           const char & item )
  {
    return internal::AsciiIntReadTraits<char>::write( ds, item );
  }
#endif

  template<>
  inline
  bool AsciiDataSourceTraits<uint16_t>::read( DataSource & ds, 
                                              uint16_t & item )
  {
    return internal::AsciiIntReadTraits<uint16_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<uint16_t>::write( DataSource & ds, 
                                               const uint16_t & item )
  {
    return internal::AsciiIntReadTraits<uint16_t>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int16_t>::read( DataSource & ds, int16_t & item )
  {
    return internal::AsciiIntReadTraits<int16_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int16_t>::write( DataSource & ds, 
                                              const int16_t & item )
  {
    return internal::AsciiIntReadTraits<int16_t>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<uint32_t>::read( DataSource & ds, 
                                              uint32_t & item )
  {
    return internal::AsciiIntReadTraits<uint32_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<uint32_t>::write( DataSource & ds, 
                                               const uint32_t & item )
  {
    return internal::AsciiIntReadTraits<uint32_t>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int32_t>::read( DataSource & ds, int32_t & item )
  {
    return internal::AsciiIntReadTraits<int32_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int32_t>::write( DataSource & ds, 
                                              const int32_t & item )
  {
    return internal::AsciiIntReadTraits<int32_t>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int64_t>::read( DataSource & ds, int64_t & item )
  {
    return internal::AsciiIntReadTraits<int64_t>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<int64_t>::write( DataSource & ds, 
                                              const int64_t & item )
  {
    return internal::AsciiIntReadTraits<int64_t>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<uint64_t>::read( DataSource & ds, 
                                              uint64_t & item )
  {
    return internal::AsciiIntReadTraits<uint64_t>::read( ds, item );
  }

  template<>
  inline
  bool 
  AsciiDataSourceTraits<uint64_t>::write( DataSource & ds, 
                                          const uint64_t & item )
  {
    return internal::AsciiIntReadTraits<uint64_t>::write( ds, item );
  }

#ifndef __LP64__
  // these types are int64_t/uint64_t on 64 bit systems
  template<>
  inline
  bool AsciiDataSourceTraits<long>::read( DataSource & ds, long & item )
  {
    return internal::AsciiIntReadTraits<long>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<long>::write( DataSource & ds,
                                           const long & item )
  {
    return internal::AsciiIntReadTraits<long>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<unsigned long>::read( DataSource & ds,
                                                   unsigned long & item )
  {
    return internal::AsciiIntReadTraits<unsigned long>::read( ds, item );
  }

  template<>
  inline
  bool
  AsciiDataSourceTraits<unsigned long>::write( DataSource & ds,
                                               const unsigned long & item )
  {
    return internal::AsciiIntReadTraits<unsigned long>::write( ds, item );
  }

#endif

  template<>
  inline
  bool AsciiDataSourceTraits<float>::read( DataSource & ds, float & item )
  {
    return internal::AsciiFloatReadTraits<float>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<float>::write( DataSource & ds, 
                                            const float & item )
  {
    return internal::AsciiFloatReadTraits<float>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<double>::read( DataSource & ds, double & item )
  {
    return internal::AsciiFloatReadTraits<double>::read( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<double>::write( DataSource & ds, 
                                            const double & item )
  {
    return internal::AsciiFloatReadTraits<double>::write( ds, item );
  }

  template<>
  inline
  bool AsciiDataSourceTraits<std::string>::read( DataSource & ds, 
                                                 std::string & item )
  {
    int		c, i = 0;
    item.clear();

    while( true )
      {
        c = ds.getch();
        if( c == '\0' || !ds.isOpen() || c == '\n' || c == '\r' || c == ' ' 
            || c == '\t' )
          break;
        item += (char) c;
        ++i;
      }
    ds.ungetch( c );
    if( i == 0 )
      return false;
    return true;
  }

  template<>
  inline
  bool AsciiDataSourceTraits<std::string>::write( DataSource & ds, 
                                                  const std::string & item )
  {
    ds.writeBlock( item.c_str(), item.size() );
    return ds.isOpen();
  }


  template<>
  inline
  bool AsciiDataSourceTraits<char *>::write( DataSource & ds, 
                                             char * const & item )
  {
    ds.writeBlock( item, strlen( item ) );
    return ds.isOpen();
  }


  template<>
  inline
  bool AsciiDataSourceTraits<const char *>::write( DataSource & ds, 
                                                   const char * const & item )
  {
    ds.writeBlock( item, strlen( item ) );
    return ds.isOpen();
  }

  namespace internal
  {

    template<typename T>
    inline
    bool AsciiIntReadTraits<T>::read( DataSource & ds, T & item )
    {
      int		c, i = 0;
      std::string	num;
      bool		hex = false;
      int		sign = 0;
      int		expo = 0;

      while( true )
        {
          c = ds.getch();
          if( c == '\0' || !ds.isOpen() )
            break;
          if( c < '0' || c > '9' )
          {
            if( !sign && i == expo && ( c == '-' || c == '+' ) )
              sign = i+1;
            else if( c == 'x' && !hex && i == sign+1 && num[sign] == '0' )
              hex = true;
            else if( !hex && !expo && ( c == 'e' || c == 'E' ) && i > sign )
              {
                expo = i+1;
                sign = 0;
              }
            else if( !hex || c < 'A' || ( c > 'F' && c < 'a' ) || c > 'f' )
              break;
          }
          num += (char) c;
          ++i;
        }
      ds.ungetch( c );
      if( i == 0 )
        return false;
      carto::stringTo( num, item );
      return true;
    }


    template<typename T>
    inline
    bool AsciiIntReadTraits<T>::write( DataSource & ds, const T & item )
    {
      std::ostringstream	ss;
      ss << item;
      return AsciiDataSourceTraits<std::string>::write( ds, ss.str() );
    }


    template<typename T>
    inline
    bool AsciiFloatReadTraits<T>::read( DataSource & ds, T & item )
    {
      int		c, i = 0;
      std::string	num;
      bool		hex = false;
      bool		dot = false;
      int		nan = 0;
      int		inf = 0;
      int		sign = 0;
      int		expo = 0;

      while( true )
        {
          c = ds.getch();
          if( c == '\0' || !ds.isOpen() )
            break;
          if( inf )
            {
              if( inf >= 3 )
                {
                  if( sign )
                    item = -std::numeric_limits<T>::infinity();
                  else
                    item = std::numeric_limits<T>::infinity();
                  return true;
                }
              if( c != "nf"[inf-1] && c != "NF"[inf-1] )
                return false;
              ++ inf;
            }
          else if( nan )
            {
              if( nan >= 3 )
                {
                  if( sign )
                    item = -std::numeric_limits<T>::quiet_NaN();
                  else
                    item = std::numeric_limits<T>::quiet_NaN();
                  return true;
                }
              if( c != "an"[nan-1] && c != "AN"[nan-1] )
                return false;
              ++nan;
            }
          else if( c < '0' || c > '9' )
          {
            if( !sign && i == expo && ( c == '-' || c == '+' ) )
              sign = i+1;
            else if( !dot && c == 'x' && !hex && i == sign+1
                     && num[sign] == '0' )
              hex = true;
            else if( !dot && !hex && c == '.' )
              dot = true;
            else if( !hex && !expo && ( c == 'e' || c == 'E' ) && i > sign )
              {
                expo = i+1;
                sign = 0;
              }
            else if( !hex && !dot && ( c == 'n' || c == 'N' ) )
              nan = 1;
            else if( !hex && !dot && ( c == 'i' || c == 'I' ) )
              inf = 1;
            else if( !hex || c < 'A' || ( c > 'F' && c < 'a' ) || c > 'f' )
              break;
          }
          num += (char) c;
          ++i;
        }
      ds.ungetch( c );
      if( i == 0 )
        return false;
      carto::stringTo( num, item );
      return true;
    }


    template<typename T>
    inline
    bool AsciiFloatReadTraits<T>::write( DataSource & ds, const T & item )
    {
      std::ostringstream	ss;
      ss.precision(std::numeric_limits< double >::digits10);
      ss << item;
      return AsciiDataSourceTraits<std::string>::write( ds, ss.str() );
    }


  } // namespace internal

#endif // DOXYGEN_HIDE_INTERNAL_CLASSES

  // stream operators
  /*
  template<typename T> inline
  DataSource & 
  operator << ( DataSource & ds, const T & x )
  {
    AsciiDataSourceTraits<T>::write( ds, x );
    return ds;
  }
  */

  template<typename T> inline 
  DataSource & 
  operator >> ( DataSource & ds, T & x )
  {
    AsciiDataSourceTraits<T>::read( ds, x );
    return ds;
  }

#if !defined(__sun__) || !defined(_CHAR_IS_SIGNED)
  inline DataSource & 
  operator << ( DataSource & ds, const char & x )
  {
    AsciiDataSourceTraits<char>::write( ds, x );
    return ds;
  }
#endif

  inline DataSource & 
  operator << ( DataSource & ds, const int8_t & x )
  {
    AsciiDataSourceTraits<int8_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const uint8_t & x )
  {
    AsciiDataSourceTraits<uint8_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const int16_t & x )
  {
    AsciiDataSourceTraits<int16_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const uint16_t & x )
  {
    AsciiDataSourceTraits<uint16_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const int32_t & x )
  {
    AsciiDataSourceTraits<int32_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const uint32_t & x )
  {
    AsciiDataSourceTraits<uint32_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const int64_t & x )
  {
    AsciiDataSourceTraits<int64_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const uint64_t & x )
  {
    AsciiDataSourceTraits<uint64_t>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const float & x )
  {
    AsciiDataSourceTraits<float>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const double & x )
  {
    AsciiDataSourceTraits<double>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const std::string & x )
  {
    AsciiDataSourceTraits<std::string>::write( ds, x );
    return ds;
  }

  inline DataSource & 
  operator << ( DataSource & ds, const char * const & x )
  {
    AsciiDataSourceTraits<const char *>::write( ds, x );
    return ds;
  }

  //=== VOXEL VALUE ==========================================================

  template <typename T, unsigned int C>
  class AsciiDataSourceTraits< carto::VoxelValue<T,C> >
  {
  public:
    static bool read( DataSource &ds, carto::VoxelValue<T,C>  &item );
    static bool write( DataSource &ds, const carto::VoxelValue<T,C>  &item );
  };

  template <typename T, unsigned int C> inline
  bool AsciiDataSourceTraits<carto::VoxelValue<T,C> >::read( DataSource & ds,
                                               carto::VoxelValue<T,C> & item )
  {
    int c;
    carto::VoxelValue<T,C> result;

    while( true ) {
      c = ds.getch();
      if( c != ' ' && c != '\t' && c != '\n' && c != '(' )
        break;
      if( c == '(' ) {
        AsciiDataSourceTraits<T>::read( ds, result[0] );
        c = ds.getch();
        for( unsigned int i=1; i<C; ++i )
          if( c == ',' ) {
            AsciiDataSourceTraits<T>::read( ds, result[i] );
            c = ds.getch();
          }
        if( c == 0 || c == ')' ) {
          item = result;
          return true;
        }
        return false;
      }
    }
    return false;
  }

  template <typename T, unsigned int C> inline
  bool AsciiDataSourceTraits<carto::VoxelValue<T,C> >::write( DataSource & ds,
                                                  const carto::VoxelValue<T,C>
                                                    & item )
  {
    ds << '(';
    for( unsigned int i=0; i<C-1; ++i )
      ds << (int) item[i] << ',';
    ds << (int) item[C-1] << ')';
    return true;
  }

  template<typename T, unsigned int C> inline DataSource &
  operator << ( DataSource & ds, const carto::VoxelValue<T,C> & x )
  {
    AsciiDataSourceTraits<carto::VoxelValue<T,C> >::write( ds, x );
    return ds;
  }

  //=== VOXEL RGB ============================================================

  template <> inline
  bool AsciiDataSourceTraits<carto::VoxelRGB>::read( DataSource & ds,
                                                     carto::VoxelRGB & item )
  {
    return AsciiDataSourceTraits<carto::VoxelValue<uint8_t,3> >::read( ds, item );
  }

  template <> inline
  bool AsciiDataSourceTraits<carto::VoxelRGB>::write( DataSource & ds,
                                                const carto::VoxelRGB & item )
  {
    return AsciiDataSourceTraits<carto::VoxelValue<uint8_t,3> >::write( ds, item );
  }

  inline DataSource &
  operator << ( DataSource & ds, const carto::VoxelRGB & x )
  {
    AsciiDataSourceTraits<carto::VoxelRGB>::write( ds, x );
    return ds;
  }

  //=== VOXEL RGBA ===========================================================

  template <> inline
  bool AsciiDataSourceTraits<carto::VoxelRGBA>::read( DataSource & ds,
                                                     carto::VoxelRGBA & item )
  {
    return AsciiDataSourceTraits<carto::VoxelValue<uint8_t,4> >::read( ds, item );
  }

  template <> inline
  bool AsciiDataSourceTraits<carto::VoxelRGBA>::write( DataSource & ds,
                                                const carto::VoxelRGBA & item )
  {
    return AsciiDataSourceTraits<carto::VoxelValue<uint8_t,4> >::write( ds, item );
  }

  inline DataSource &
  operator << ( DataSource & ds, const carto::VoxelRGBA & x )
  {
    AsciiDataSourceTraits<carto::VoxelRGBA>::write( ds, x );
    return ds;
  }

  //=== VOXEL HSV ============================================================

  template <> inline
  bool AsciiDataSourceTraits<carto::VoxelHSV>::read( DataSource & ds,
                                              carto::VoxelHSV & item )
  {
    return AsciiDataSourceTraits<carto::VoxelValue<uint8_t,3> >::read( ds, item );
  }

  template <> inline
  bool AsciiDataSourceTraits<carto::VoxelHSV>::write( DataSource & ds,
                                               const carto::VoxelHSV & item )
  {
    return AsciiDataSourceTraits<carto::VoxelValue<uint8_t,3> >::write( ds, item );
  }

  inline DataSource &
  operator << ( DataSource & ds, const carto::VoxelHSV & x )
  {
    AsciiDataSourceTraits<carto::VoxelHSV>::write( ds, x );
    return ds;
  }

} // namespace soma

#endif
