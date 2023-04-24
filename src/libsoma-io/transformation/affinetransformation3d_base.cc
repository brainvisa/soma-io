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
#include <cartobase/object/property.h>
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
  AffineTransformationBase::Table<float>
  inversionLU( const AffineTransformationBase::Table<float> &matrix );

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
	
 # then, an AffineTransformation3d should contain the following attributes:
 	_rotation (the equivalent of _rot in DecomposedAffineTransformation3d), 3x3 matrix
 	_scaling (equivalent in DecomposedAffineTransformation3d), 3x3 matrix
 	_shearing (equivalent in DecomposedAffineTransformation3d), 3x3 matrix
 	_translation (would contain only T), 3x1 vector
 	_center ( would contain C ), 3x1 vector
 	_linear ( would contain A = _rotation x _scaling x _shearing, i.e. the
equivalent of the current _rotation attribute, whose name is highly confusing),
3x3 matrix
 	_shift ( would contain t = C - A x C + T ), 3x1 vector
 
 # by rewriting (1) as
 	AffineTransformation3d( P ) = A x P + ( C - A x C + T ) = A x P + t            (2)
we see that the method AffineTransformation3d::transform corresponds the current
AffineTransformation3d::transform, implementing P = _linear x P + _shift.
The reason of the attributes _linear and _shift is a matter of optimization:
each transformation require only 9 multiplications et 12 additions. Otherwise,
computing directly the trnsformation as written in (1) will more than double
the number of necessary operations.

 # there must be methods which simply SETS the attributes _rotation, _scaling,
_shearing, _translation, _center, BUT DO NOT CHANGE _linear and _shift, so it
does not change the results of a transformation by this AffineTransformation3d

 # finally a method like "AffineTransformation3d::updateTransformation" should update
the attributes _linear and _shift so that the transformation (2) correctly
computes (1) consistently with all attributes i.e. this methods do
	_linear <- _rotation x _scaling x _shearing;
	_shift <- _center - _linear x _center + _translation;

*/

  //----------------------------//
 //  AffineTransformationBase  //
//----------------------------//

//-----------------------------------------------------------------------------
AffineTransformationBase::AffineTransformationBase( int order )
  : RCObject(),
    Transformation(),
    _matrix( order + 1, order + 1 )
{
  for ( int i = 0; i < order + 1; i++ )
  {
    _matrix[ i + i* ( order + 1 ) ] = 1.0;
  }
}


//-----------------------------------------------------------------------------
AffineTransformationBase::~AffineTransformationBase()
{
}


//-----------------------------------------------------------------------------
AffineTransformationBase::AffineTransformationBase(
  const AffineTransformationBase& other ) :
  RCObject(),
  Transformation( other ),
  _matrix( other._matrix )
{
  if( other.header() )
    _header = Object::value( other.header()->value<PropertySet>() );
}


//-----------------------------------------------------------------------------
AffineTransformationBase::AffineTransformationBase( const vector<float> & mat )
  : RCObject(),
    Transformation(),
    _matrix( 0, 0 )
{
  int n = int( ceil( sqrt( mat.size() ) ) );
  _matrix.resize( n * n, 0. );
  _matrix.ncols = n;
  int m = mat.size(), l, c;
  for( int i=0; i<m; ++i )
  {
    c = i % n;
    l = i / n;
    _matrix[ c * n + l ] = mat[i];
  }
}


AffineTransformationBase::AffineTransformationBase( const carto::Object mat )
  : RCObject(),
    Transformation(),
    _matrix( 0, 0 )
{
  unsigned n = int( ceil( sqrt( mat->size() ) ) );
  _matrix.resize( n * n, 0.f );
  _matrix.ncols = n;
  unsigned l, c;
  Object it = mat->objectIterator();
  for( unsigned i=0; it->isValid(); ++i, it->next() )
  {
    c = i % n;
    l = i / n;
    _matrix[ c * n + l ] = float( it->currentValue()->getScalar() );
  }
}


AffineTransformationBase & AffineTransformationBase::operator = (
  const AffineTransformationBase& other )
{
  if( &other == this )
    return *this;
  Transformation::operator = ( other );
  _matrix = other._matrix;
  return *this;
}


AffineTransformationBase & AffineTransformationBase::operator = (
  const std::vector<float> & mat )
{
  int n = int( ceil( sqrt( mat.size() ) ) );
  _matrix.resize( n * n, 0. );
  _matrix.ncols = n;
  int m = mat.size(), l, c;
  for( int i=0; i<m; ++i )
  {
    c = i % n;
    l = i / n;
    _matrix[ c * n + l ] = mat[i];
  }

  return *this;
}


AffineTransformationBase & AffineTransformationBase::operator = (
  const carto::Object mat )
{
  size_t m = mat->size();
  int n = int( ceil( sqrt( m ) ) );
  _matrix.resize( n * n, 0. );
  _matrix.ncols = n;
  size_t l, c;
  for( int i=0; i<m; ++i )
  {
    c = i % n;
    l = i / n;
    _matrix[ c * n + l ] = mat->getArrayItem( i )->getScalar();
  }

  return *this;
}


bool AffineTransformationBase::operator == (
  const AffineTransformationBase & other ) const
{
  if( this == &other )
    return true;
  if( _matrix.size() != other._matrix.size() )
    return false;

  static float eps = 1e-5;
  size_t i, n = _matrix.size();
  for( i=0; i<n; ++i )
    if( fabs( _matrix[i] - other._matrix[i] ) >= eps )
      return false;

  return true;
}


AffineTransformationBase & AffineTransformationBase::operator *= (
  const AffineTransformationBase & trans )
{
  size_t i, j, k, n = _matrix.ncols;
  if( trans.matrix().ncols != n )
    throw runtime_error( "transformation matrix size mismatch" );

  vector<float> line( n, 0.f );

  for( i=0; i<n; ++i )
  {
    for( j=0; j<n; ++j )
      line[j] = _matrix( i, j );
    for( j=0; j<n; ++j )
    {
      float & v = _matrix( i, j );
      v = 0;
      for( k=0; k<n; ++k )
        v += line[ k ] * trans.matrix()( k, j );
    }
  }

  return *this;
}


void AffineTransformationBase::negate()
{
  size_t n = _matrix.size() - 1;
  for( int i=0; i<n; ++i )
    _matrix[i] = -_matrix[i];
}


AffineTransformationBase AffineTransformationBase::operator - () const
{
  AffineTransformationBase t( *this );

  t.negate();

  return t;
}


bool AffineTransformationBase::isIdentity() const
{
  size_t n = _matrix.ncols, i, j;
  for( i=0; i<n; ++i )
    for( j=0; j<n; ++j )
      if( _matrix( i, j ) != ( i == j ? 1. : 0.) )
        return false;

  return true;
}


void AffineTransformationBase::setToIdentity()
{
  size_t n = _matrix.ncols, i, j;
  for( i=0; i<n; ++i )
    for( j=0; j<n; ++j )
      _matrix( i, j ) = ( i == j ? 1. : 0.);
}


bool AffineTransformationBase::invertible() const
{
  try
  {
    getInverse();
  }
  catch( ... )
  {
    return false;
  }
  return true;
}


void AffineTransformationBase::extendOrder( unsigned n )
{
  if( order() == n )
    return;

  Table<float> mat = _matrix;
  _matrix.clear();
  _matrix.resize( (n + 1) * (n + 1), 0.f );
  _matrix.ncols = n + 1;
  unsigned i, j, m = mat.ncols - 1;
  if( m > n )
    m = n;
  for( i=0; i<m; ++i )
  {
    for( j=0; j<m; ++j )
      _matrix( i, j ) = mat( i, j );
    _matrix( i, n ) = mat( i, m );
    for( ; j<n; ++j )
    {
      if( i == j )
        _matrix( i, i ) = 1.f;
      else
        _matrix( i, j ) = 0.f;
    }
  }
  for( ; i<=n; ++i )
    for( j=0; j<=n; ++j )
    {
      if( i == j )
        _matrix( i, i ) = 1.f;
      else
        _matrix( i, j ) = 0.f;
    }
}

void AffineTransformationBase::squeezeOrder( unsigned n, bool check,
                                             bool notify_fail )
{
  float eps = 1e-5f;

  if( n >= order() )
    return;

  unsigned i, j, m = order();
  float x;

  if( check )
  {
    for( i=0; i<n; ++i )
      for( j=n; j<m; ++j )
        if( fabs( _matrix( i, j ) - ( i == j ? 1.f : 0.f ) ) > eps )
        {
          if( notify_fail )
            throw runtime_error( "matrix cannot be squeezed" );
          return;
        }
    for( i=n; i<m; ++i )
      for( j=0; j<m; ++j )
        if( fabs( _matrix( i, j ) - ( i == j ? 1.f : 0.f ) ) > eps )
        {
          if( notify_fail )
            throw runtime_error( "matrix cannot be squeezed" );
          return;
        }
  }

  // actually squeeze
  Table<float> matrix( n + 1, n + 1 );
  for( i=0; i<n; ++i )
  {
    for( j=0; j<n; ++j )
      matrix( i, j ) = _matrix( i, j );
    matrix( i, n ) = _matrix( i, m );
  }
  matrix( n, n ) = 1.f;

  _matrix = matrix;
}


//-----------------------------------------------------------------------------
unique_ptr<AffineTransformationBase> AffineTransformationBase::inverse() const
{
  AffineTransformationBase *affineTransformation
    = new AffineTransformationBase;
  affineTransformation->_matrix = inversionLU( _matrix );
  return unique_ptr<AffineTransformationBase>( affineTransformation );
}


std::unique_ptr<Transformation> AffineTransformationBase::getInverse() const
{
  unique_ptr<AffineTransformationBase> ti = inverse();
  unique_ptr<Transformation> res( ti.get() );
  ti.release();
  return res;
}


std::vector<double> AffineTransformationBase::transform(
  const std::vector<double> & pos ) const
{
  if( pos.size() < _matrix.ncols - 1 )
    throw logic_error( "vector is too small" );
  size_t i, k, n =  _matrix.ncols - 1;
  vector<double> tpos = pos;
  for( i=0; i<n; ++i )
  {
    double & v = tpos[i];
    v = 0.;
    for( k=0; k<n; ++k )
      v += _matrix( i, k ) * pos[k];
    v += _matrix( i, n );
  }
  return tpos;
}


//-----------------------------------------------------------------------------
vector<float> AffineTransformationBase::toVector() const
{
  vector<float> vec( ( order() + 1 ) * ( order() + 1 ) );
  int i, j, n = order() + 1;
  for( i=0; i<n; ++i )
    for( j=0; j<n; ++j )
      vec[ i + j * n ] = _matrix[ j + i * n ];

  return vec;
}


//-----------------------------------------------------------------------------
vector<float> AffineTransformationBase::toColumnVector() const
{
  return _matrix;
}


//-----------------------------------------------------------------------------
void AffineTransformationBase::fromColumnVector( const float* vec,
                                                 unsigned size )
{
  int n = int( sqrt( size ) );
  _matrix.clear();
  _matrix.insert( _matrix.begin(), vec, vec + n * n );
}


  //------------------------------//
 //  AffineTransformation3dBase  //
//------------------------------//

//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase()
  : RCObject(),
    Transformation(),
    Transformation3d(),
    AffineTransformationBase( 3 )
{
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::~AffineTransformation3dBase()
{
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase(
  const AffineTransformation3dBase& other ) :
  RCObject(),
  Transformation( other ),
  Transformation3d( other ),
  AffineTransformationBase( other )
{
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase(
  const vector<float> & mat ) :
    RCObject(),
    Transformation(),
    Transformation3d(),
    AffineTransformationBase( mat )
{
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase::AffineTransformation3dBase( const Object mat ) :
    RCObject(),
    Transformation(),
    Transformation3d(),
    AffineTransformationBase( 3 )
{
  *this = mat;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase& AffineTransformation3dBase::operator =
  ( const AffineTransformation3dBase& other )
{
  if( &other == this )
    return *this;
  AffineTransformationBase::operator = ( other );
  return *this;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase& AffineTransformation3dBase::operator =
  ( const vector<float> & other )
{
  AffineTransformationBase::operator = ( other );
  return *this;
}


//-----------------------------------------------------------------------------
AffineTransformation3dBase& AffineTransformation3dBase::operator =
  ( const Object other )
{
  AffineTransformationBase::operator = ( other );
  return *this;
}


//-----------------------------------------------------------------------------
bool AffineTransformation3dBase::operator ==
  ( const AffineTransformation3dBase & other ) const
{
  return AffineTransformationBase::operator == ( other );
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


Point3di
AffineTransformation3dBase::transformInt( int x, int y, int z ) const
{
  return Point3di(
    int( rint( _matrix[0] * x + _matrix[4] * y + _matrix[8] * z
              + _matrix[12] ) ),
    int( rint( _matrix[1] * x + _matrix[5] * y + _matrix[9] * z
               + _matrix[13] ) ),
    int( rint( _matrix[2] * x + _matrix[6] * y + _matrix[10] * z
               + _matrix[14] ) ) );
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


Point3di
AffineTransformation3dBase::transformVectorInt( int x, int y, int z ) const
{
  return Point3di(
    int( rint( _matrix[0] * x + _matrix[4] * y + _matrix[8] * z ) ),
    int( rint( _matrix[1] * x + _matrix[5] * y + _matrix[9] * z ) ),
    int( rint( _matrix[2] * x + _matrix[6] * y + _matrix[10] * z ) ) );
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
unique_ptr<AffineTransformation3dBase> AffineTransformation3dBase::inverse() const
{
  AffineTransformation3dBase *AffineTransformation3d
    = new AffineTransformation3dBase;
  AffineTransformation3d->_matrix = inversionLU( _matrix );
  return unique_ptr<AffineTransformation3dBase>( AffineTransformation3d );
}


bool AffineTransformation3dBase::invertible() const
{
  // inverse() will throw carto::assert_error if matrix inversion fails
  try
  {
    getInverse();
  }
  catch( const assert_error& )
  {
    return false;
  }
  return true;
}

unique_ptr<Transformation> AffineTransformation3dBase::getInverse() const
{
  unique_ptr<AffineTransformation3dBase> pi = inverse();
  unique_ptr<Transformation> res( pi.get() );
  pi.release();
  return res;
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
To be more relevant (and consistent with the name of the method), one should
even use
		translation() = t;
however, it should be noted that for the moment, the information about the
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


void AffineTransformation3dBase::extendOrder( unsigned n )
{
  if( n != 3 )
    throw logic_error( "only order 3 is allowed for an "
                       "AffineTransformation3dBase" );
}


AffineTransformation3dBase & AffineTransformation3dBase::operator *= (
  const AffineTransformation3dBase & m )
{
  Point3df	r2 = transform( m._matrix[12], m._matrix[13], m._matrix[14] );
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
  AffineTransformation3dBase t( *this );

  t.negate();

  return t;
}


void AffineTransformation3dBase::fromColumnVector( const float* vec )
{
  _matrix.clear();
  _matrix.insert( _matrix.begin(), vec, vec + 16 );
  _matrix[3] = 0.f;
  _matrix[7] = 0.f;
  _matrix[11] = 0.f;
  _matrix[15] = 1.f;
}


//-----------------------------------------------------------------------------
namespace soma
{

  AffineTransformation3dBase operator * (
    const AffineTransformation3dBase& AffineTransformation3d1,
    const AffineTransformation3dBase& AffineTransformation3d2 )
  {
    AffineTransformation3dBase AffineTransformation3dOut;

    const AffineTransformation3dBase::Table< float >
      & mat1 = AffineTransformation3d1.matrix(),
      & mat2 = AffineTransformation3d2.matrix();
    AffineTransformation3dBase::Table< float >
      & mat3 = AffineTransformation3dOut.matrix();
    // init mat3 to null, not to ID
    mat3( 0, 0 ) = 0.f;
    mat3( 1, 1 ) = 0.f;
    mat3( 2, 2 ) = 0.f;
    mat3( 3, 3 ) = 0.f;

    unsigned i, j, k;

    for( j=0; j<4; ++j )
      for( i=0; i<4; ++i )
        for(k=0;k<4;++k)
          mat3(i,j) += mat1(i,k) * mat2(k,j);

    return AffineTransformation3dOut;
  }


  AffineTransformationBase operator * (
    const AffineTransformationBase& AffineTransformation1,
    const AffineTransformationBase& AffineTransformation2 )
  {
    if( AffineTransformation1.order() != AffineTransformation2.order() )
      throw logic_error( "mismatching matrices dimensions for *" );

    unsigned i, n = AffineTransformation1.order();
    AffineTransformationBase AffineTransformationOut( n );

    const AffineTransformationBase::Table< float >
      & mat1 = AffineTransformation1.matrix(),
      & mat2 = AffineTransformation2.matrix();
    AffineTransformationBase::Table< float >
      & mat3 = AffineTransformationOut.matrix();
    // init mat3 to null, not to ID
    ++n;
    for( i=0; i<n; ++i )
      mat3( i, i ) = 0.f;

    unsigned j, k;

    for( j=0; j<n; ++j )
      for( i=0; i<n; ++i )
        for(k=0;k<n;++k)
          mat3(i,j) += mat1(i,k) * mat2(k,j);

    return AffineTransformationOut;
  }


  ostream& operator << ( ostream& os,
                        const soma::AffineTransformationBase & thing )
  {
    os << "[";
    size_t i, j, n = thing.order();
    for( i=0; i<n + 1; ++i )
    {
      os << "[ ";
      for( j=0; j<n; ++j )
        os << setw(8) << thing.matrix()(i, j) << ", ";
      os << setw(8) << thing.matrix()(i, n) << " ]";
      if( i < n )
        os << ",\n ";
    }
    os << "]";

    return os;
  }

}

