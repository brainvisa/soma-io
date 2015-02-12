#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USImageStorageReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/USImageStorageReader.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::USImageStorageReader::USImageStorageReader()
                          : soma::USReader()
{
}


std::string soma::USImageStorageReader::getStorageUID()
{

  return UID_UltrasoundImageStorage;

}


bool soma::USImageStorageReader::readData( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    std::string fileName = datasetHeader.getFileList().front();
    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( fileName.c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer() );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        soma::DataInfo& info = proxy.getDataInfo();
        int32_t bpp = info._bpp;
        int32_t sizeX = info._width;
        int32_t startX = info._boundingBox.getLowerX();
        int32_t startY = info._boundingBox.getLowerY();
        int32_t endX = info._boundingBox.getUpperX() + 1;
        int32_t endY = info._boundingBox.getUpperY() + 1;
        int32_t offset = startY * sizeX + startX;
        int32_t lineShift = sizeX + startX - endX;
        int32_t representation = dcmImage.getInterData()->getRepresentation();

        if ( bpp == 3 )
        {

          uint8_t** d = (uint8_t**)dcmImage.getInterData()->getData();
          uint8_t* r = d[ 0 ] + offset;
          uint8_t* g = d[ 1 ] + offset;
          uint8_t* b = d[ 2 ] + offset;
          uint8_t* p;
          int32_t x, y;

          for ( y = startY; y < endY; y++ )
          {

            for ( x = startX; x < endX; x++ )
            {

              p = proxy( x - startX, y - startY );
              *p++ = *r++;
              *p++ = *g++;
              *p++ = *b++;

            }

            r += lineShift;
            g += lineShift;
            b += lineShift;

          }

        }
        else if ( ( bpp == 2 ) && ( representation < 2 ) )
        {

          uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData() + offset;
          int32_t x, y;

          for ( y = startY; y < endY; y++, pIn += lineShift )
          {

            for ( x = startX; x < endX; x++ )
            {

              *( (int16_t*)proxy( x - startX, y - startY ) ) = 
                                                              int16_t( *pIn++ );

            }

          }

        }
        else
        {

          uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData() + 
                         offset * bpp;

          lineShift *= bpp;

          if ( proxy.isMemoryMapped() )
          {

            int32_t x, y;

            for ( y = startY; y < endY; y++, pIn += lineShift )
            {

              for ( x = startX; x < endX; x++, pIn += bpp )
              {

                std::memcpy( proxy( x - startX, y - startY ), pIn, bpp );

              }

            }

          }
          else
          {

            int32_t length = bpp * ( endX - startX );
            int32_t lineShift2 = sizeX * bpp;
            int32_t y;

            for ( y = startY; y < endY; y++, pIn += lineShift2 )
            {

              std::memcpy( proxy( 0, y - startY ), pIn, length );

            }

          }

        }

        info._pixelRepresentation = representation;

        if ( bpp < 3 )
        {

          double min = 0.0, max = 0.0;

          dcmImage.getMinMaxValues( min, max );
          info._minimum = int32_t( min );
          info._maximum = int32_t( max );

        }

        return true;

      }

    }

  }

  return false;

}
