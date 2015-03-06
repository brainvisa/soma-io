#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/Siemens/MosaicDicomImage.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Transformation/PatientOrientation.h>
#else
#include <Dicom/Siemens/Demosaicer.h>
#include <Dicom/Siemens/MosaicDicomImage.h>
#include <Container/DataInfo.h>
#include <Transformation/PatientOrientation.h>
#endif

#include <cstring>


soma::Demosaicer::Demosaicer( const soma::Vector& rowCosine,
                              const soma::Vector& columnCosine,
                              int32_t sizeX,
                              int32_t sizeY,
                              int32_t sizeZ,
                              int32_t sliceCount,
                              double resolutionX,
                              double resolutionY,
                              double resolutionZ )
                : _rowVector( rowCosine ),
                  _columnVector( columnCosine ),
                  _mosaicSizeX( sizeX ),
                  _mosaicSizeY( sizeY ),
                  _sizeX( sizeX ),
                  _sizeY( sizeY ),
                  _sizeZ( sizeZ ),
                  _divider( 1 ),
                  _resolutionX( resolutionX ),
                  _resolutionY( resolutionY ),
                  _resolutionZ( resolutionZ )
{

  if ( ( _sizeZ == 1 ) && ( sliceCount > 1 ) )
  {

    _sizeZ = sliceCount;

    while ( ( _divider * _divider ) < sliceCount )
    {

      _divider++;

    }

    _sizeX /= _divider;
    _sizeY /= _divider;

  }

}


soma::Demosaicer::~Demosaicer()
{
}


int32_t soma::Demosaicer::getMosaicSize()
{

  return _mosaicSizeX * _mosaicSizeY;

}


int32_t soma::Demosaicer::getMosaicSizeX()
{

  return _mosaicSizeX;

}


void soma::Demosaicer::getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ )
{

  sizeX = _sizeX;
  sizeY = _sizeY;
  sizeZ = _sizeZ;

}


void soma::Demosaicer::demosaic( soma::MosaicDicomImage& dicomImage,
                                 soma::DataInfo& info,
                                 int32_t slice,
                                 int32_t t )
{

  int32_t divider = _divider;
  int32_t startX = info._boundingBox.getLowerX();
  int32_t startY = info._boundingBox.getLowerY();
  int32_t baseOffset = startY * _mosaicSizeX + startX;

  if ( info._noDemosaic )
  {

    divider = 1;
    dicomImage.setLineShift( 0 );

  }

  if ( divider > 1 )
  {

    int32_t z;
    int32_t startZ = info._boundingBox.getLowerZ();
    int32_t endZ = info._boundingBox.getUpperZ() + 1;
    int32_t sliceSize = _mosaicSizeX * _sizeY;

    for ( z = startZ; z < endZ; z++ )
    {

      dicomImage.setOffset( ( z / _divider ) * sliceSize +
                            ( z % _divider ) * _sizeX + baseOffset );
      dicomImage.fill( z, t );

    }

  }
  else
  {

    dicomImage.setOffset( baseOffset );
    dicomImage.fill( slice, t );

  }

}


soma::Vector soma::Demosaicer::demosaic( const Vector& pos )
{

  Vector outPos;

  double diffX = 0.5 * ( _mosaicSizeX - _sizeX ) * _resolutionX;
  double diffY = 0.5 * ( _mosaicSizeY - _sizeY ) * _resolutionY;

  outPos.x = pos.x + _rowVector.x * diffX + _columnVector.x * diffY;
  outPos.y = pos.y + _rowVector.y * diffX + _columnVector.y * diffY;
  outPos.z = pos.z + _rowVector.z * diffX + _columnVector.z * diffY;

  return outPos;

}
