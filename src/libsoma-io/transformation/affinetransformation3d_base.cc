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

#include <cstdlib>
#include <soma-io/transformation/affinetransformation3d_base.h>
#include <soma-io/vector/vector.h>
#include <iostream>
#include <iomanip>

using namespace soma;
using namespace carto;
using namespace std;

/*
 *  AFFINE 3-D transformations (Linear + Translation)
 */


namespace soma
{

//   float decompositionLU( Table<float> &a, Table<int32_t> &indx);
//   void backSubstitutionLU( Table<float> &a, Table<int32_t> &indx,
//                            Table<float> &b );
  AffineTransformation3dBase::Table<float>
  inversionLU( const AffineTransformation3dBase::Table<float> &matrix );

}


/** \class AffineTransformation3dBase
NOTE: the class AffineTransformation3d has got real problem of CLARITY and IMPLEMENTATION.
For example the order of specification of the attributes can change the
resulting AffineTransformation3d.
At this stage of the code (11/03/2010, submitted by Thierry Delzescaux, Nicolas
Souedet and Hugo Raguet), one must respect this order of specification to get
the right AffineTransformation3d:

	AffineTransformation3d::
		- create the AffineTransformation3d
		- first specify setRotationAffine
		- then specify setTranslation
		- one can then redo this process any time as long as we first specify
setRotationAffine

	DecomposedAffineTransformation3d::
		- create the AffineTransformation3d
		- first specify setTranslation
		- then specify transAffine
		- it is then IMPOSSIBLE to reset the transformation because neither the
setTranslation nor the transAffine methods resets the _translation attribute

One way to address those problems (and several others, see methods below) would
be to create the Affine::AffineTransformation3d class according to the following specifications:

 # the AffineTransformation3d, applied to a point P, should do an AFFINE transformation under
the form
	AffineTransformation3d( P ) = C + A x ( P - C ) + T                            (1)
where
	A is the linear transformation, composition of rotation, scaling, shearing
	C is the "center" of the linear transformation
	T is the translation.
	
 # then, an AffineAffineTransformation3d should contain the following attributes:
 	_rotation (the equivalent of _rot in DecomposedAffineTransformation3d), 3x3 matrix
 	_scaling (equivalent in DecomposedAffineTransformation3d), 3x3 matrix
 	_shearing (equivalent in DecomposedAffineTransformation3d), 3x3 matrix
 	_translation (would contain only T), 3x1 vector
 	_center ( would contain C ), 3x1 vector
 	_linear ( would contain A = _rotation x _scaling x _shearing, i.e. the
equivalent of the current _rotation attribute, whose name is highly confusing),
3x3 matrix
 	_shift ( would contain t = C - A x C + T ), 3x1 vector
 
 # by rewritting (1) as
 	AffineTransformation3d( P ) = A x P + ( C - A x C + T ) = A x P + t            (2)
we see that the method AffineAffineTransformation3d::transform corresponds the current
AffineTransformation3d::transform, implementing P = _linear x P + _shift.
The reason of the attributes _linear and _shift is a matter of optimization:
each transformation require only 9 multiplications et 12 additions. Otherwise,
computing directly the trnsformation as written in (1) will more than double
the number of necessary operations.

 # there must be methods which simply SETS the attributes _rotation, _scaling,
_shearing, _translation, _center, BUT DO NOT CHANGE _linear and _shift, so it
does not change the results of a transformation by this AffineTransformation3d

 # finally a method like "AffineAffineTransformation3d::updateTransformation" should update
the attributes _linear and _shift so that the transformation (2) correctly
computes (1) consistently with all attributes i.e. this methods do
	_linear <- _rotation x _scaling x _shearing;
	_shift <- _center - _linear x _center + _translation;

*/
  //----------//
 //  AffineTransformation3dBase  //
//----------//

//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase()
  : _matrix( 4, 4 )
{
  for ( int i = 0; i < 4; i++ )
  {
    _matrix[ i + i*4 ] = 1.0;
  }
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::~AffineTransformation3dBase()
{
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase(
  const AffineTransformation3dBase& other ) :
  RCObject(),
  Transformation3d( other ),
  _matrix( other._matrix )
{
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase( const vector<float> & mat ) :
    Transformation3d(), _matrix( 4, 4 )
{
  _matrix[0] = mat[0];
  _matrix[4] = mat[1];
  _matrix[8] = mat[2];
  _matrix[12] = mat[3];
  _matrix[1] = mat[4];
  _matrix[5] = mat[5];
  _matrix[9] = mat[6];
  _matrix[13] = mat[7];
  _matrix[2] = mat[8];
  _matrix[6] = mat[9];
  _matrix[10] = mat[10];
  _matrix[14] = mat[11];
  _matrix[15] = 1.f;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase( const Object mat ) :
    Transformation3d(), _matrix( 4, 4 )
{
  *this = mat;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase& AffineTransformation3dBase::operator =
  ( const AffineTransformation3dBase& other )
{
  if( &other == this )
    return *this;
  _matrix = other._matrix;
  return *this;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase& AffineTransformation3dBase::operator = (
  const vector<float> & mat )
{
  _matrix.resize( 16 );
  _matrix[0] = mat[0];
  _matrix[4] = mat[1];
  _matrix[8] = mat[2];
  _matrix[12] = mat[3];
  _matrix[1] = mat[4];
  _matrix[5] = mat[5];
  _matrix[9] = mat[6];
  _matrix[13] = mat[7];
  _matrix[2] = mat[8];
  _matrix[6] = mat[9];
  _matrix[10] = mat[10];
  _matrix[14] = mat[11];
  _matrix[3] = 0.f;
  _matrix[7] = 0.f;
  _matrix[11] = 0.f;
  _matrix[15] = 1.f;
  return *this;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase& AffineTransformation3dBase::operator = ( const Object mat )
{
  if( mat->size() < 12 )
    return *this; // or throw something ?
  Object  iter = mat->objectIterator();
  if( !iter->isValid() )
    return *this; // or throw something ?
  _matrix[0] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[4] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[8] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[12] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[1] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[5] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[9] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[13] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[2] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[6] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[10] = iter->currentValue()->getScalar();
  iter->next();
  _matrix[14] = iter->currentValue()->getScalar();
  _matrix[3] = 0.F;
  _matrix[7] = 0.F;
  _matrix[11] = 0.F;
  _matrix[15] = 1.F;
  return *this;
}


//-----------------------------------------------------------------------------
bool AffineTransformation3dBase::operator == ( const AffineTransformation3dBase & other )
{
  static float eps = 1e-5;
  return fabs( _matrix[0] - other._matrix[0] ) < eps
      && fabs( _matrix[1] - other._matrix[1] ) < eps
      && fabs( _matrix[2] - other._matrix[2] ) < eps
      && fabs( _matrix[4] - other._matrix[4] ) < eps
      && fabs( _matrix[5] - other._matrix[5] ) < eps
      && fabs( _matrix[6] - other._matrix[6] ) < eps
      && fabs( _matrix[8] - other._matrix[8] ) < eps
      && fabs( _matrix[9] - other._matrix[9] ) < eps
      && fabs( _matrix[10] - other._matrix[10] ) < eps
      && fabs( _matrix[12] - other._matrix[12] ) < eps
      && fabs( _matrix[13] - other._matrix[13] ) < eps
      && fabs( _matrix[14] - other._matrix[14] ) < eps;
}


//-----------------------------------------------------------------------------
Point3dd
AffineTransformation3dBase::transformDouble( double x, double y, double z ) const
{
  return Point3dd(
    _matrix[0] * x + _matrix[4] * y + _matrix[8] * z + _matrix[12],
    _matrix[1] * x + _matrix[5] * y + _matrix[9] * z + _matrix[13],
    _matrix[2] * x + _matrix[6] * y + _matrix[10] * z + _matrix[14] );
}


Point3df
AffineTransformation3dBase::transformFloat( float x, float y, float z ) const
{
  return Point3df(
    _matrix[0] * x + _matrix[4] * y + _matrix[8] * z + _matrix[12],
    _matrix[1] * x + _matrix[5] * y + _matrix[9] * z + _matrix[13],
    _matrix[2] * x + _matrix[6] * y + _matrix[10] * z + _matrix[14] );
}


Point3dd
AffineTransformation3dBase::transformVectorDouble( double x, double y,
                                                   double z ) const
{
  return Point3dd( _matrix[0] * x + _matrix[4] * y + _matrix[8] * z,
                   _matrix[1] * x + _matrix[5] * y + _matrix[9] * z,
                   _matrix[2] * x + _matrix[6] * y + _matrix[10] * z );
}


Point3df
AffineTransformation3dBase::transformVectorFloat( float x, float y, float z ) const
{
  return Point3df( _matrix[0] * x + _matrix[4] * y + _matrix[8] * z,
                   _matrix[1] * x + _matrix[5] * y + _matrix[9] * z,
                   _matrix[2] * x + _matrix[6] * y + _matrix[10] * z );
}


Point3dd AffineTransformation3dBase::transformNormalDouble( double x, double y,
                                                            double z ) const
{
  Point3dd      dir2( x, y, z );
  Point3dd	u = vectProduct( dir2, Point3dd( 0, 0, 1 ) );
  if( u.norm() <= 1e-4 ) u = vectProduct( dir2, Point3dd( 0, 1, 0 ) );
  Point3dd      w = vectProduct( dir2, u );

  u = transformVector( u );
  w = transformVector( w );
  dir2 = vectProduct( u, w );
  return dir2;
}


//-----------------------------------------------------------------------------
bool AffineTransformation3dBase::isIdentity() const
{
  if ( _matrix[0]  != 1.0 ) return false;
  if ( _matrix[5]  != 1.0 ) return false;
  if ( _matrix[10]  != 1.0 ) return false;
  if ( _matrix[1]  != 0.0 ) return false;
  if ( _matrix[2]  != 0.0 ) return false;
  if ( _matrix[4]  != 0.0 ) return false;
  if ( _matrix[6]  != 0.0 ) return false;
  if ( _matrix[8]  != 0.0 ) return false;
  if ( _matrix[9]  != 0.0 ) return false;
  if ( _matrix[12]  != 0.0 ) return false;
  if ( _matrix[13]  != 0.0 ) return false;
  if ( _matrix[14]  != 0.0 ) return false;

  return true;
}


//-----------------------------------------------------------------------------
void AffineTransformation3dBase::setToIdentity()
{
  _matrix[0] = 1.F;
  _matrix[1] = 0.F;
  _matrix[2] = 0.F;
  _matrix[3] = 0.F;
  _matrix[4] = 0.F;
  _matrix[5] = 1.F;
  _matrix[6] = 0.F;
  _matrix[7] = 0.F;
  _matrix[8] = 0.F;
  _matrix[9] = 0.F;
  _matrix[10] = 1.F;
  _matrix[11] = 0.F;
  _matrix[12] = 0.F;
  _matrix[13] = 0.F;
  _matrix[14] = 0.F;
  _matrix[15] = 1.F;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase AffineTransformation3dBase::inverse() const
{
  AffineTransformation3dBase AffineTransformation3d;
  AffineTransformation3d._matrix = inversionLU( _matrix );
  return AffineTransformation3d;
}

bool AffineTransformation3dBase::invertible() const
{
  // inverse() will throw carto::assert_error if matrix inversion fails
  try
  {
    inverse();
  }
  catch( const assert_error& )
  {
    return false;
  }
  return true;
}

unique_ptr<Transformation3d> AffineTransformation3dBase::getInverse() const
{
  return unique_ptr<Transformation3d>(
    new AffineTransformation3dBase(inverse()));
}


//-----------------------------------------------------------------------------
void AffineTransformation3dBase::scale( const Point3df& sizeFrom,
                                        const Point3df& sizeTo )
{
  _matrix( 0, 0 ) *= sizeFrom[ 0 ] / sizeTo[ 0 ];
  _matrix( 0, 1 ) *= sizeFrom[ 1 ] / sizeTo[ 0 ];
  _matrix( 0, 2 ) *= sizeFrom[ 2 ] / sizeTo[ 0 ];

  _matrix( 1, 0 ) *= sizeFrom[ 0 ] / sizeTo[ 1 ];
  _matrix( 1, 1 ) *= sizeFrom[ 1 ] / sizeTo[ 1 ];
  _matrix( 1, 2 ) *= sizeFrom[ 2 ] / sizeTo[ 1 ];

  _matrix( 2, 0 ) *= sizeFrom[ 0 ] / sizeTo[ 2 ];
  _matrix( 2, 1 ) *= sizeFrom[ 1 ] / sizeTo[ 2 ];
  _matrix( 2, 2 ) *= sizeFrom[ 2 ] / sizeTo[ 2 ];

  _matrix( 0, 3 ) /= sizeTo[ 0 ];
  _matrix( 1, 3 ) /= sizeTo[ 1 ];
  _matrix( 2, 3 ) /= sizeTo[ 2 ];
}


//-----------------------------------------------------------------------------
bool AffineTransformation3dBase::isDirect() const
{
  float	det = _matrix(0,0) * ( _matrix(1,1) * _matrix(2,2)
                               - _matrix(2,1) * _matrix(1,2) )
    + _matrix(1,0) * ( _matrix(2,1) * _matrix(0,2)
                       - _matrix(0,1) * _matrix(2,2) )
    + _matrix(2,0) * ( _matrix(0,1) * _matrix(1,2)
                       - _matrix(1,1) * _matrix(0,2) );
  return( det >= 0 );
}


//-----------------------------------------------------------------------------
void AffineTransformation3dBase::setTranslation(Point3df t)
/* NOTE: for the moment, in spite of its name, this method actually ADD t to the
current translation ( _translation <- (_translation + t) ). This LONG and
UNCLEAR code seems to be strictly equivalent to
	  translation() += t;
To be more relevent (and consistent with the name of the method), one should
even use
		translation() = t;
however, it should be noted that for the moment, the informations about the
center of transformations are contained in the _translation attribute (see for
example AffineTransformation3d::setRotationAffine or DecomposedAffineTransformation3d::transAffine) so that
 using this last form would reset the center of transformation to the origin
(0,0,0).

see NOTE, line 38. */
{
  _matrix( 0, 3 ) = t[0];
  _matrix( 1, 3 ) = t[1];
  _matrix( 2, 3 ) = t[2];
}


AffineTransformation3dBase & AffineTransformation3dBase::operator *= (
  const AffineTransformation3dBase & m )
{
  Point3df	r2 = transform( m._matrix[3], m._matrix[7], m._matrix[11] );
  _matrix[12] = r2[0];
  _matrix[13] = r2[1];
  _matrix[14] = r2[2];

  float	x[3];
  x[0] = _matrix[0];
  x[1] = _matrix[4];
  x[2] = _matrix[8];
  _matrix(0,0) = x[0] * m._matrix(0,0) + x[1] * m._matrix(1,0)
      + x[2] * m._matrix(2,0);
  _matrix(0,1) = x[0] * m._matrix(0,1) + x[1] * m._matrix(1,1)
      + x[2] * m._matrix(2,1);
  _matrix(0,2) = x[0] * m._matrix(0,2) + x[1] * m._matrix(1,2)
      + x[2] * m._matrix(2,2);
  x[0] = _matrix(1,0);
  x[1] = _matrix(1,1);
  x[2] = _matrix(1,2);
  _matrix(1,0) = x[0] * m._matrix(0,0) + x[1] * m._matrix(1,0)
      + x[2] * m._matrix(2,0);
  _matrix(1,1) = x[0] * m._matrix(0,1) + x[1] * m._matrix(1,1)
      + x[2] * m._matrix(2,1);
  _matrix(1,2) = x[0] * m._matrix(0,2) + x[1] * m._matrix(1,2)
      + x[2] * m._matrix(2,2);
  x[0] = _matrix(2,0);
  x[1] = _matrix(2,1);
  x[2] = _matrix(2,2);
  _matrix(2,0) = x[0] * m._matrix(0,0) + x[1] * m._matrix(1,0)
      + x[2] * m._matrix(2,0);
  _matrix(2,1) = x[0] * m._matrix(0,1) + x[1] * m._matrix(1,1)
      + x[2] * m._matrix(2,1);
  _matrix(2,2) = x[0] * m._matrix(0,2) + x[1] * m._matrix(1,2)
      + x[2] * m._matrix(2,2);

  return( *this );
}


AffineTransformation3dBase AffineTransformation3dBase::operator - () const
{
  AffineTransformation3dBase t;

  for( int i=0; i<15; ++i )
    t._matrix[i] = -_matrix[i];

  return t;
}


//-----------------------------------------------------------------------------
namespace soma
{

  AffineTransformation3dBase operator * (
    const AffineTransformation3dBase& AffineTransformation3d1,
    const AffineTransformation3dBase& AffineTransformation3d2 )
  {
    AffineTransformation3dBase AffineTransformation3dOut;

    AffineTransformation3dBase::Table< float > mat1(4,4), mat2(4,4), mat3(4,4);

    int i, j, k;

    for(i=0;i<3;++i)
      for(j=0;j<3;++j)
      {
        mat1(j,i) = AffineTransformation3d1.matrix()(i,j);
        mat2(j,i) = AffineTransformation3d2.matrix()(i,j);
      }

    for(i=0;i<3;++i)
    {
      mat1(3,i) = AffineTransformation3d1.matrix()(i, 3);
      mat2(3,i) = AffineTransformation3d2.matrix()(i, 3);
      mat1(i,3) = mat2(i,3) = 0;
    }

    mat1(3,3) = mat2(3,3) = 1.0;
    mat3 = 0;

    for( j=0; j<3; ++j )
      for( i=0; i<3; ++i )
        for(k=0;k<4;++k)
          mat3(i,j) += mat2(i,k)*mat1(k,j);


    for(i=0;i<3;++i)
    {
      AffineTransformation3dOut.matrix()(i, 3) = mat3(3,i);
    }

    for(i=0;i<3;++i)
      for(j=0;j<3;++j)
      {
        AffineTransformation3dOut.matrix()(j,i) = mat3(i,j);
      }
    // cout << "Compose  : " << AffineTransformation3dOut << endl << endl;

    return AffineTransformation3dOut;
  }

}

//-----------------------------------------------------------------------------
vector<float> AffineTransformation3dBase::toVector() const
{
  vector<float> vec( 16 );
  vec[0] = _matrix[0];
  vec[1] = _matrix[4];
  vec[2] = _matrix[8];
  vec[3] = _matrix[12];
  vec[4] = _matrix[1];
  vec[5] = _matrix[5];
  vec[6] = _matrix[9];
  vec[7] = _matrix[13];
  vec[8] = _matrix[2];
  vec[9] = _matrix[6];
  vec[10] = _matrix[10];
  vec[11] = _matrix[14];
  vec[12] = 0;
  vec[13] = 0;
  vec[14] = 0;
  vec[15] = 1.;
  return vec;
}


//-----------------------------------------------------------------------------
vector<float> AffineTransformation3dBase::toColumnVector() const
{
  return _matrix;
}


//-----------------------------------------------------------------------------
void AffineTransformation3dBase::fromColumnVector( const float* vec )
{
  _matrix.clear();
  _matrix.insert( _matrix.begin(), vec, vec + 16 );
  _matrix[3] = 0.f;
  _matrix[7] = 0.f;
  _matrix[11] = 0.f;
  _matrix[15] = 1.f;
}


namespace soma
{

  ostream& operator << ( ostream& os,
                        const soma::AffineTransformation3dBase & thing )
  {
  //   os << "{translation=" << thing.translation() << ", "<<endl
  //      << "rotation=[[" << thing.rotation()(0,0)
  //      << ", " << thing.rotation()(0,1)
  //      << ", " << thing.rotation()(0,2)
  //      << "]," << endl << "[" << thing.rotation()(1,0)
  //      << ", " << thing.rotation()(1,1)
  //      << ", " << thing.rotation()(1,2)
  //      << "]," << endl << "[" << thing.rotation()(2,0)
  //      << ", " << thing.rotation()(2,1)
  //      << ", " << thing.rotation()(2,2)
  //      << "]]}";

    os << "[[ " << setw(8) << thing.matrix()(0,0)
      << ", " << setw(8) << thing.matrix()(0,1)
      << ",   " << setw(8) << thing.matrix()(0,2)
      << ", " << setw(8) << thing.matrix()(0, 3)
      << " ]," << endl
      << " [ " << setw(8) << thing.matrix()(1,0)
      << ", " << setw(8) << thing.matrix()(1,1)
      << ",   " << setw(8) << thing.matrix()(1,2)
      << ", " << setw(8) << thing.matrix()(1, 3)
      << " ]," << endl
      << " [ " << setw(8) << thing.matrix()(2,0)
      << ", " << setw(8) << thing.matrix()(2,1)
      << ",   " << setw(8) << thing.matrix()(2,2)
      << ", " << setw(8) << thing.matrix()(2,3)
      << " ]]";

    return os;
  }

}

