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


dcm::Demosaicer::Demosaicer( const dcm::Vector3d< double >& rowCosine,
                             const dcm::Vector3d< double >& columnCosine,
                             int32_t sizeX,
                             int32_t sizeY,
                             int32_t sizeZ,
                             int32_t sliceCount,
                             double resolutionX,
                             double resolutionY )
               : _rowVector( rowCosine ),
                 _columnVector( columnCosine ),
                 _mosaicSizeX( sizeX ),
                 _mosaicSizeY( sizeY ),
                 _sizeX( sizeX ),
                 _sizeY( sizeY ),
                 _sizeZ( sizeZ ),
                 _divider( 1 ),
                 _resolutionX( resolutionX ),
                 _resolutionY( resolutionY )
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


dcm::Demosaicer::~Demosaicer()
{
}


int32_t dcm::Demosaicer::getMosaicSize()
{

  return _mosaicSizeX * _mosaicSizeY;

}


int32_t dcm::Demosaicer::getMosaicSizeX()
{

  return _mosaicSizeX;

}


void dcm::Demosaicer::getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ )
{

  sizeX = _sizeX;
  sizeY = _sizeY;
  sizeZ = _sizeZ;

}


void dcm::Demosaicer::demosaic( dcm::MosaicDicomImage& dicomImage,
                                dcm::DataInfo& info,
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


dcm::Vector3d< double > dcm::Demosaicer::demosaic( 
                                            const dcm::Vector3d< double >& pos )
{

  double diffX = 0.5 * ( _mosaicSizeX - _sizeX ) * _resolutionX;
  double diffY = 0.5 * ( _mosaicSizeY - _sizeY ) * _resolutionY;

  return pos + _rowVector * diffX + _columnVector * diffY;

}
