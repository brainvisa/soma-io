#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ImagePixel.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/ImagePixel.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


dcm::ImagePixel::Parameters::Parameters()
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


dcm::ImagePixel::Parameters::Parameters( dcm::DicomProxy& proxy )
{

  dcm::DataInfo& info = proxy.getDataInfo();
  dcm::Vector3d< int32_t > size = info._patientOrientation.getOnDiskSize();
  dcm::BoundingBox< int32_t > outBoundingBox;

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
  sizeX = size.x;
  sizeZ = size.z;
  sizeXY = size.x * size.y;
  outLowerX = outBoundingBox.getLowerX();
  outLowerY = outBoundingBox.getLowerY();
  outLowerZ = outBoundingBox.getLowerZ();
  outLowerT = outBoundingBox.getLowerT();
  transform = &info._patientOrientation;

}


dcm::ImagePixel::Parameters::Parameters( 
                                      const dcm::ImagePixel::Parameters& other )
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


dcm::ImagePixel::ImagePixel( dcm::DicomProxy& proxy )
               : _proxy( proxy ),
                 _slope( 1.0f ),
                 _intercept( 0.0f )
{
}


dcm::ImagePixel::~ImagePixel()
{
}


bool dcm::ImagePixel::initialize( DcmDataset* /* dataset */ )
{

  return true;

}


void dcm::ImagePixel::setSlopeAndIntercept( float slope, float intercept )
{

  _slope = slope;
  _intercept = intercept;

}


float dcm::ImagePixel::applyModalityLut( float value )
{

  return value * _slope + _intercept;

}
