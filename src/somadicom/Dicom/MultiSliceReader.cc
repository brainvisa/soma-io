#include <soma-io/Dicom/MultiSliceReader.h>
#include <soma-io/Container/Data.h>

#include <limits>


soma::MultiSliceReader::MultiSliceReader()
                      : soma::DicomReader()
{
}


void soma::MultiSliceReader::setOrientation( soma::DataInfo& dataInfo )
{

  soma::Vector normalVec = dataInfo.m_rowVec.cross( dataInfo.m_colVec );

  m_origin = soma::Vector( 0.0, 0.0, 0.0 );
  dataInfo.m_pixelSpacingZ = dataInfo.m_spacingBetweenSlices;

  if ( !m_positions.empty() )
  {

    if ( m_dataInfo.m_slices > 1 )
    {

      soma::Vector normVec = m_positions[ 1 ] - m_positions[ 0 ];

      if ( normVec.getNorm() > 0.0 )
      {

        normVec.normalize();
        normalVec = normVec;

      }

    }

    double minPos = std::numeric_limits< double >::max();
    double maxPos = -std::numeric_limits< double >::max();
    double n = (double)m_positions.size();
    std::vector< soma::Vector >::iterator 
      p = m_positions.begin(),
      pe = m_positions.end();

    m_origin = *p;

    while ( p != pe )
    {

      double dotProduct = normalVec.dot( *p );

      if ( dotProduct < minPos )
      {

        minPos = dotProduct;
        m_origin = *p;

      }
      else if ( dotProduct > maxPos )
      {

        maxPos = dotProduct;

      }

      ++p;

    }

    if ( ( n > 1.0 ) && ( maxPos > minPos ) )
    {

      dataInfo.m_pixelSpacingZ = ( maxPos - minPos ) / n;

    }

  }

  dataInfo.m_patientOrientation.set( dataInfo.m_rowVec,
                                     dataInfo.m_colVec,
                                     normalVec,
                                     m_origin,
                                     dataInfo.m_width,
                                     dataInfo.m_height,
                                     dataInfo.m_slices,
                                     dataInfo.m_pixelSpacingX,
                                     dataInfo.m_pixelSpacingY,
                                     dataInfo.m_pixelSpacingZ );
  dataInfo.m_patientOrientation.getSize( dataInfo.m_width,
                                         dataInfo.m_height,
                                         dataInfo.m_slices );
  dataInfo.m_patientOrientation.getResolution( dataInfo.m_pixelSpacingX,
                                               dataInfo.m_pixelSpacingY,
                                               dataInfo.m_pixelSpacingZ );

}
