#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomSelectContext.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/System/DirectoryParser.h>
#else
#include <Dicom/DicomSelectContext.h>
#include <Dicom/DicomDatasetHeader.h>
#include <System/DirectoryParser.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::DicomSelectContext::DicomSelectContext( 
                                       soma::DirectoryParser& directory,
                                       const std::string& seriesInstanceUID,
                                       soma::DicomDatasetHeader& datasetHeader,
                                       int32_t& fileCount )
                        : _seriesInstanceUID( seriesInstanceUID ),
                          _datasetHeader( datasetHeader ),
                          _fileCount( fileCount )
{

  _files.insert( _files.end(), 
                 directory.getFiles().begin(),
                 directory.getFiles().end() );

}


void soma::DicomSelectContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  int32_t nFiles = 0;

  for ( i = startIndex; i < stopIndex; i++ )
  {

    DcmFileFormat header;
    std::string fileName = _files[ i ];

    if ( header.loadFile( fileName.c_str() ).good() )
    {

      DcmDataset* dataset = header.getDataset();

      if ( dataset )
      {

        OFString tmpString;
        if ( dataset->findAndGetOFString( DCM_SeriesInstanceUID, 
                                          tmpString ).good() )
        {

          if ( !tmpString.compare( _seriesInstanceUID.c_str() ) )
          {

            if ( _datasetHeader.add( i, header.getDataset(), fileName ) )
            {

              nFiles++;

            }

          }

        }

      }

    }

  }

  lock();
  _fileCount += nFiles;
  unlock();

}
