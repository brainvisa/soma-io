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

#ifndef SOMAIO_CHECKER_TRANSFORMATION_H
#define SOMAIO_CHECKER_TRANSFORMATION_H

#include <cartobase/object/object.h>
#include <soma-io/config/soma_config.h>
#include <vector>

namespace soma
{

  // -- points and transformations

  class Point3df : public std::vector<float>
  {
  public:

    Point3df( float x=0.F, float y=0.F, float z=0.F ) : std::vector<float>(3)
    {
      (*this)[0] = x;
      (*this)[1] = y;
      (*this)[2] = z;
    }

    Point3df operator +=( const Point3df & p )
    {
      (*this)[0] += p[0];
      (*this)[1] += p[1];
      (*this)[2] += p[2];
      return *this;
    }

    Point3df operator -=( const Point3df & p )
    {
      (*this)[0] -= p[0];
      (*this)[1] -= p[1];
      (*this)[2] -= p[2];
      return *this;
    }

    Point3df operator +( const Point3df & p ) const
    {
      Point3df p2 = *this;
      p2 += p;
      return p2;
    }

    Point3df operator -( const Point3df & p ) const
    {
      Point3df p2 = *this;
      p2 -= p;
      return p2;
    }
  };


  template <typename T>
  class Table : public std::vector<T>
  {
  public:
    Table(unsigned ncols, unsigned nlines)
      : std::vector<T>(ncols*nlines, 0), ncols(ncols) {}
    Table(unsigned nitems)
      : std::vector<T>(nitems, 0), ncols(4) {}
    Table( const Table & other )
      : std::vector<T>()
    {
      *this = other;
    }

    T & operator () ( int x, int y=0 )
    {
      return (*this)[ y*ncols + x ];
    }

    T operator () ( int x, int y=0 ) const
    {
      return (*this)[ y*ncols + x ];
    }

    Table & operator = ( const Table & other )
    {
      if( &other == this )
        return *this;
      ncols = other.ncols;
      this->std::vector<T>::operator = ( other );
      return *this;
    }

    unsigned dimX() const { return ncols; }
    unsigned ncols;
  };


  class AffineTransformation3d
  {
  public:

    AffineTransformation3d();
    AffineTransformation3d( const AffineTransformation3d& other );
    /// Create a AffineTransformation3d from a 4x4 matrix given as a line vector
    AffineTransformation3d( const std::vector<float> & mat );
    /// Create a AffineTransformation3d from a 4x4 matrix given as a line vector in an Object
    AffineTransformation3d( const carto::Object mat );
    ~AffineTransformation3d();
    AffineTransformation3d &operator = ( const AffineTransformation3d& other );
    AffineTransformation3d &operator = ( const std::vector<float> & mat );
    AffineTransformation3d &operator = ( const carto::Object mat );
    void setToIdentity() ;

    bool operator == ( const AffineTransformation3d & );

    AffineTransformation3d inverse() const;
    std::vector<float> toVector() const;

    Point3df transform( float x, float y, float z ) const;
    Point3df transform( const Point3df & ) const;

    Table<float> matrix;
  };


  AffineTransformation3d operator * (
    const AffineTransformation3d& AffineTransformation3d1,
    const AffineTransformation3d& AffineTransformation3d2 );


  //
  inline Point3df
  AffineTransformation3d::transform( const Point3df & p ) const
  {
    return transform( p[0], p[1], p[2] );
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

}

#endif
