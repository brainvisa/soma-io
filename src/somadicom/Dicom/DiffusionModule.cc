#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DiffusionModule.h>
#include <soma-io/Dicom/OrientationModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/DiffusionModule.h>
#include <Dicom/OrientationModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::DiffusionModule::DiffusionModule()
                    : dcm::DicomModule()
{
}


bool dcm::DiffusionModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    DcmItem* item = 0;

    if ( dcmItem->findAndGetSequenceItem( DCM_MRDiffusionSequence, 
                                          item ).good() )
    {

      Float64 tmpDouble;

      if ( item->findAndGetFloat64( DCM_DiffusionBValue, tmpDouble ).good() )
      {

        DcmItem* itemDir = 0;
        std::vector< double > direction( 3, 0.0 );

        _bValues.push_back( double( tmpDouble ) );

        if ( item->findAndGetSequenceItem( 
                                         DCM_DiffusionGradientDirectionSequence,
                                         itemDir ).good() )
        {

          int32_t d;

          for ( d = 0; d < 3; d++ )
          {

            if ( itemDir->findAndGetFloat64( DCM_DiffusionGradientOrientation,
                                             tmpDouble,
                                             d ).good() )
            {

              direction[ d ] = double( tmpDouble );

            }

          }

        }

        addDirection( direction );

        return true;

      }

    }

  }

  return false;

}


bool dcm::DiffusionModule::parseHeader( dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  if ( n )
  {

    DcmDataset dataset;
    bool status = true;
    int32_t i, step = getStep( datasetHeader );
    dcm::OrientationModule orientationModule;

    datasetHeader.get( dataset );

    if ( orientationModule.parseItem( &dataset ) )
    {

      _dicomTransform = dcm::DicomTransform3d< double >( 
                                     orientationModule.getRowCosine(),
                                     orientationModule.getColumnCosine(),
                                     dcm::Vector3d< double >( 0.0, 0.0, 0.0 ) );

    }

    _bValues.clear();
    _directions.clear();

    for ( i = 0; status && ( i < n ); i += step )
    {

      datasetHeader.get( dataset, i );
      status &= parseItem( &dataset );

    }

    return status;

  }

  return false;

}


const std::vector< double >& dcm::DiffusionModule::getBValues() const
{

  return _bValues;

}


const std::vector< std::vector< double > >& 
dcm::DiffusionModule::getDirections() const
{

  return _directions;

}


void dcm::DiffusionModule::addDirection( std::vector< double >& direction )
{

  dcm::Vector3d< double > to( direction[ 0 ], direction[ 1 ], direction[ 2 ] );
  dcm::Vector3d< double > from;
  std::vector< double > outDirection( 3, 0.0 );

  _dicomTransform.getInverse( to, from );
  outDirection[ 0 ] = from.x;
  outDirection[ 1 ] = -from.y;
  outDirection[ 2 ] = from.z;
  _directions.push_back( outDirection );

}


int32_t dcm::DiffusionModule::getStep( dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t step = 1;
  DcmDataset dataset;
  OFString position;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient,
                                        position ).good() )
  {

    OFString tmpString;
    int32_t i, tCount = 1, n = datasetHeader.size();

    for ( i = 1; i < n; i++ )
    {

      datasetHeader.get( dataset, i );

      if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient,
                                            tmpString ).good() )
      {

        if ( !tmpString.compare( position ) )
        {

          tCount++;

        }

      }

    }

    step = n / tCount;

  }

  return step;

}
