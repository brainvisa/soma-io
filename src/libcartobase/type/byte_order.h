#ifndef CARTOBASE_TYPE_BYTE_ORDER_H
#define CARTOBASE_TYPE_BYTE_ORDER_H

#include <cartobase/config/cartobase_config.h>
#include <complex>
#include <iostream>
#include <stdexcept>
#include <string>

namespace carto {

// Actual machine byte order
int byteOrder();

// String representing the system byte order :
//    "ABCD" for big endian
//    "DCBA" for little endian
//    "CDAB" for pdpEndian
const char * byteOrderString();


// Convert a string representing byte order
// to the corresponding constant
int stringToByteOrder( const std::string &bos );


// Class to do byte swapping in memory or from a file
class ByteSwapper
{
public:

  // Build a converter from any byte order to
  // actual machine byte order
  ByteSwapper( int bo = byteOrder() );
  ByteSwapper( const std::string &bos );

  inline bool isSwapped() const { return _swap; }
  inline bool setSwapped( bool s ) { _swap = s; return _swap; }

  inline void reorder( signed char &p ) const;
  inline void reorder( unsigned char &p ) const;
  inline void reorder( short &p ) const;
  inline void reorder( unsigned short &p ) const;
  inline void reorder( int &p ) const;
  inline void reorder( unsigned int &p ) const;
  inline void reorder( long &p ) const;
  inline void reorder( unsigned long &p ) const;
  inline void reorder( long long &p ) const;
  inline void reorder( unsigned long long &p ) const;
  inline void reorder( float &p ) const;
  inline void reorder( double &p ) const;
  inline void reorder( long double &p ) const;
  inline void reorder( std::complex<float> &p ) const;
  inline void reorder( std::complex<double> &p ) const;
  inline void reorder( std::complex<long double> &p ) const;

  inline std::istream &read( std::istream &, short &p ) const;
  inline std::istream &read( std::istream &, unsigned short &p ) const;
  inline std::istream &read( std::istream &, int &p ) const;
  inline std::istream &read( std::istream &, unsigned int &p ) const;
  inline std::istream &read( std::istream &, long &p ) const;
  inline std::istream &read( std::istream &, unsigned long &p ) const;
  inline std::istream &read( std::istream &, long long &p ) const;
  inline std::istream &read( std::istream &, unsigned long long &p ) const;
  inline std::istream &read( std::istream &, float &p ) const;
  inline std::istream &read( std::istream &, double &p ) const;
  inline std::istream &read( std::istream &, long double &p ) const;

  inline std::ostream &write( std::ostream &, short &p ) const;
  inline std::ostream &write( std::ostream &, unsigned short &p ) const;
  inline std::ostream &write( std::ostream &, int &p ) const;
  inline std::ostream &write( std::ostream &, unsigned int &p ) const;
  inline std::ostream &write( std::ostream &, long &p ) const;
  inline std::ostream &write( std::ostream &, unsigned long &p ) const;
  inline std::ostream &write( std::ostream &, long long &p ) const;
  inline std::ostream &write( std::ostream &, unsigned long long &p ) const;
  inline std::ostream &write( std::ostream &, float &p ) const;
  inline std::ostream &write( std::ostream &, double &p ) const;
  inline std::ostream &write( std::ostream &, long double &p ) const;

private:

  template <unsigned SIZE> 
  static void _doSwap( char * );

  template <unsigned SIZE> 
  static std::istream & _swappedRead( std::istream &, char * );

  template <unsigned SIZE> 
  static std::ostream & _swappedWrite( std::ostream &, char * );

  bool _swap;
};


// Non specialized swapping is forbidden
template <unsigned SIZE> 
void ByteSwapper::_doSwap( char * )
{
  throw std::runtime_error( "Invalid byte order swap size" );
}

// Non specialized reading is forbidden
template <unsigned SIZE> 
std::istream &ByteSwapper::_swappedRead( std::istream & s, char * )
{
  throw std::runtime_error( "Invalid byte order read size" );
  return s;
}

// Non specialized writing is forbidden
template <unsigned SIZE> 
std::ostream &ByteSwapper::_swappedWrite( std::ostream & s, char * )
{
  throw std::runtime_error( "Invalid byte order write size" );
  return s;
}


template <> 
void ByteSwapper::_doSwap<2>( char *p );

template <> 
void ByteSwapper::_doSwap<4>( char *p );

template <> 
void ByteSwapper::_doSwap<8>( char *p );

template <> 
void ByteSwapper::_doSwap<16>( char *p );


template <> 
std::istream & ByteSwapper::_swappedRead<2>( std::istream &, char * );

template <> 
std::istream & ByteSwapper::_swappedRead<4>( std::istream &, char * );

template <> 
std::istream & ByteSwapper::_swappedRead<8>( std::istream &, char * );

template <> 
std::istream & ByteSwapper::_swappedRead<16>( std::istream &, char * );

template <> 
std::ostream &ByteSwapper::_swappedWrite<2>( std::ostream &, char * );

template <> 
std::ostream &ByteSwapper::_swappedWrite<4>( std::ostream &, char * );

template <> 
std::ostream &ByteSwapper::_swappedWrite<8>( std::ostream &, char * );

template <> 
std::ostream &ByteSwapper::_swappedWrite<16>( std::ostream &, char * );




inline void ByteSwapper::reorder( signed char & ) const
{
  // Char is 1 byte, nothing to do
}

inline void ByteSwapper::reorder( unsigned char & ) const
{
  // Char is 1 byte, nothing to do
}

inline void ByteSwapper::reorder( short &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( unsigned short &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( int &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( unsigned int &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( long &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( unsigned long &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( long long &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( unsigned long long &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( float &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( double &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( long double &p ) const 
{
  if ( _swap ) _doSwap< sizeof(p) >( reinterpret_cast< char * >( &p ) );
}

inline void ByteSwapper::reorder( std::complex<float> &p ) const
{
  if ( _swap ) {
    float real = p.real();
    reorder(real);
    p.real(real);
    float imag = p.imag();
    reorder(imag);
    p.imag(imag);
  }
}

inline void ByteSwapper::reorder( std::complex<double> &p ) const
{
  if ( _swap ) {
    double real = p.real();
    reorder(real);
    p.real(real);
    double imag = p.imag();
    reorder(imag);
    p.imag(imag);
  }
}

inline void ByteSwapper::reorder( std::complex<long double> &p ) const
{
  if ( _swap ) {
    long double real = p.real();
    reorder(real);
    p.real(real);
    long double imag = p.imag();
    reorder(imag);
    p.imag(imag);
  }
}


inline std::istream &ByteSwapper::read( std::istream &in, 
                                        short &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in,
                                        unsigned short &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, 
                                        int &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in,
                                        unsigned int &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, 
                                        long &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, 
                                        unsigned long &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, 
                                        long long &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, unsigned long long &p ) 
     const {
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, float &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in,
                                        double &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::istream &ByteSwapper::read( std::istream &in, 
                                        long double &p ) const 
{
  if ( _swap ) _swappedRead< sizeof(p) >( in, 
                                          reinterpret_cast< char * >( &p ) );
  else in.read( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return in;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         short &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         unsigned short &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         int &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         unsigned int &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         long &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         unsigned long &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         unsigned long long &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         long long &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         float &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out,
                                         double &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


inline std::ostream &ByteSwapper::write( std::ostream &out, 
                                         long double &p ) const 
{
  if ( _swap ) _swappedWrite< sizeof(p) >( out, 
                                           reinterpret_cast< char * >( &p ) );
  else out.write( reinterpret_cast<char *>( &p ), sizeof( p ) );
  return out;
}


} // namesapce carto

#endif
