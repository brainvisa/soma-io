#include <soma-io/Dicom/DicomSortContext.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <sstream>


soma::DicomSortContext::DicomSortContext( 
                         const std::string& seriesInstanceUID,
                         const std::set< std::string >& files,
                         std::multimap< double, soma::FileInformation >& slices,
                         const soma::Vector& rowVector,
                         const soma::Vector& columnVector,
                         int32_t& selectedFileCount )
                      : m_seriesInstanceUID( seriesInstanceUID ),
                        m_slices( slices ),
                        m_selectedFileCount( selectedFileCount )
{

  m_files.clear();
  m_files.insert( m_files.end(), files.begin(), files.end() );
  m_absNormal = 
          rowVector.cross( columnVector ).mainDirection().absoluteCoordinates();

}


void soma::DicomSortContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  std::multimap< double, soma::FileInformation > slices;

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

          soma::FileInformation fileInfo;
          double index = 0.0;

          fileInfo.m_instanceNumber = int32_t( slices.size() );
          fileInfo.m_fileName = m_files[ i ];
          fileInfo.m_slope = 1.0;
          fileInfo.m_intercept = 0.0;

          Sint32 tmpInt;
          if ( dataset->findAndGetSint32( DCM_InstanceNumber, tmpInt ).good() )
          {

            fileInfo.m_instanceNumber = int32_t( tmpInt );

          }
          
          Float64 tmpFloat;
          if ( dataset->findAndGetFloat64( DCM_EchoTime, tmpFloat ).good() )
          {

            fileInfo.m_echoTime = double( tmpFloat );

          }
    
          if ( dataset->findAndGetFloat64( DCM_RescaleSlope, tmpFloat ).good() )
          {

            fileInfo.m_slope = (double)tmpFloat ;

          }

          if ( dataset->findAndGetFloat64( DCM_RescaleIntercept, 
                                           tmpFloat ).good() )
          {

             fileInfo.m_intercept = (double)tmpFloat;

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

            iss >> fileInfo.m_imagePosition.x
                >> fileInfo.m_imagePosition.y
                >> fileInfo.m_imagePosition.z;

            index = m_absNormal.dot( fileInfo.m_imagePosition );

          }

          slices.insert( std::make_pair( index, fileInfo ) );

        }

      }

    }

  }

  lock();
  m_slices.insert( slices.begin(), slices.end() );
  m_selectedFileCount += (int32_t)slices.size();
  unlock();

}
