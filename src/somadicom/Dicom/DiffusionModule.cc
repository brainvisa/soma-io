#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/DiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::DiffusionModule::DiffusionModule()
                     : soma::DicomModule()
{
}


bool soma::DiffusionModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmItem* item = 0;

    if ( dataset->findAndGetSequenceItem( DCM_MRDiffusionSequence, 
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

          direction[ 2 ] *= -1.0;

        }

        _directions.push_back( direction );

        return true;

      }

    }

  }

  return false;

}


bool soma::DiffusionModule::parseHeader( 
                                       soma::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  if ( n )
  {

    DcmDataset dataset;
    bool status = true;
    int32_t i, step = getStep( datasetHeader );

    _bValues.clear();
    _directions.clear();

    for ( i = 0; status && ( i < n ); i += step )
    {

      datasetHeader.get( dataset, i );
      status &= parseDataset( &dataset );

    }

    return status;

  }

  return false;

}


const std::vector< double >& soma::DiffusionModule::getBValues() const
{

  return _bValues;

}


const std::vector< std::vector< double > >& 
soma::DiffusionModule::getDirections() const
{

  return _directions;

}


int32_t soma::DiffusionModule::getStep( 
                                       soma::DicomDatasetHeader& datasetHeader )
{

  int32_t step = 1;
  DcmDataset dataset;
  OFString position;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient,
                                        position ).good() )
  {

    OFString tmpString;
    int32_t i, n = datasetHeader.size();

    for ( i = 1; i < n; i++ )
    {

      datasetHeader.get( dataset, i );

      if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient,
                                            tmpString ).good() )
      {

        if ( !tmpString.compare( position ) )
        {

          step++;

        }

      }

    }

  }

  return step;

}

