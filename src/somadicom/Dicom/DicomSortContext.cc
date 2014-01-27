#include <Dicom/DicomSortContext.h>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <sstream>


soma::DicomSortContext::DicomSortContext( 
                         const std::string& seriesInstanceUID,
                         const std::vector< std::string >& files,
                         std::multimap< double, soma::SortInformation >& slices,
                         int32_t& selectedFileCount )
                      : m_seriesInstanceUID( seriesInstanceUID ),
                        m_files( files ),
                        m_slices( slices ),
                        m_selectedFileCount( selectedFileCount )
{
}


void soma::DicomSortContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  std::multimap< double, soma::SortInformation > slices;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    DcmFileFormat fileFormat;
    OFCondition status = fileFormat.loadFile( m_files[ i ].c_str() );

    if ( status.good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      OFString tmpString;

      if ( dataset->findAndGetOFString( DCM_SeriesInstanceUID, 
                                        tmpString ).good() )
      {

        if ( !tmpString.compare( m_seriesInstanceUID.c_str() ) )
        {

          soma::SortInformation sortInfo;
          double index = 0.0;

          sortInfo.m_instanceNumber = int32_t( slices.size() );
          sortInfo.m_fileName = m_files[ i ];

          Float64 tmpFloat;
          if ( dataset->findAndGetFloat64( DCM_SliceLocation, 
                                           tmpFloat ).good() )
          {

            index = double( tmpFloat );

          }
          
          Sint32 tmpInt;
          if ( dataset->findAndGetSint32( DCM_InstanceNumber,
                                          tmpInt ).good() )
          {

            sortInfo.m_instanceNumber = int32_t( tmpInt );

          }
          
          if ( dataset->findAndGetFloat64( DCM_EchoTime,
                                           tmpFloat ).good() )
          {

            sortInfo.m_echoTime = double( tmpFloat );

          }
          
          if ( dataset->findAndGetOFStringArray( DCM_ImagePositionPatient,
                                                 tmpString ).good() )
          {

            std::string strPosition = tmpString.c_str();
            size_t pos = strPosition.find( "\\" );

            while ( pos != std::string::npos )
            {

              strPosition[ pos ] = ' ';
              pos = strPosition.find( "\\" );

            }

            std::istringstream iss( strPosition );

            iss >> sortInfo.m_imagePosition.x
                >> sortInfo.m_imagePosition.y
                >> sortInfo.m_imagePosition.z;

          }

          slices.insert( std::make_pair( index, sortInfo ) );

        }

      }

    }

  }

  lock();
  m_slices.insert( slices.begin(), slices.end() );
  m_selectedFileCount += (int32_t)slices.size();
  unlock();

}
