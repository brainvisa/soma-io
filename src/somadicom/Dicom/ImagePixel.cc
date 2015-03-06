#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImagePixel.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/ImagePixel.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


soma::ImagePixel::Parameters::Parameters()
                            : noFlip( false ),
                              startX( 0 ),
                              endX( 0 ),
                              startY( 0 ),
                              endY( 0 ),
                              sizeX( 0 ),
                              sizeZ( 0 ),
                              sizeXY( 0 ),
                              outLowerX( 0 ),
                              outLowerY( 0 ),
                              outLowerZ( 0 ),
                              outLowerT( 0 ),
                              transform( 0 )
{
}


soma::ImagePixel::Parameters::Parameters( soma::DicomProxy& proxy )
{

  soma::DataInfo& info = proxy.getDataInfo();
  soma::BoundingBox< int32_t > outBoundingBox;
  int32_t tmpX, tmpY, tmpZ;

  info._patientOrientation.getOnDiskSize( tmpX, tmpY, tmpZ );

  if ( info._noFlip )
  {

    outBoundingBox = info._boundingBox;

  }
  else
  {

    outBoundingBox =
             info._patientOrientation.getDirectBoundingBox( info._boundingBox );

  }

  noFlip = info._noFlip;
  startX = info._boundingBox.getLowerX();
  endX = info._boundingBox.getUpperX() + 1;
  startY = info._boundingBox.getLowerY();
  endY = info._boundingBox.getUpperY() + 1;
  sizeX = tmpX;
  sizeZ = tmpZ;
  sizeXY = tmpX * tmpY;
  outLowerX = outBoundingBox.getLowerX();
  outLowerY = outBoundingBox.getLowerY();
  outLowerZ = outBoundingBox.getLowerZ();
  outLowerT = outBoundingBox.getLowerT();
  transform = &info._patientOrientation;

}


soma::ImagePixel::Parameters::Parameters( 
                                     const soma::ImagePixel::Parameters& other )
                            : noFlip( other.noFlip ),
                              startX( other.startX ),
                              endX( other.endX ),
                              startY( other.startY ),
                              endY( other.endY ),
                              sizeX( other.sizeX ),
                              sizeZ( other.sizeZ ),
                              sizeXY( other.sizeXY ),
                              outLowerX( other.outLowerX ),
                              outLowerY( other.outLowerY ),
                              outLowerZ( other.outLowerZ ),
                              outLowerT( other.outLowerT ),
                              transform( other.transform )
{
}


soma::ImagePixel::ImagePixel( soma::DicomProxy& proxy )
                : _proxy( proxy )
{
}


soma::ImagePixel::~ImagePixel()
{
}


bool soma::ImagePixel::initialize( DcmDataset* /* dataset */ )
{

  return true;

}
