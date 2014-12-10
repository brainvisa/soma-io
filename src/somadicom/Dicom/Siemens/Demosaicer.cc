#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Transformation/PatientOrientation.h>

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
                : m_rowVector( rowCosine ),
                  m_columnVector( columnCosine ),
                  m_mosaicSizeX( sizeX ),
                  m_mosaicSizeY( sizeY ),
                  m_sizeX( sizeX ),
                  m_sizeY( sizeY ),
                  m_sizeZ( sizeZ ),
                  m_divider( 1 ),
                  m_resolutionX( resolutionX ),
                  m_resolutionY( resolutionY ),
                  m_resolutionZ( resolutionZ )
{

  if ( ( m_sizeZ == 1 ) && ( sliceCount > 1 ) )
  {

    m_sizeZ = sliceCount;

    while ( ( m_divider * m_divider ) < sliceCount )
    {

      m_divider++;

    }

    m_sizeX /= m_divider;
    m_sizeY /= m_divider;

  }

}


soma::Demosaicer::~Demosaicer()
{
}


int32_t soma::Demosaicer::getMosaicSize()
{

  return m_mosaicSizeX * m_mosaicSizeY;

}


void soma::Demosaicer::getSize( int32_t& sizeX, int32_t& sizeY, int32_t& sizeZ )
{

  sizeX = m_sizeX;
  sizeY = m_sizeY;
  sizeZ = m_sizeZ;

}


void soma::Demosaicer::demosaic( DicomImage& dcmImage,
                                 soma::DicomProxy& data,
                                 int32_t slice,
                                 int32_t t )
{

  const uint8_t* m = (const uint8_t*)dcmImage.getInterData()->getData();

  data.getDataInfo().m_pixelRepresentation = 
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
  int32_t lineShift = ( m_mosaicSizeX - m_sizeX );
  int32_t divider = m_divider;

  if ( info.m_noDemosaic )
  {

    divider = 1;
    lineShift = 0;

  }

  if ( divider > 1 )
  {

    int32_t z;
    int32_t sliceSize = m_mosaicSizeX * m_sizeY;

    if ( ( info.m_bpp == 2 ) && ( info.m_pixelRepresentation < 2 ) )
    {

      if ( info.m_noFlip )
      {

        for ( z = 0; z < m_sizeZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( z / m_divider ) * sliceSize +
                                             ( z % m_divider ) * m_sizeX;

          for ( y = 0; y < m_sizeY; y++, slicePtr += lineShift )
          {

            for ( x = 0; x < m_sizeX; x++ )
            {

              *( (uint16_t*)data( x, y, z, t ) ) = (uint16_t)*slicePtr++;

            }

          }

        }

      }
      else
      {

        for ( z = 0; z < m_sizeZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( z / m_divider ) * sliceSize +
                                             ( z % m_divider ) * m_sizeX;

          for ( y = 0; y < m_sizeY; y++, slicePtr += lineShift )
          {

            for ( x = 0; x < m_sizeX; x++ )
            {

              info.m_patientOrientation.getDirect( x, y, z, px, py, pz );
              *( (uint16_t*)data( px, py, pz, t ) ) = (uint16_t)*slicePtr++;

            }

          }

        }

      }

    }
    else
    {

      lineShift *= info.m_bpp;

      if ( info.m_noFlip )
      {

        for ( z = 0; z < m_sizeZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( ( z / m_divider ) * sliceSize +
                                             ( z % m_divider ) * m_sizeX ) * 
                                             info.m_bpp;

          for ( y = 0; y < m_sizeY; y++, slicePtr += lineShift )
          {

            for ( x = 0; x < m_sizeX; x++, slicePtr += info.m_bpp )
            {

              std::memcpy( data( x, y, z, t ), slicePtr, info.m_bpp );

            }

          }

        }

      }
      else
      {

        for ( z = 0; z < m_sizeZ; z++ )
        {

          const uint8_t* slicePtr = mosaic + ( ( z / m_divider ) * sliceSize +
                                             ( z % m_divider ) * m_sizeX ) * 
                                             info.m_bpp;

          for ( y = 0; y < m_sizeY; y++, slicePtr += lineShift )
          {

            for ( x = 0; x < m_sizeX; x++, slicePtr += info.m_bpp )
            {

              info.m_patientOrientation.getDirect( x, y, z, px, py, pz );
              std::memcpy( data( px, py, pz, t ), slicePtr, info.m_bpp );

            }

          }

        }

      }

    }

  }
  else
  {

    if ( ( info.m_bpp == 2 ) && ( info.m_pixelRepresentation < 2 ) )
    {

      if ( info.m_noFlip )
      {

        for ( y = 0; y < m_sizeY; y++, mosaic += lineShift )
        {

          for ( x = 0; x < m_sizeX; x++ )
          {

            *( (uint16_t*)data( x, y, slice, t ) ) = (uint16_t)*mosaic++;

          }

        }

      }
      else
      {

        for ( y = 0; y < m_sizeY; y++, mosaic += lineShift )
        {

          for ( x = 0; x < m_sizeX; x++ )
          {

            info.m_patientOrientation.getDirect( x, y, slice, px, py, pz );
            *( (uint16_t*)data( px, py, pz, t ) ) = (uint16_t)*mosaic++;

          }

        }

      }

    }
    else
    {

      lineShift *= info.m_bpp;

      if ( info.m_noFlip )
      {

        for ( y = 0; y < m_sizeY; y++, mosaic += lineShift )
        {

          for ( x = 0; x < m_sizeX; x++, mosaic += info.m_bpp )
          {

            std::memcpy( data( x, y, slice, t ), mosaic, info.m_bpp );

          }

        }

      }
      else
      {

        for ( y = 0; y < m_sizeY; y++, mosaic += lineShift )
        {

          for ( x = 0; x < m_sizeX; x++, mosaic += info.m_bpp )
          {

            info.m_patientOrientation.getDirect( x, y, slice, px, py, pz );
            std::memcpy( data( px, py, pz, t ), mosaic, info.m_bpp );

          }

        }

      }

    }

  }

}


soma::Vector soma::Demosaicer::demosaic( const Vector& pos )
{

  Vector outPos;

  double diffX = 0.5 * ( m_mosaicSizeX - m_sizeX ) * m_resolutionX;
  double diffY = 0.5 * ( m_mosaicSizeY - m_sizeY ) * m_resolutionY;

  outPos.x = pos.x + m_rowVector.x * diffX + m_columnVector.x * diffY;
  outPos.y = pos.y + m_rowVector.y * diffX + m_columnVector.y * diffY;
  outPos.z = pos.z + m_rowVector.z * diffX + m_columnVector.z * diffY;

  return outPos;

}
