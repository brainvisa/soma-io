#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/EnhancedIndexModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/EnhancedIndexModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::EnhancedIndexModule::EnhancedIndexModule()
                         : soma::DicomModule(),
                           _numberOfFrames( 0 ),
                           _zIndex( -1 ),
                           _tIndex( 0 ),
                           _zCount( 0 ),
                           _tCount( 1 )
{
}


bool soma::EnhancedIndexModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Sint32 tmpInt;

    if ( dcmItem->findAndGetSint32( DCM_NumberOfFrames, tmpInt ).good() )
    {

      _numberOfFrames = int32_t( tmpInt );

    }
    else
    {

      return false;

    }

    int32_t dim = 0;
    DcmSequenceOfItems* seq = 0;

    if ( dcmItem->findAndGetSequence( DCM_DimensionIndexSequence,
                                      seq ).good() )
    {

      dim = seq->card();

      // Generic version only support up to 4D data structure.
      // Any other data organisation have to be described in a derived class.
      if ( dim > 3 )
      {

        return false;

      }

      int32_t i;
      DcmTagKey stackID( DCM_StackID );
      DcmTagKey inStackPos( DCM_InStackPositionNumber );

      for ( i = 0; i < dim; i++ )
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
           else if ( tmpString.compare( stackID.toString() ) )
           {

             _tIndex = i;

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
      std::vector< std::pair< int32_t, int32_t > > tmpLut( nItems );

      _zCount = 0;

      for ( i = 0; i < nItems; i++ )
      {

        DcmItem* item = seq->getItem( i );
        DcmItem* tmpItem = 0;

        if ( item->findAndGetSequenceItem( DCM_FrameContentSequence,
                                           tmpItem ).good() )
        {

          Uint32 z, t = 1;

          tmpItem->findAndGetUint32( DCM_DimensionIndexValues, z, _zIndex );

          if ( dim == 3 )
          {

            tmpItem->findAndGetUint32( DCM_DimensionIndexValues, t, _tIndex );

          }

          if  ( int32_t( z ) > _zCount )
          {

            _zCount = z;

          }

          tmpLut[ i ] = std::make_pair( z - 1, t - 1 );

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

      for ( i = 0; i < nItems; i++ )
      {

        _indices[ i ] = tmpLut[ i ].second * _zCount + tmpLut[ i ].first;

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


bool soma::EnhancedIndexModule::parseHeader( 
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( datasetHeader.size() == 1 )
  {

    DcmDataset dataset;

    datasetHeader.get( dataset );

    return parseItem( &dataset );

  }

  return false;

}


int32_t soma::EnhancedIndexModule::getNumberOfFrames() const
{

  return _numberOfFrames;

}


int32_t soma::EnhancedIndexModule::getZIndex() const
{

  return _zIndex;

}


int32_t soma::EnhancedIndexModule::getTIndex() const
{

  return _tIndex;

}


int32_t soma::EnhancedIndexModule::getZCount() const
{

  return _zCount;

}


int32_t soma::EnhancedIndexModule::getTCount() const
{

  return _tCount;

}


const std::vector< int32_t >& soma::EnhancedIndexModule::getIndices() const
{

  return _indices;

}

