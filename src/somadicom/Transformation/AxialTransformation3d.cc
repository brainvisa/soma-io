#include <soma-io/Transformation/AxialTransformation3d.h>

#include <cmath>


soma::AxialTransformation3d::AxialTransformation3d()
                           : soma::Transformation3d()
{
}


soma::AxialTransformation3d::AxialTransformation3d( 
                                               const soma::Vector& rowCosine,
                                               const soma::Vector& columnCosine,
                                               const soma::Vector& normal )
                           : soma::Transformation3d()
{

  soma::Vector rowVec = changeToMainDirection( rowCosine );
  soma::Vector columnVec = changeToMainDirection( columnCosine );
  soma::Vector normalVec = changeToMainDirection( normal );

  setItems(  rowVec.x,  columnVec.x,  normalVec.x, 0.0,
             rowVec.y,  columnVec.y,  normalVec.y, 0.0,
            -rowVec.z, -columnVec.z, -normalVec.z, 0.0,
            0.0,          0.0,             0.0,    1.0 );

}


soma::Vector soma::AxialTransformation3d::changeToMainDirection( 
                                                 const soma::Vector& vec ) const
{

  soma::Vector mainVector( 0.0, 0.0, 0.0 );
  double absX = std::fabs( vec.x );
  double absY = std::fabs( vec.y );
  double absZ = std::fabs( vec.z );

  if ( absX > absY )
  {

    if ( absX > absZ )
    {
  
      mainVector.x = vec.x / absX;
  
    }
    else
    {
  
      mainVector.z = vec.z / absZ;
  
    }

  }
  else
  {

    if ( absY > absZ )
    {
  
      mainVector.y = vec.y / absY;
  
    }
    else
    {
   
      mainVector.z = vec.z / absZ;
  
    }

  }

  return mainVector;

}
