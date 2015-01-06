#include <soma-io/Dicom/MultiSliceReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Object/HeaderProxy.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <limits>


soma::MultiSliceReader::MultiSliceReader()
                      : soma::DicomReader()
{
}


void soma::MultiSliceReader::setOrientation()
{

  soma::Vector normalVec = m_dataInfo->m_rowVec.cross( m_dataInfo->m_colVec );

  m_origin = soma::Vector( 0.0, 0.0, 0.0 );
  m_dataInfo->m_resolution.z = m_dataInfo->m_spacingBetweenSlices;

  if ( !m_positions.empty() )
  {

    if ( m_positions.size() > 1 )
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
    double n = (double)m_dataInfo->m_slices - 1.0;
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

    if ( ( n >= 1.0 ) && ( maxPos > minPos ) )
    {

      m_dataInfo->m_resolution.z = ( maxPos - minPos ) / n;

    }

  }

  m_dataInfo->m_patientOrientation.set( m_dataInfo->m_rowVec,
                                        m_dataInfo->m_colVec,
                                        normalVec,
                                        m_origin,
                                        m_dataInfo->m_width,
                                        m_dataInfo->m_height,
                                        m_dataInfo->m_slices,
                                        m_dataInfo->m_resolution );

  if ( !m_dataInfo->m_noFlip )
  {

    m_dataInfo->m_patientOrientation.getSize( m_dataInfo->m_width,
                                              m_dataInfo->m_height,
                                              m_dataInfo->m_slices );
    m_dataInfo->m_patientOrientation.getResolution( 
                                                   m_dataInfo->m_resolution.x,
                                                   m_dataInfo->m_resolution.y,
                                                   m_dataInfo->m_resolution.z );

  }

}


bool soma::MultiSliceReader::getHeader( 
                                       soma::HeaderProxy& header, 
                                       soma::DataInfo& dataInfo,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  OFString tmpString;
  DcmDataset dataset;
  std::vector< std::string > referential;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFString( DCM_FrameOfReferenceUID, tmpString ).good() )
  {

    referential.push_back( tmpString.c_str() );
    header.addAttribute( "referential", referential );

  }

  std::vector< std::string > referentials;
  referentials.push_back(
                        std::string( "Scanner-based anatomical coordinates" ) );
  header.addAttribute( "referentials", referentials );

  std::vector< double > transformation = 
         dataInfo.m_patientOrientation.getReferential().getDirectCoefficients();
  transformation[ 3 ] *= -1.0;
  transformation[ 7 ] *= -1.0;
  transformation[ 11 ] *= -1.0;
  std::vector< std::vector< double > > transformations;
  transformations.push_back( transformation );
  header.addAttribute( "transformations", transformations );

  std::vector< double > axialTransformation = 
    dataInfo.m_patientOrientation.
                               getAxialTransformation().getDirectCoefficients();
  header.addAttribute( "storage_to_memory", axialTransformation );

  return soma::DicomReader::getHeader( header, dataInfo, datasetHeader );

}
