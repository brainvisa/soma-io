#ifndef _Vector_h_
#define _Vector_h_


#include <soma-io/config/soma_config.h>

#include <iostream>


namespace soma
{


class Vector
{

  public:

    Vector();
    Vector( const Vector& other );
    Vector( double theX, double theY, double theZ );

    Vector& operator += ( const Vector& other );
    Vector& operator -= ( const Vector& other );
    Vector& operator *= ( double value );
    Vector& operator /= ( double value );

    Vector cross( const Vector& other ) const;
    double dot( const Vector& other );
    double getNorm();
    void normalize();
    Vector mainDirection() const;
    Vector abs() const;

    double x;
    double y;
    double z;

};


}


soma::Vector operator + ( const soma::Vector& vec );
soma::Vector operator - ( const soma::Vector& vec );

soma::Vector operator + ( const soma::Vector& vec1, const soma::Vector& vec2 );
soma::Vector operator - ( const soma::Vector& vec1, const soma::Vector& vec2 );
soma::Vector operator * ( const soma::Vector& vec, const double& val );
soma::Vector operator / ( const soma::Vector& vec, const double& val );


namespace std
{


ostream& operator << ( ostream& os, const soma::Vector& vec );


}


#endif
