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
                                               const soma::Vector& origin )
                           : soma::Transformation3d()
{

  soma::Vector rowVec = rowCosine;
  soma::Vector columnVec = columnCosine;
  soma::Vector normalVec = rowVec.cross( columnVec );

  setItems( rowVec.x, columnVec.x, normalVec.x,  origin.x,
            rowVec.y, columnVec.y, normalVec.y,  origin.y,
            rowVec.z, columnVec.z, normalVec.z,  origin.z,
            0.0,      0.0,         0.0,          1.0 );

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
  _direct[ 2 ][ 3 ] = translation.z;

  invert( _direct, _inverse );

}
