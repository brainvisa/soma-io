#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/FlipTransformation3d.h>
#else
#include <Transformation/FlipTransformation3d.h>
#endif


soma::FlipTransformation3d::FlipTransformation3d()
                          : soma::AxialTransformation3d()
{
}


soma::FlipTransformation3d::FlipTransformation3d( 
                                              const soma::Vector& rowCosine,
                                              const soma::Vector& columnCosine )
                          : soma::AxialTransformation3d()
{

  soma::Vector rowVec = rowCosine.mainDirection().absoluteCoordinates();
  soma::Vector columnVec = columnCosine.mainDirection().absoluteCoordinates();
  soma::Vector normalVec = rowVec.cross( columnVec ).absoluteCoordinates();

  initialize( normalVec );

  setItems( rowVec.x, columnVec.x, normalVec.x, 0.0,
            rowVec.y, columnVec.y, normalVec.y, 0.0,
            rowVec.z, columnVec.z, normalVec.z, 0.0,
            0.0,      0.0,         0.0,         1.0 );

}


soma::FlipTransformation3d::FlipTransformation3d( 
                                       const soma::FlipTransformation3d& other )
                          : soma::AxialTransformation3d( other )
{
}
