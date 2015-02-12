#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Transformation/PatientOrientation.h>
#else
#include <Dicom/Siemens/Demosaicer.h>
#include <Container/DicomProxy.h>
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


void soma::Demosaicer::getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ )
{

  sizeX = _sizeX;
  sizeY = _sizeY;
  sizeZ = _sizeZ;

}


void soma::Demosaicer::demosaic( DicomImage& dcmImage,
                                 soma::DicomProxy& data,
                                 int32_t slice,
                                 int32_t t )
{

  const uint8_t* m = (const uint8_t*)dcmImage.getInterData()->getData();

  data.getDataInfo()._pixelRepresentation = 
                                   dcmImage.getInterData()->getRepresentation();

  demosaic( m, data, slice, t );

}


void soma::Demosaicer::demosaic( const uint8_t* mosaic,
                                 soma::DicomProxy& data,
                                 int32_t slice,
                                 int32_t t )
{

  int32_t x, y, px, py, pz;
  soma::DataInfo& info = data.getDataInfo();
  int32_t startX = info._boundingBox.getLowerX();
  int32_t startY = info._boundingBox.getLowerY();
  int32_t endX = info._boundingBox.getUpperX() + 1;
  int32_t endY = info._boundingBox.getUpperY() + 1;
  int32_t lineShift = _mosaicSizeX + startX - endX;
  int32_t divider = _divider;
  soma::BoundingBox< int32_t > outBoundingBox;

  if ( info._noDemosaic )
  {

    divider = 1;
    lineShift = 0;

  }

  if ( info._noFlip )
  {

    outBoundingBox = info._boundingBox;

  }
  else
  {

    outBoundingBox =
           info._patientOrientation.getDirectBoundingBox( info._boundingBox );

  }

  int32_t bbLowerX = outBoundingBox.getLowerX();
  int32_t bbLowerY = outBoundingBox.getLowerY();
  int32_t bbLowerZ = outBoundingBox.getLowerZ();
  int32_t bbLowerT = outBoundingBox.getLowerT();

  if ( divider > 1 )
  {

    int32_t z;
    int32_t startZ = info._boundingBox.getLowerZ();
    int32_t endZ = info._boundingBox.getUpperZ() + 1;
    int32_t sliceSize = _mosaicSizeX * _sizeY;

    if ( ( info._bpp == 2 ) && ( info._pixelRepresentation < 2 ) )
    {

      if ( info._noFlip )
      {

        for ( z = startZ; z < endZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( z / _divider ) * sliceSize +
                                             ( z % _divider ) * _sizeX + 
                                             startY * _mosaicSizeX + startX;

          for ( y = startY; y < endY; y++, slicePtr += lineShift )
          {

            for ( x = startX; x < endX; x++ )
            {

              *( (uint16_t*)data( x - bbLowerX, 
                                  y - bbLowerY, 
                                  z - bbLowerZ, 
                                  t - bbLowerT ) ) = (uint16_t)*slicePtr++;

            }

          }

        }

      }
      else
      {

        for ( z = startZ; z < endZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( z / _divider ) * sliceSize +
                                             ( z % _divider ) * _sizeX + 
                                             startY * _mosaicSizeX + startX;

          for ( y = startY; y < endY; y++, slicePtr += lineShift )
          {

            for ( x = startX; x < endX; x++ )
            {

              info._patientOrientation.getDirect( x, y, z, px, py, pz );
              *( (uint16_t*)data( px - bbLowerX, 
                                  py - bbLowerY, 
                                  pz - bbLowerZ, 
                                  t - bbLowerT ) ) = (uint16_t)*slicePtr++;

            }

          }

        }

      }

    }
    else
    {

      lineShift *= info._bpp;

      if ( info._noFlip )
      {

        for ( z = startZ; z < endZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( ( z / _divider ) * sliceSize +
                                               ( z % _divider ) * _sizeX + 
                                               startY * _mosaicSizeX + 
                                               startX ) * info._bpp;

          for ( y = startY; y < endY; y++, slicePtr += lineShift )
          {

            for ( x = startX; x < endX; x++, slicePtr += info._bpp )
            {

              std::memcpy( data( x - bbLowerX, 
                                 y - bbLowerY, 
                                 z - bbLowerZ, 
                                 t - bbLowerT ), 
                           slicePtr, 
                           info._bpp );

            }

          }

        }

      }
      else
      {

        for ( z = startZ; z < endZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( ( z / _divider ) * sliceSize +
                                               ( z % _divider ) * _sizeX + 
                                               startY * _mosaicSizeX + 
                                               startX ) * info._bpp;

          for ( y = startY; y < endY; y++, slicePtr += lineShift )
          {

            for ( x = startX; x < endX; x++, slicePtr += info._bpp )
            {

              info._patientOrientation.getDirect( x, y, z, px, py, pz );
              std::memcpy( data( px - bbLowerX, 
                                 py - bbLowerY, 
                                 pz - bbLowerZ, 
                                 t - bbLowerT ), 
                           slicePtr, 
                           info._bpp );

            }

          }

        }

      }

    }

  }
  else
  {

    if ( ( info._bpp == 2 ) && ( info._pixelRepresentation < 2 ) )
    {

      const uint8_t* slicePtr = mosaic + startY * _mosaicSizeX + startX;

      if ( info._noFlip )
      {

        for ( y = startY; y < endY; y++, slicePtr += lineShift )
        {

          for ( x = startX; x < endX; x++ )
          {

            *( (uint16_t*)data( x - bbLowerX, 
                                y - bbLowerY, 
                                slice - bbLowerZ, 
                                t - bbLowerT ) ) = (uint16_t)*slicePtr++;

          }

        }

      }
      else
      {

        for ( y = startY; y < endY; y++, slicePtr += lineShift )
        {

          for ( x = startX; x < endX; x++ )
          {

            info._patientOrientation.getDirect( x, y, slice, px, py, pz );
            *( (uint16_t*)data( px - bbLowerX, 
                                py - bbLowerY, 
                                pz - bbLowerZ, 
                                t - bbLowerT ) ) = (uint16_t)*slicePtr++;

          }

        }

      }

    }
    else
    {

      const uint8_t* slicePtr = mosaic + ( startY * _mosaicSizeX + startX ) * 
                                         info._bpp;

      lineShift *= info._bpp;

      if ( info._noFlip )
      {

        for ( y = startY; y < endY; y++, slicePtr += lineShift )
        {

          for ( x = startX; x < endX; x++, slicePtr += info._bpp )
          {

            std::memcpy( data( x - bbLowerX, 
                               y - bbLowerY, 
                               slice - bbLowerZ, 
                               t - bbLowerT ), 
                         slicePtr, 
                         info._bpp );

          }

        }

      }
      else
      {

        for ( y = startY; y < endY; y++, slicePtr += lineShift )
        {

          for ( x = startX; x < endX; x++, slicePtr += info._bpp )
          {

            info._patientOrientation.getDirect( x, y, slice, px, py, pz );
            std::memcpy( data( px - bbLowerX, 
                               py - bbLowerY, 
                               pz - bbLowerZ, 
                               t - bbLowerT ), 
                         slicePtr, 
                         info._bpp );

          }

        }

      }

    }

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
