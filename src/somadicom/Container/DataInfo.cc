#include <soma-io/Container/DataInfo.h>


soma::DataInfo::DataInfo()
              : m_depth( 8 ),
                m_bitsStored( 8 ),
                m_pixelRepresentation( 0 ),
                m_monochrome( true ),
                m_minimum( 0 ),
                m_maximum( 0 ),
                m_width( 1 ),
                m_height( 1 ),
                m_slices( 1 ),
                m_frames( 1 ),
                m_pixelSpacingX( 1.0 ),
                m_pixelSpacingY( 1.0 ),
                m_pixelSpacingZ( 1.0 ),
                m_spacingBetweenSlices( 1.0 ),
                m_repetitionTime( 1.0 ),
                m_modalityLUTApplied( true ),
                m_fileCount( 0 )
{

  m_rowVec = soma::Vector( 1.0, 0.0, 0.0 );
  m_colVec = soma::Vector( 0.0, 1.0, 0.0 );
  initialize();

}


soma::DataInfo::DataInfo( const soma::DataInfo& other )
              : m_depth( other.m_depth ),
                m_bitsStored( other.m_bitsStored ),
                m_pixelRepresentation( other.m_pixelRepresentation ),
                m_monochrome( other.m_monochrome ),
                m_minimum( other.m_minimum ),
                m_maximum( other.m_maximum ),
                m_width( other.m_width ),
                m_height( other.m_height ),
                m_slices( other.m_slices ),
                m_frames( other.m_frames ),
                m_pixelSpacingX( other.m_pixelSpacingX ),
                m_pixelSpacingY( other.m_pixelSpacingY ),
                m_pixelSpacingZ( other.m_pixelSpacingZ ),
                m_spacingBetweenSlices( other.m_spacingBetweenSlices ),
                m_repetitionTime( other.m_repetitionTime ),
                m_modalityLUTApplied( other.m_modalityLUTApplied ),
                m_fileCount( other.m_fileCount ),
                m_rowVec( other.m_rowVec ),
                m_colVec( other.m_colVec ),
                m_patientOrientation( other.m_patientOrientation )
{

  initialize();

}


soma::DataInfo::~DataInfo()
{
}


void soma::DataInfo::clear()
{

  m_depth = 8;
  m_bpp = 0;
  m_bitsStored = 8;
  m_pixelRepresentation = 0;
  m_monochrome = true;
  m_minimum = 0;
  m_maximum = 0;
  m_width = 1;
  m_height = 1;
  m_slices = 1;
  m_frames = 1;
  m_sliceSize = 0;
  m_volumeSize = 0;
  m_datasetSize = 0;
  m_pixelSpacingX = 1.0;
  m_pixelSpacingY = 1.0;
  m_pixelSpacingZ = 1.0;
  m_spacingBetweenSlices = 1.0;
  m_repetitionTime = 1.0;
  m_modalityLUTApplied = true;
  m_fileCount = 0;
  m_slope.clear();
  m_intercept.clear();
  m_datasetHeader.clear();
  m_rowVec = soma::Vector( 1.0, 0.0, 0.0 );
  m_colVec = soma::Vector( 0.0, 1.0, 0.0 );

}


void soma::DataInfo::initialize()
{

  m_bpp = m_depth / 8;

  if ( !m_monochrome && ( m_bpp < 3 ) )
  {

    m_bpp *= 3;

  }

  int32_t sliceCount = m_slices * m_frames;

  m_sliceSize = m_width * m_height;
  m_volumeSize = m_sliceSize * m_slices;
  m_datasetSize = m_volumeSize * m_frames;
  m_slope.resize( sliceCount, 1.0 );
  m_intercept.resize( sliceCount, 0.0 );
  m_datasetHeader.resize( m_fileCount, soma::DatasetHeader() );
  m_normVec = m_rowVec.cross( m_colVec );

}
