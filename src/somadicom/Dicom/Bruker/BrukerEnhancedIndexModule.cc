#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Bruker/BrukerEnhancedIndexModule.h>
#else
#include <Dicom/Bruker/BrukerEnhancedIndexModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::BrukerEnhancedIndexModule::BrukerEnhancedIndexModule()
                                : soma::EnhancedIndexModule()
{
}


bool soma::BrukerEnhancedIndexModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    int32_t nItems = 0;
    Sint32 tmpInt;
    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSint32( DCM_NumberOfFrames, tmpInt ).good() )
    {

      _numberOfFrames = int32_t( tmpInt );

    }

    _indices.resize( _numberOfFrames );

    if ( dataset->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      nItems = seq->card();

      if ( nItems < 4 )
      {

        return soma::EnhancedIndexModule::parseDataset( dataset );

      }

      int32_t i;
      DcmTagKey inStackPos( DCM_InStackPositionNumber );

      for ( i = 0; i < nItems; i++ )
      {

        OFString tmpString;
        DcmItem* item = seq->getItem( i );

        if ( item->findAndGetOFString( DCM_DimensionIndexPointer, 
                                       tmpString ).good() )
        {

          if ( !tmpString.compare( inStackPos.toString() ) )
          {

            _zIndex = i;

          }

        }

      }

    }

    if ( _zIndex < 0 )
    {

      return false;

    }

    if ( dataset->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence,
                                      seq ).good() )
    {

      uint32_t i, nItems = seq->card();

      _zCount = 0;

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_FrameContentSequence,
                                           tmpItem ).good() )
        {

          Uint16 f;
          Uint32 z;

          tmpItem->findAndGetUint16( DCM_FrameAcquisitionNumber, f );

          _indices[ i ] = f - 1;

          tmpItem->findAndGetUint32( DCM_DimensionIndexValues, z, _zIndex );

          if  ( int32_t( z ) > _zCount )
          {

            _zCount = z;

          }

        }

      }

    }
    else
    {

      int32_t i;

      for ( i = 0; i < _numberOfFrames; i++ )
      {

        _indices[ i ] = i;

      }

    }

    return true;

  }
  return false;

}
