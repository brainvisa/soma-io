#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomDataContext.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/DicomDataContext.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcxfer.h>

#include <iostream>


soma::DicomDataContext::DicomDataContext( 
                                        soma::DicomDatasetHeader& datasetHeader,
                                        soma::DicomProxy& proxy,
                                        const std::vector< int32_t >& selection,
                                        bool applyModalityLUT )
                      : soma::LoopContext(),
                        _datasetHeader( datasetHeader ),
                        _proxy( proxy ),
                        _selection( selection )
{

  _proxy.getDataInfo()._modalityLUTApplied = applyModalityLUT;

}


void soma::DicomDataContext::doIt( int32_t startIndex, int32_t count )
{

  int32_t i, stopIndex = startIndex + count;
  soma::DataInfo& info = _proxy.getDataInfo();
  int32_t px, py, pz, sizeX, sizeY, sizeZ;
  int32_t bpp = info._bpp;
  int32_t min = ( 1 << info._depth ) - 1;
  int32_t max = 1 - ( 1 << info._depth );
  int32_t startX = info._boundingBox.getLowerX();
  int32_t startY = info._boundingBox.getLowerY();
  int32_t endX = info._boundingBox.getUpperX() + 1;
  int32_t endY = info._boundingBox.getUpperY() + 1;
  unsigned long flags = 0;
  soma::BoundingBox< int32_t > outBoundingBox;

  info._patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

  int32_t offset = startY * sizeX + startX;
  int32_t lineShift = sizeX + startX - endX;

  if ( !info._modalityLUTApplied )
  {

    flags |= CIF_IgnoreModalityTransformation;

  }

  if ( info._noFlip )
  {

    outBoundingBox = info._boundingBox;

  }
  else
  {

    outBoundingBox =
           info._patientOrientation.getDirectBoundingBox( info._boundingBox );

  }

  int32_t bbLowerX = outBoundingBox.getLowerX();
  int32_t bbLowerY = outBoundingBox.getLowerY();
  int32_t bbLowerZ = outBoundingBox.getLowerZ();
  int32_t bbLowerT = outBoundingBox.getLowerT();

  std::vector< std::string >& fileList = _datasetHeader.getFileList();
  std::vector< int32_t >& lut = _datasetHeader.getLut();

  for ( i = startIndex; i < stopIndex; i++ )
  {

    int32_t index = _selection[ i ];
    DcmFileFormat fileFormat;

    if ( fileFormat.loadFile( fileList[ lut[ index ] ].c_str() ).good() )
    {

      DcmDataset* dataset = fileFormat.getDataset();
      DicomImage dcmImage( &fileFormat, dataset->getOriginalXfer(), flags );

      if ( dcmImage.getStatus() == EIS_Normal )
      {

        int32_t x, y;
        int32_t z = index % sizeZ;
        int32_t t = index / sizeZ;
        double dMin = 0.0, dMax = 0.0;
        int32_t representation = dcmImage.getInterData()->getRepresentation();

        dcmImage.getMinMaxValues( dMin, dMax );

        info._pixelRepresentation = representation;

        if ( int32_t( dMin ) < min )
        {

          min = int32_t( dMin );

        }

        if ( int32_t( dMax ) > max )
        {

          max = int32_t( dMax );

        }

        if ( bpp == 3 )
        {

          uint8_t** d = (uint8_t**)dcmImage.getInterData()->getData();
          uint8_t* r = d[ 0 ] + offset;
          uint8_t* g = d[ 1 ] + offset;
          uint8_t* b = d[ 2 ] + offset;
 
          if ( info._noFlip )
          {

            for ( y = startY; y < endY; y++ )
            {

              for ( x = startX; x < endX; x++ )
              {

                uint8_t* pOut = _proxy( x - bbLowerX, 
                                        y - bbLowerY, 
                                        z - bbLowerZ, 
                                        t - bbLowerT );

                *pOut++ = *r++;
                *pOut++ = *g++;
                *pOut = *b++;

              }

              r += lineShift;
              g += lineShift;
              b += lineShift;

            }

          }
          else
          {

            for ( y = startY; y < endY; y++ )
            {

              for ( x = startX; x < endX; x++ )
              {

                info._patientOrientation.getDirect( x, y, z, px, py, pz );

                uint8_t* pOut = _proxy( px - bbLowerX, 
                                        py - bbLowerY, 
                                        pz - bbLowerZ, 
                                        t - bbLowerT );

                *pOut++ = *r++;
                *pOut++ = *g++;
                *pOut = *b++;

              }

              r += lineShift;
              g += lineShift;
              b += lineShift;

            }

          }

        }
        else if ( ( bpp == 2 ) && ( representation < 2 ) )
        {

          uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData() + offset;

          if ( info._noFlip )
          {

            for ( y = startY; y < endY; y++, pIn += lineShift )
            {

              for ( x = startX; x < endX; x++ )
              {

                *( (int16_t*)_proxy( x - bbLowerX, 
                                     y - bbLowerY, 
                                     z - bbLowerZ, 
                                     t - bbLowerT ) ) = (int16_t)*pIn++;

              }

            }

          }
          else
          {

            for ( y = startY; y < endY; y++, pIn += lineShift )
            {

              for ( x = startX; x < endX; x++ )
              {

                info._patientOrientation.getDirect( x, y, z, px, py, pz );

                *( (uint16_t*)_proxy( px - bbLowerX, 
                                      py - bbLowerY, 
                                      pz - bbLowerZ, 
                                      t - bbLowerT ) ) = (uint16_t)*pIn++;

              }

            }

          }

        }
        else
        {

          uint8_t* pIn = (uint8_t*)dcmImage.getInterData()->getData() + 
                         offset * bpp;
          int32_t lineShift2 = lineShift * bpp;

          if ( info._noFlip )
          {

            if ( _proxy.isMemoryMapped() )
            {

              for ( y = startY; y < endY; y++, pIn += lineShift2 )
              {

                for ( x = startX; x < endX; x++, pIn += bpp )
                {

                  std::memcpy( _proxy( x - bbLowerX, 
                                       y - bbLowerY, 
                                       z - bbLowerZ, 
                                       t - bbLowerT ), 
                               pIn, 
                               bpp );

                }

              }

            }
            else
            {

              int32_t length = bpp * ( endX - startX );

              lineShift2 = sizeX * bpp;

              for ( y = startY; y < endY; y++, pIn += lineShift2 )
              {

                std::memcpy( _proxy( startX - bbLowerX, 
                                     y - bbLowerY, 
                                     z - bbLowerZ, 
                                     t - bbLowerT ), 
                             pIn, 
                             length );

              }

            }

          }
          else
          {

            for ( y = startY; y < endY; y++, pIn += lineShift2 )
            {

              for ( x = startX; x < endX; x++, pIn += bpp )
              {

                info._patientOrientation.getDirect( x, y, z, px, py, pz );
                std::memcpy( _proxy( px - bbLowerX, 
                                     py - bbLowerY, 
                                     pz - bbLowerZ, 
                                     t - bbLowerT ), 
                             pIn, 
                             bpp );

              }

            }

          }

        }

      }

    }

  }

  lock();

  if ( min < info._minimum )
  {

    info._minimum = min;

  }

  if ( max > info._maximum )
  {

    info._maximum = max;

  }

  unlock();

}
