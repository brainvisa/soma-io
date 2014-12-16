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
                m_spacingBetweenSlices( 1.0 ),
                m_repetitionTime( 1.0 ),
                m_modalityLUTApplied( true ),
                m_fileCount( 0 ),
                m_mosaic( false ),
                m_noFlip( false ),
                m_noDemosaic( false )
{

  m_resolution = soma::Vector( 1.0, 1.0, 1.0 );
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
                m_resolution( other.m_resolution ),
                m_spacingBetweenSlices( other.m_spacingBetweenSlices ),
                m_repetitionTime( other.m_repetitionTime ),
                m_modalityLUTApplied( other.m_modalityLUTApplied ),
                m_fileCount( other.m_fileCount ),
                m_rowVec( other.m_rowVec ),
                m_colVec( other.m_colVec ),
                m_patientOrientation( other.m_patientOrientation ),
                m_datasetHeader( other.m_datasetHeader ),
                m_mosaic( other.m_mosaic ),
                m_noFlip( other.m_noFlip ),
                m_noDemosaic( other.m_noDemosaic )
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
  m_resolution = soma::Vector( 1.0, 1.0, 1.0 );
  m_spacingBetweenSlices = 1.0;
  m_repetitionTime = 1.0;
  m_modalityLUTApplied = true;
  m_fileCount = 0;
  m_slope.clear();
  m_intercept.clear();
  m_rowVec = soma::Vector( 1.0, 0.0, 0.0 );
  m_colVec = soma::Vector( 0.0, 1.0, 0.0 );
  m_datasetHeader.clear();
  m_mosaic = false;
  m_noFlip = false;
  m_noDemosaic = false;

}


void soma::DataInfo::initialize()
{

  m_bpp = m_depth / 8;

  if ( !m_monochrome && ( m_bpp < 3 ) )
  {

    m_bpp *= 3;

  }

  if ( !m_maximum )
  {

    m_maximum = ( 1 << m_depth ) - 1;

  }

  m_sliceSize = m_width * m_height;
  m_volumeSize = m_sliceSize * m_slices;
  m_datasetSize = m_volumeSize * m_frames;
  m_normVec = m_rowVec.cross( m_colVec );

}
