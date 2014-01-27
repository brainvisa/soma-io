#include <Transformation/DicomTransformation3d.h>
#include <Container/Vector.h>


soma::DicomTransformation3d::DicomTransformation3d()
                           : soma::Transformation3d()
{
}


soma::DicomTransformation3d::DicomTransformation3d( 
                                               const soma::Vector& rowCosine,
                                               const soma::Vector& columnCosine,
                                               const soma::Vector& normal,
                                               const soma::Vector& origin )
                           : soma::Transformation3d()
{

  setItems(  rowCosine.x,  columnCosine.x,  normal.x,  origin.x,
             rowCosine.y,  columnCosine.y,  normal.y,  origin.y,
            -rowCosine.z, -columnCosine.z, -normal.z, -origin.z,
            0.0,          0.0,             0.0,       1.0 );

}


void soma::DicomTransformation3d::setTranslation( 
                                               const soma::Vector& translation )
{

  m_direct[ 0 ][ 3 ] = translation.x;
  m_direct[ 1 ][ 3 ] = translation.y;
  m_direct[ 2 ][ 3 ] = -translation.z;

  invert( m_direct, m_inverse );

}
