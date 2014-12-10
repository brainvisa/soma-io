#include <soma-io/Transformation/AxialTransformation3d.h>

#include <cmath>


soma::AxialTransformation3d::AxialTransformation3d()
                           : soma::Transformation3d()
{
}


soma::AxialTransformation3d::AxialTransformation3d( 
                                               const soma::Vector& rowCosine,
                                               const soma::Vector& columnCosine,
                                               const soma::Vector& normal,
                                               const soma::Vector& origin  )
                           : soma::Transformation3d()
{

  soma::Vector rowVec = rowCosine.mainDirection();
  soma::Vector columnVec = columnCosine.mainDirection();
  soma::Vector normalVec = normal.mainDirection();

  setItems(  rowVec.x,  columnVec.x,  normalVec.x, origin.x,
             rowVec.y,  columnVec.y,  normalVec.y, origin.y,
            -rowVec.z, -columnVec.z, -normalVec.z, origin.z,
            0.0,          0.0,             0.0,    1.0 );

}


soma::AxialTransformation3d::AxialTransformation3d( 
                                      const soma::AxialTransformation3d& other )
                           : soma::Transformation3d( other )
{
}


void soma::AxialTransformation3d::setTranslation( 
                                               const soma::Vector& translation )
{

  m_direct[ 0 ][ 3 ] = std::fabs( m_direct[ 0 ][ 2 ] * translation.x );
  m_direct[ 1 ][ 3 ] = std::fabs( m_direct[ 1 ][ 2 ] * translation.y );
  m_direct[ 2 ][ 3 ] = std::fabs( m_direct[ 2 ][ 2 ] * translation.z );

  invert( m_direct, m_inverse );

}


void soma::AxialTransformation3d::getDirect( 
                     const double fromX, const double fromY, const double fromZ,
                     double& toX, double& toY, double& toZ )
{

  if ( std::fabs( m_direct[ 0 ][ 2 ] ) > 0.0 ) // sagittal
  {

    toX = m_direct[ 0 ][ 2 ] * fromZ + m_direct[ 0 ][ 3 ];
    toY = m_direct[ 1 ][ 0 ] * fromX;
    toZ = m_direct[ 2 ][ 1 ] * fromY;

  }
  else if ( std::fabs( m_direct[ 1 ][ 2 ] ) > 0.0 ) // coronal
  {

    toX = m_direct[ 0 ][ 0 ] * fromX;
    toY = m_direct[ 1 ][ 2 ] * fromZ + m_direct[ 1 ][ 3 ];
    toZ = m_direct[ 2 ][ 1 ] * fromY;

  }
  else // axial
  {

    toX = m_direct[ 0 ][ 0 ] * fromX;
    toY = m_direct[ 1 ][ 1 ] * fromY;
    toZ = m_direct[ 2 ][ 2 ] * fromZ + m_direct[ 2 ][ 3 ];

  }

}


void soma::AxialTransformation3d::getInverse( 
                           const double toX, const double toY, const double toZ,
                           double& fromX, double& fromY, double& fromZ )
{

  if ( std::fabs( m_direct[ 0 ][ 2 ] ) > 0.0 ) // sagittal
  {

    fromX = m_direct[ 1 ][ 0 ] * toY;
    fromY = m_direct[ 2 ][ 1 ] * toZ;
    fromZ = m_direct[ 0 ][ 2 ] * ( toX - m_direct[ 0 ][ 3 ] );

  }
  else if ( std::fabs( m_direct[ 1 ][ 2 ] ) > 0.0 ) // coronal
  {

    fromX = m_direct[ 0 ][ 0 ] * toX;
    fromY = m_direct[ 2 ][ 1 ] * toZ;
    fromZ = m_direct[ 1 ][ 2 ] * ( toY - m_direct[ 1 ][ 3 ] );

  }
  else // axial
  {

    fromX = m_direct[ 0 ][ 0 ] * toX;
    fromY = m_direct[ 1 ][ 1 ] * toY;
    fromZ = m_direct[ 2 ][ 2 ] * ( toZ - m_direct[ 2 ][ 3 ] );

  }

}

