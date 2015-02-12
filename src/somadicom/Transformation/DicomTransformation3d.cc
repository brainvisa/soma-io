#ifdef SOMA_IO_DICOM
#include <soma-io/Transformation/DicomTransformation3d.h>
#else
#include <Transformation/DicomTransformation3d.h>
#endif


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


soma::DicomTransformation3d::DicomTransformation3d( 
                                      const soma::DicomTransformation3d& other )
                           : soma::Transformation3d( other )
{
}


void soma::DicomTransformation3d::setTranslation( 
                                               const soma::Vector& translation )
{

  _direct[ 0 ][ 3 ] = translation.x;
  _direct[ 1 ][ 3 ] = translation.y;
  _direct[ 2 ][ 3 ] = -translation.z;

  invert( _direct, _inverse );

}
