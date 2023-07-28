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

#ifndef SOMAIO_TRANSFORMATION_TRANSFORMATION_H
#define SOMAIO_TRANSFORMATION_TRANSFORMATION_H
/*
 *  These classes used to be in aimsdata, but they are used in Transformation
 *  classes, which are needed in IO, so they have been moved to soma-io
 *  in AIMS 4.7.
 */

#include <stdexcept>

#include <soma-io/vector/vector.h>
#include <cartobase/smart/rcptr.h>
#include <cartobase/type/types.h>

namespace soma
{

  /** Polymorphic base class for spatial transformations.

      In Soma-IO 5.1 (2022) the header() has beed added in the base
      Transformation class, whereas it used to be only in Aims
      AffineTransformation3d. Now all transformations variants have a header.
   */
  class Transformation : public virtual carto::RCObject
  {
  public:
    virtual ~Transformation();

    virtual Transformation & operator = ( const Transformation & other );

    /** Test if the transformation can safely be omitted

        This method returns true only if the transformation behaves exactly
        like an identity transformation (notably, the transform methods will
        always return the input coordinates unchanged).

        \note{Implementors of derived classes may choose to always return false
        if a test would be difficult to implement or expensive to run. As a
        result, a false result does not guarantee in general that the
        transformation is different from identity.}
     */
    virtual bool isIdentity() const
    {
      return false;
    }

    carto::Object header() { return _header; }
    const carto::Object header() const { return _header; }
    void setHeader( carto::Object ph );

    virtual std::vector<double>
      transform( const std::vector<double> & pos ) const = 0;
    virtual std::vector<float>
      transform( const std::vector<float> & pos ) const;
    virtual std::vector<int> transform( const std::vector<int> & pos ) const;

    virtual std::vector<double>
      transformVector( const std::vector<double> & pos ) const;
    virtual std::vector<float>
      transformVector( const std::vector<float> & pos ) const;
    virtual std::vector<int>
      transformVector( const std::vector<int> & pos ) const;

    /** Test if the transformation can be inverted

        getInverse() can be called if this method returns true, in order to
        obtain the inverse transformation.
     */
    virtual bool invertible() const
    {
      return false;
    };
    /** Obtain the inverse transformation

        This method should only be called if invertible() returns true.
        Otherwise, it will throw an exception if the transformation is not
        actually invertible.
     */
    virtual std::unique_ptr<Transformation> getInverse() const
    {
      throw std::logic_error("not implemented");
    }

    /// true if the transformation is direct, false if it changes orientation
    virtual bool isDirect() const = 0;

    /// vector arithmetics, as convenience static functions
    template <typename T>
    static std::vector<T> vadd( const std::vector<T> & v1,
                                const std::vector<T> & v2 );
    /// vector arithmetics, as convenience static functions
    template <typename T>
    static std::vector<T> vsub( const std::vector<T> & v1,
                                const std::vector<T> & v2 );
    /// vector arithmetics, as convenience static functions
    template <typename T>
    static std::vector<T> vadd( const std::vector<T> & v1,
                                const T & v2 );
    /// vector arithmetics, as convenience static functions
    template <typename T>
    static std::vector<T> vsub( const std::vector<T> & v1,
                                const T & v2 );
    /// vector arithmetics, as convenience static functions
    template <typename T>
    static std::vector<T> vadd( const T & v1,
                                const std::vector<T> & v2 );
    /// vector arithmetics, as convenience static functions
    template <typename T>
    static std::vector<T> vsub( const T & v1,
                                const std::vector<T> & v2 );

  protected:
    carto::Object _header;

    Transformation();
  };


  /** Polymorphic base class for spatial transformations in 3D
   */
  class Transformation3d : public virtual Transformation
  {
  public:
    virtual ~Transformation3d();

    Point3dd transform( double x, double y, double z ) const;
    Point3dd transform( const Point3dd & pos ) const;
    Point3df transform( const Point3df & dir ) const;
    Point3df transform( float x, float y, float z ) const;
    Point3d transform( const Point3d & p ) const;
    Point3di transform( const Point3di & p ) const;
    Point3di transform( int x, int y, int z ) const;
    virtual std::vector<double>
      transform( const std::vector<double> & pos ) const;
    virtual std::vector<float>
      transform( const std::vector<float> & pos ) const;
    virtual std::vector<int> transform( const std::vector<int> & pos ) const;

    virtual std::vector<double>
      transformVector( const std::vector<double> & pos ) const;
    virtual std::vector<float>
      transformVector( const std::vector<float> & pos ) const;
    virtual std::vector<int>
      transformVector( const std::vector<int> & pos ) const;

  protected:
    Transformation3d() : Transformation() {}

    virtual Point3dd transformDouble( double x, double y, double z ) const = 0;
    virtual Point3dd transformPoint3dd( const Point3dd & pos ) const;
    virtual Point3df transformPoint3df( const Point3df & dir ) const;
    virtual Point3d transformPoint3d( const Point3d & p ) const;
    virtual Point3di transformPoint3di( const Point3di & p ) const;
    virtual Point3df transformFloat( float x, float y, float z ) const;
    virtual Point3di transformInt( int x, int y, int z ) const;
  };


  // --

  inline std::vector<float>
  Transformation::transform( const std::vector<float> & pos ) const
  {
    std::vector<double> tr
      = transform( std::vector<double>( pos.begin(), pos.end() ) );
    return std::vector<float>( tr.begin(), tr.end() );
  }


  inline std::vector<int>
  Transformation::transform( const std::vector<int> & pos ) const
  {
    std::vector<double> tr
      = transform( std::vector<double>( pos.begin(), pos.end() ) );
    std::vector<int> itr( tr.size() );
    for( size_t i=0; i<tr.size(); ++i )
      itr[i] = int( rint( tr[i] ) );

    return itr;
  }


  inline std::vector<double>
  Transformation::transformVector( const std::vector<double> & pos ) const
  {
    std::vector<double> tr = transform( pos );
    std::vector<double> tr0
      = transform( std::vector<double>( pos.size(), 0. ) );
    for( size_t i=0; i<tr.size(); ++i )
      tr[i] -= tr0[i];
    return tr;
  }


  inline std::vector<float>
  Transformation::transformVector( const std::vector<float> & pos ) const
  {
    std::vector<double> tr
      = transformVector( std::vector<double>( pos.begin(), pos.end() ) );
    return std::vector<float>( tr.begin(), tr.end() );
  }


  inline std::vector<int>
  Transformation::transformVector( const std::vector<int> & pos ) const
  {
    std::vector<double> tr
      = transformVector( std::vector<double>( pos.begin(), pos.end() ) );
    std::vector<int> itr( tr.size() );
    for( size_t i=0; i<tr.size(); ++i )
      itr[i] = int( rint( tr[i] ) );

    return itr;
  }


  // --

  inline Point3dd
  Transformation3d::transform( double x, double y, double z ) const
  {
    return transformDouble( x, y, z );
  }


  inline Point3df
  Transformation3d::transform( float x, float y, float z ) const
  {
    return transformFloat( x, y, z );
  }


  inline Point3di
  Transformation3d::transform( int x, int y, int z ) const
  {
    return transformInt( x, y, z );
  }


  inline Point3df Transformation3d::transform( const Point3df & pos ) const
  {
    return transformPoint3df( pos );
  }


  inline Point3dd
  Transformation3d::transform( const Point3dd & pos ) const
  {
    return transformPoint3dd( pos );
  }


  inline Point3d
  Transformation3d::transform( const Point3d & pos ) const
  {
    return transformPoint3d( pos );
  }


  inline Point3di
  Transformation3d::transform( const Point3di & pos ) const
  {
    return transformPoint3di( pos );
  }


  inline Point3df
  Transformation3d::transformPoint3df( const Point3df & pos ) const
  {
    Point3dd transformed = transform( (double) pos[0], (double) pos[1],
                                      (double) pos[2] );
    return Point3df( (float) transformed[0], (float) transformed[1],
                     (float) transformed[2] );
  }


  inline Point3dd
  Transformation3d::transformPoint3dd( const Point3dd & pos ) const
  {
    return transform( pos[0], pos[1], pos[2] );
  }


  inline Point3df
  Transformation3d::transformFloat( float x, float y, float z ) const
  {
    Point3dd transformed = transform( (double) x, (double) y, (double) z );
    return Point3df( (float) transformed[0], (float) transformed[1],
                     (float) transformed[2] );
  }


  inline Point3d Transformation3d::transformPoint3d( const Point3d & p ) const
  {
    Point3dd transformed = transform( (double) p[0], (double) p[1],
                                      (double) p[2] );
    return Point3d( (int16_t) rint( transformed[0] ),
                    (int16_t) rint( transformed[1] ),
                    (int16_t) rint( transformed[2] ) );
  }


  inline Point3di
  Transformation3d::transformPoint3di( const Point3di & p ) const
  {
    Point3dd transformed = transform( (double) p[0], (double) p[1],
                                      (double) p[2] );
    return Point3di( (int) rint( transformed[0] ),
                     (int) rint( transformed[1] ),
                     (int) rint( transformed[2] ) );
  }


  inline Point3di
  Transformation3d::transformInt( int x, int y, int z ) const
  {
    Point3dd transformed = transform( (double) x, (double) y, (double) z );
    return Point3di( (int) rint( transformed[0] ),
                     (int) rint( transformed[1] ),
                     (int) rint( transformed[2] ) );
  }


  inline std::vector<double>
  Transformation3d::transform( const std::vector<double> & pos ) const
  {
    std::vector<double> tr = pos;
    Point3dd p = transform( pos[0], pos[1], pos[2] );
    tr[0] = p[0];
    tr[1] = p[1];
    tr[2] = p[2];
    return tr;
  }


  inline std::vector<float>
  Transformation3d::transform( const std::vector<float> & pos ) const
  {
    std::vector<float> tr = pos;
    Point3df p = transform( pos[0], pos[1], pos[2] );
    tr[0] = p[0];
    tr[1] = p[1];
    tr[2] = p[2];
    return tr;
  }


  inline std::vector<int>
  Transformation3d::transform( const std::vector<int> & pos ) const
  {
    std::vector<int> tr = pos;
    Point3di p = transform( pos[0], pos[1], pos[2] );
    tr[0] = p[0];
    tr[1] = p[1];
    tr[2] = p[2];
    return tr;
  }


  inline std::vector<double>
  Transformation3d::transformVector( const std::vector<double> & pos ) const
  {
    std::vector<double> tr = pos;
    Point3dd p = transform( pos[0], pos[1], pos[2] )
      - transform( Point3dd( 0., 0., 0. ) );
    tr[0] = p[0];
    tr[1] = p[1];
    tr[2] = p[2];
    return tr;
  }


  inline std::vector<float>
  Transformation3d::transformVector( const std::vector<float> & pos ) const
  {
    std::vector<float> tr = pos;
    Point3df p = transform( pos[0], pos[1], pos[2] )
      - transform( Point3df( 0.f, 0.f, 0.f ) );
    tr[0] = p[0];
    tr[1] = p[1];
    tr[2] = p[2];
    return tr;
  }


  inline std::vector<int>
  Transformation3d::transformVector( const std::vector<int> & pos ) const
  {
    std::vector<int> tr = pos;
    Point3di p = transform( pos[0], pos[1], pos[2] )
      - transform( Point3di( 0, 0, 0 ) );
    tr[0] = p[0];
    tr[1] = p[1];
    tr[2] = p[2];
    return tr;
  }

  // -- vector arithmetics

  template <typename T>
  inline
  std::vector<T> Transformation::vadd( const std::vector<T> & v1,
                                       const std::vector<T> & v2 )
  {
    size_t n1 = v1.size(), n2 = v2.size(), n3 = std::max( n1, n2 ),
      n4 = std::min( n1, n2 ), i;
    std::vector<T> res( n3, 0 );

    for( i=0; i<n4; ++i )
      res[i] = v1[i] + v2[i];
    for( ; i<n1; ++i )
      res[i] = v1[i];
    for( ; i<n2; ++i )
      res[i] = v2[i];
    for( ; i<n3; ++i )
      res[i] = 0;

    return res;
  }


  template <typename T>
  inline
  std::vector<T> Transformation::vsub( const std::vector<T> & v1,
                                       const std::vector<T> & v2 )
  {
    size_t n1 = v1.size(), n2 = v2.size(), n3 = std::max( n1, n2 ),
      n4 = std::min( n1, n2 ), i;
    std::vector<T> res( n3, 0 );

    for( i=0; i<n4; ++i )
      res[i] = v1[i] - v2[i];
    for( ; i<n1; ++i )
      res[i] = v1[i];
    for( ; i<n2; ++i )
      res[i] = -v2[i];
    for( ; i<n3; ++i )
      res[i] = 0;

    return res;
  }


  template <typename T>
  inline
  std::vector<T> Transformation::vadd( const std::vector<T> & v1,
                                       const T & v2 )
  {
    unsigned n1 = v1.size(), i;
    std::vector<T> res( n1, 0 );

    for( i=0; i<n1; ++i )
      res[i] = v1[i] + v2;

    return res;
  }


  template <typename T>
  inline
  std::vector<T> Transformation::vsub( const std::vector<T> & v1,
                                       const T & v2 )
  {
    unsigned n1 = v1.size(), i;
    std::vector<T> res( n1, 0 );

    for( i=0; i<n1; ++i )
      res[i] = v1[i] - v2;

    return res;
  }


  template <typename T>
  inline
  std::vector<T> Transformation::vadd( const T & v1,
                                       const std::vector<T> & v2 )
  {
    unsigned n1 = v2.size(), i;
    std::vector<T> res( n1, 0 );

    for( i=0; i<n1; ++i )
      res[i] = v1 + v2[i];

    return res;
  }


  template <typename T>
  inline
  std::vector<T> Transformation::vsub( const T & v1,
                                       const std::vector<T> & v2 )
  {
    unsigned n1 = v2.size(), i;
    std::vector<T> res( n1, 0 );

    for( i=0; i<n1; ++i )
      res[i] = v1 - v2[i];

    return res;
  }

  // --

  template <> inline std::string DataTypeCode<Transformation3d>::objectType()
  {
    return "Transformation3d";
  }

  template <> inline std::string DataTypeCode<Transformation3d>::dataType()
  {
    return "VOID";
  }

  template <> inline std::string DataTypeCode<Transformation3d>::name()
  {
    return "Transformation3d";
  }

}

#endif
