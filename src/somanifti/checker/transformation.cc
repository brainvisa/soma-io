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

#include "transformation.h"
#include "inversion_lu.cc" // FIXME

using namespace soma;
using namespace carto;
using namespace std;

//-----------------------------------------------------------------------------
AffineTransformation3d::AffineTransformation3d()
  : matrix( 16 )
{
  setToIdentity();
}


//-----------------------------------------------------------------------------
AffineTransformation3d::~AffineTransformation3d()
{
}


//-----------------------------------------------------------------------------
AffineTransformation3d::AffineTransformation3d(
  const AffineTransformation3d& other ) : matrix( 4, 4 )
{
  for ( int i = 0; i < 16; i++ )
    matrix[i] = other.matrix[i];
}


//-----------------------------------------------------------------------------
AffineTransformation3d::AffineTransformation3d( const vector<float> & mat )
  : matrix( 4, 4 )
{
  *this = mat;
}


//-----------------------------------------------------------------------------
AffineTransformation3d::AffineTransformation3d( const Object mat )
  : matrix( 4, 4 )
{
  *this = mat;
}


//-----------------------------------------------------------------------------
AffineTransformation3d& AffineTransformation3d::operator = ( const AffineTransformation3d& other )
{
  matrix = other.matrix;
  return *this;
}


//-----------------------------------------------------------------------------
AffineTransformation3d& AffineTransformation3d::operator = ( const vector<float> & mat )
{
  int n = mat.size();
  for ( int i = 0; i < n; i++ )
    matrix[(i%4)*4+(i/4)] = mat[i];

  return *this;
}


//-----------------------------------------------------------------------------
AffineTransformation3d& AffineTransformation3d::operator = ( const Object mat )
{
  if( mat->size() < 12 )
    return *this; // or throw something ?
  Object  iter = mat->objectIterator();
  if( !iter->isValid() )
    return *this; // or throw something ?
  int n = mat->size();
  for ( int i = 0; i < n; i++ )
  {
    matrix[ (i%4)*4+(i/4) ] = iter->currentValue()->getScalar();
    iter->next();
  }
  return *this;
}


inline Point3df
AffineTransformation3d::transform( float x, float y, float z ) const
{
  return Point3df( matrix[0] * x + matrix[4] * y
                   + matrix[8] * z + matrix[12],
                   matrix[1] * x + matrix[5] * y
                   + matrix[9] * z + matrix[13],
                   matrix[2] * x + matrix[6] * y
                   + matrix[10] * z + matrix[14]
                   );
}


inline Point3df
AffineTransformation3d::transform( const Point3df & p ) const
{
  return transform( p[0], p[1], p[2] );
}


//-----------------------------------------------------------------------------
void AffineTransformation3d::setToIdentity()
{
  for( int i = 0; i < 16; i++ )
    matrix[i] = 0.;
  for( int i = 0; i < 4; i++ )
    matrix[i*5] = 1.0;
}


//-----------------------------------------------------------------------------
AffineTransformation3d AffineTransformation3d::inverse() const
{
  AffineTransformation3d AffineTransformation3d;
  AffineTransformation3d.matrix = inversionLU( matrix );
  return AffineTransformation3d;
}


vector<float> AffineTransformation3d::toVector() const
{
  vector<float> vec( 16 );
  for ( int i = 0; i < 16; i++ )
    vec[ (i%4)*4 + (i/4) ] = matrix[i];
  return vec;
}

//-----------------------------------------------------------------------------
AffineTransformation3d soma::operator * (const AffineTransformation3d& AffineTransformation3d1, const AffineTransformation3d& AffineTransformation3d2)
{
  AffineTransformation3d AffineTransformation3dOut;

  Table<float> mat1(4, 4), mat2(4, 4), mat3(4, 4);

  int i, j, k;

  for(i=0;i<3;++i)
    for(j=0;j<3;++j)
    {
      mat1(j, i) = AffineTransformation3d1.matrix[j*4+i];
      mat2(j, i) = AffineTransformation3d2.matrix[j*4+i];
    }

  for(i=0;i<3;++i)
  {
    mat1(3, i) = AffineTransformation3d1.matrix(i, 3);
    mat2(3, i) = AffineTransformation3d2.matrix(i, 3);
    mat1(i, 3) = mat2(i, 3) = 0;
  }

  mat1[15] = mat2[15] = 1.0;

  for( j=0; j<4; ++j )
    for( i=0; i<4; ++i )
    {
      for(k=0;k<4;++k)
        mat3(i, j) += mat2(i, k)*mat1(k, j);
    }


  for(i=0;i<3;++i)
  {
    AffineTransformation3dOut.matrix(i, 3) = mat3(3, i);
  }

  for(i=0;i<3;++i)
    for(j=0;j<3;++j)
    {
      AffineTransformation3dOut.matrix(j, i) = mat3(i, j);
    }

  return AffineTransformation3dOut;
}

