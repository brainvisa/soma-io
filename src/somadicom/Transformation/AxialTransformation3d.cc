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

  soma::Vector rowVec = rowCosine.mainDirection();
  soma::Vector columnVec = columnCosine.mainDirection();
  soma::Vector normalVec = normal.mainDirection();

  setItems(  rowVec.x,  columnVec.x,  normalVec.x, 0.0,
             rowVec.y,  columnVec.y,  normalVec.y, 0.0,
            -rowVec.z, -columnVec.z, -normalVec.z, 0.0,
            0.0,          0.0,             0.0,    1.0 );

}
