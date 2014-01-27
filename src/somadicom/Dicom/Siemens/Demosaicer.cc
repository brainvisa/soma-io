#include <Dicom/Siemens/Demosaicer.h>
#include <Container/Data.h>

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
                                 soma::Data& data,
                                 int32_t slice,
                                 int32_t t )
{

  int32_t x, y, px, py, pz;
  int32_t bpp = data.m_info.m_bpp;
  int32_t lineShift = ( m_mosaicSizeX - m_sizeX );

  const uint8_t* m = (const uint8_t*)dcmImage.getInterData()->getData();
  int32_t representation = dcmImage.getInterData()->getRepresentation();

  data.m_info.m_pixelRepresentation = representation & 1;

  if ( m_divider > 1 )
  {

    int32_t z;
    int32_t sliceSize = m_mosaicSizeX * m_sizeY;

    if ( ( bpp == 2 ) && ( representation < 2 ) )
    {

      for ( z = 0; z < m_sizeZ; z++ )
      {

        const uint8_t* slicePtr = m + ( z / m_divider ) * sliceSize +
                                      ( z % m_divider ) * m_sizeX;

        for ( y = 0; y < m_sizeY; y++, slicePtr += lineShift )
        {

          for ( x = 0; x < m_sizeX; x++ )
          {

            data.m_info.m_patientOrientation.getDirect( x, y, z, px, py, pz );

            uint16_t* v = (uint16_t*)data.getLine( py, pz, t ) + px;

            *v = (uint16_t)*slicePtr++;

          }

        }

      }

    }
    else
    {

      lineShift *= bpp;

      for ( z = 0; z < m_sizeZ; z++ )
      {

        const uint8_t* slicePtr = m + ( ( z / m_divider ) * sliceSize +
                                        ( z % m_divider ) * m_sizeX ) * bpp;

        for ( y = 0; y < m_sizeY; y++, slicePtr += lineShift )
        {

          for ( x = 0; x < m_sizeX; x++, slicePtr += bpp )
          {

            data.m_info.m_patientOrientation.getDirect( x, y, z, px, py, pz );

            uint8_t* v = data.getLine( py, pz, t ) + px * bpp;

            std::memcpy( v, slicePtr, bpp );

          }

        }

      }

    }

  }
  else
  {

    if ( ( bpp == 2 ) && ( representation < 2 ) )
    {

      for ( y = 0; y < m_sizeY; y++, m += lineShift )
      {

        for ( x = 0; x < m_sizeX; x++ )
        {

          data.m_info.m_patientOrientation.getDirect( x, y, slice, px, py, pz );

          uint16_t* v = (uint16_t*)data.getLine( py, pz, t ) + px;

          *v = (uint16_t)*m++;

        }

      }

    }
    else
    {

      lineShift *= bpp;

      for ( y = 0; y < m_sizeY; y++, m += lineShift )
      {

        for ( x = 0; x < m_sizeX; x++, m += bpp )
        {

          data.m_info.m_patientOrientation.getDirect( x, y, slice, px, py, pz );

          uint8_t* v = data.getLine( py, pz, t ) + px * bpp;

          std::memcpy( v, m, bpp );

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
