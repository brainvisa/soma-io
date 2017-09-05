#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Bruker/BrukerEnhancedIndexModule.h>
#else
#include <Dicom/Bruker/BrukerEnhancedIndexModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::BrukerEnhancedIndexModule::BrukerEnhancedIndexModule()
                              : dcm::EnhancedIndexModule()
{
}


bool dcm::BrukerEnhancedIndexModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    int32_t nItems = 0;
    Sint32 tmpInt;
    DcmSequenceOfItems* seq = 0;

    if ( dcmItem->findAndGetSint32( DCM_NumberOfFrames, tmpInt ).good() )
    {

      _numberOfFrames = int32_t( tmpInt );

    }
    else
    {

      return false;

    }

    if ( dcmItem->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      nItems = seq->card();

      if ( nItems < 4 )
      {

        return dcm::EnhancedIndexModule::parseItem( dcmItem );

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

    _indices.resize( _numberOfFrames );

    if ( dcmItem->findAndGetSequence( DCM_PerFrameFunctionalGroupsSequence,
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

      if ( _zCount < _numberOfFrames )
      {

        _tCount = _numberOfFrames / _zCount;

        if ( _numberOfFrames % _zCount )
        {

          _tCount++;

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
