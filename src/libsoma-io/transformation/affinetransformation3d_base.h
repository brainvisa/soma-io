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

/*
 *  Non elastic AffineTransformation3d ( rotation + translation )
 */
#ifndef SOMAIO_TRANSFORMATION_AFFINETRANSFORMATION3D_BASE_H
#define SOMAIO_TRANSFORMATION_AFFINETRANSFORMATION3D_BASE_H

#include <soma-io/transformation/transformation.h>
#include <soma-io/config/soma_config.h>
#include <iosfwd>

namespace soma
{

  //---------------------------------------------------------------------------

    //--------------------------//
   //  AffineTransformation3d  //
  //--------------------------//
  //---------------------------------------------------------------------------
  /** Affine 3D transformation

  WARNING: this is the old Motion class of Aims 3.x, which has been renamed and
  changed in Aims 4.0. It has been partly moved to Soma-IO in soma-io/aims 4.7
  since it is needed in IO systems.

  A new transformation classes tree has been setup to allow non-linear
  transformations: see Transformation and Transformation3d.
  AffineTransformation3d now inherits Transformation3d.

  A typedef is still provided for backward compatibility, in
  aims/resampling/motion.h (the former location of the Motion class) and should
  minimize inconvenience when compiling old code which used Motion. However a few
  API differences may cause compilation problems:

  - forward declarations for the Motion class will not work any longer, since
    Motion is not a class anymore but a typedef. It is still possible to replace:
  \code class Motion;
  \endcode
    by:
  \code namespace aims { class AffineTransformation3d; }
  typedef aims::AffineTransformation3d Motion;
  \endcode
    but of course it is better to use directly the AffineTransformation3d class
    under its real name.

  - the Motion::transform_normal() method is now transformUnitNormal()

  - there are now several overloaded transform() methods taking double, or float
    numbers, or Point3df or Point3dd arguments. As there were formerly only float
    and Point3df arguments, ambiguities may raise from calling them with mixed
    double/float arguments

  - the base aims::Transformation class introduces a name ambiguity with
    anatomist anatomist::Transformation class, so it now requires to handle
    namespaces carefully.

  - the Motion DataTypeCode has also changed to AffineTransformation3d.
  */
  class AffineTransformation3dBase : public Transformation3d
  {
  public:

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
      Table( const std::vector<T> & other )
        : std::vector<T>( other ), ncols( 4 )
      {
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


    /// Create an identity transformation
    AffineTransformation3dBase();
    AffineTransformation3dBase( const AffineTransformation3dBase& other );
    /// Create a AffineTransformation3d from a 4x4 matrix given as a line vector
    AffineTransformation3dBase( const std::vector<float> & mat );
    /// Create a AffineTransformation3d from a 4x4 matrix given as a line vector in an Object
    AffineTransformation3dBase( const carto::Object mat );
    virtual ~AffineTransformation3dBase();
    virtual AffineTransformation3dBase &operator = ( const AffineTransformation3dBase& other );
    virtual AffineTransformation3dBase &operator = ( const std::vector<float> & mat );
    virtual AffineTransformation3dBase &operator = ( const carto::Object mat );

    virtual bool operator == ( const AffineTransformation3dBase & );

    AffineTransformation3dBase & operator *= (
      const AffineTransformation3dBase & trans );
    AffineTransformation3dBase operator - () const;

    Point3dd transformVector( const Point3dd & vec ) const;
    Point3df transformVector( const Point3df & dir ) const;
    Point3dd transformVector( double x, double y, double z ) const;
    Point3df transformVector( float x, float y, float z ) const;
    Point3dd transformNormal( const Point3dd & dir ) const;
    Point3df transformNormal( const Point3df & dir ) const;
    Point3dd transformNormal( double x, double y, double z ) const;
    Point3df transformNormal( float x, float y, float z ) const;
    Point3dd transformUnitNormal( const Point3dd & dir ) const;
    Point3df transformUnitNormal( const Point3df & dir ) const;
    Point3dd transformUnitNormal( double x, double y, double z ) const;
    Point3df transformUnitNormal( float x, float y, float z ) const;

    bool isIdentity() const CARTO_OVERRIDE;
    virtual void setToIdentity();

    // AffineTransformation3d algebraic operation
    AffineTransformation3dBase inverse() const;
    bool invertible() const CARTO_OVERRIDE;
    std::unique_ptr<Transformation3d> getInverse() const CARTO_OVERRIDE;
    virtual void scale( const Point3df& sizeFrom, const Point3df& sizeTo );
    /// true if the transformation is direct, false if it changes orientation
    bool isDirect() const;

    //Initialisation
    void setTranslation(Point3df trans);
    /// conversions and IO)
    std::vector<float> toVector() const;
    /// transform AffineTransformation3d to a column vector (useful for
    /// conversions to OpenGL matrices)
    std::vector<float> toColumnVector() const;
    /// transform a column vector to an AffineTransformation3d (useful for
    /// conversions from OpenGL matrices)
    void fromColumnVector( const std::vector<float> & vec )
    {
      return fromColumnVector( &vec[0] );
    }
    /// transform a column vector to an AffineTransformation3d (useful for
    /// conversions from OpenGL matrices)
    void fromColumnVector( const float* vec );
    Table<float> & matrix() { return _matrix; }
    const Table<float> & matrix() const { return _matrix; }

  protected:
    Point3dd transformDouble( double x, double y, double z ) const CARTO_OVERRIDE;
    Point3df transformFloat( float x, float y, float z ) const CARTO_OVERRIDE;

    virtual Point3dd transformVectorPoint3dd( const Point3dd & vec ) const;
    virtual Point3df transformVectorPoint3df( const Point3df & dir ) const;
    virtual Point3dd transformVectorDouble( double x, double y, double z ) const;
    virtual Point3df transformVectorFloat( float x, float y, float z ) const;
    virtual Point3dd transformNormalPoint3dd( const Point3dd & dir ) const;
    virtual Point3df transformNormalPoint3df( const Point3df & dir ) const;
    virtual Point3dd transformNormalDouble( double x, double y, double z ) const;
    virtual Point3df transformNormalFloat( float x, float y, float z ) const;

    // column-vector to be compatible with the former Volume storage with
    // math notation
    Table<float> _matrix;
  };


  // Compose AffineTransformation3d
  soma::AffineTransformation3dBase
    operator * ( const soma::AffineTransformation3dBase&
                    affineTransformation3d1,
                 const soma::AffineTransformation3dBase&
                    affineTransformation3d2 );

  std::ostream&
    operator << ( std::ostream& os,
                  const soma::AffineTransformation3dBase & thing );

  inline Point3dd
  AffineTransformation3dBase::transformVector( double x, double y, double z ) const
  {
    return transformVectorDouble( x, y, z );
  }


  inline Point3df
  AffineTransformation3dBase::transformVector( float x, float y, float z ) const
  {
    return transformVectorFloat( x, y, z );
  }


  inline Point3df
  AffineTransformation3dBase::transformVector( const Point3df & pos ) const
  {
    return transformVectorPoint3df( pos );
  }


  inline Point3dd
  AffineTransformation3dBase::transformVector( const Point3dd & pos ) const
  {
    return transformVectorPoint3dd( pos );
  }


  inline Point3dd
  AffineTransformation3dBase::transformNormal( double x, double y, double z ) const
  {
    return transformNormalDouble( x, y, z );
  }


  inline Point3df
  AffineTransformation3dBase::transformNormal( float x, float y, float z ) const
  {
    return transformNormalFloat( x, y, z );
  }


  inline Point3df
  AffineTransformation3dBase::transformNormal( const Point3df & pos ) const
  {
    return transformNormalPoint3df( pos );
  }


  inline Point3dd
  AffineTransformation3dBase::transformNormal( const Point3dd & pos ) const
  {
    return transformNormalPoint3dd( pos );
  }


  inline Point3df
  AffineTransformation3dBase::transformVectorPoint3df( const Point3df & pos ) const
  {
    return transformVectorFloat( pos[0], pos[1], pos[2] );
  }


  inline Point3dd
  AffineTransformation3dBase::transformVectorPoint3dd( const Point3dd & pos ) const
  {
    return transformVector( pos[0], pos[1], pos[2] );
  }


  inline Point3df
  AffineTransformation3dBase::transformNormalPoint3df( const Point3df & pos ) const
  {
    Point3dd transformed = transformNormal( (double) pos[0], (double) pos[1],
                                             (double) pos[2] );
    return Point3df( (float) transformed[0], (float) transformed[1],
                     (float) transformed[2] );
  }


  inline Point3dd
  AffineTransformation3dBase::transformNormalPoint3dd( const Point3dd & pos ) const
  {
    return transformNormal( pos[0], pos[1], pos[2] );
  }


  inline Point3df
  AffineTransformation3dBase::
  transformNormalFloat( float x, float y, float z ) const
  {
    Point3dd transformed = transformNormal( (double) x, (double) y,
                                             (double) z );
    return Point3df( (float) transformed[0], (float) transformed[1],
                      (float) transformed[2] );
  }


  inline Point3dd
  AffineTransformation3dBase::
  transformUnitNormal( double x, double y, double z ) const
  {
    return transformNormal( x, y, z ).normalize();
  }


  inline Point3df
  AffineTransformation3dBase::transformUnitNormal( const Point3df & pos ) const
  {
    Point3dd transformed
        = transformUnitNormal( (double) pos[0], (double) pos[1],
                                (double) pos[2] );
    return Point3df( (float) transformed[0], (float) transformed[1],
                      (float) transformed[2] );
  }


  inline Point3dd
  AffineTransformation3dBase::transformUnitNormal( const Point3dd & pos ) const
  {
    return transformUnitNormal( pos[0], pos[1], pos[2] );
  }


  inline Point3df
  AffineTransformation3dBase::
  transformUnitNormal( float x, float y, float z ) const
  {
    Point3dd transformed = transformUnitNormal( (double) x, (double) y,
                                                (double) z );
    return Point3df( (float) transformed[0], (float) transformed[1],
                     (float) transformed[2] );
  }

}


#endif

