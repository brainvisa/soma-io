#ifdef SOMA_IO_DICOM
#include <soma-io/Container/DataInfo.h>
#else
#include <Container/DataInfo.h>
#endif


dcm::DataInfo::DataInfo()
             : _depth( 8 ),
               _spp( 1 ),
               _bitsStored( 8 ),
               _pixelRepresentation( 0 ),
               _invertLut( false ),
               _minimum( 0 ),
               _maximum( 0 ),
               _width( 1 ),
               _height( 1 ),
               _slices( 1 ),
               _frames( 1 ),
               _spacingBetweenSlices( 1.0 ),
               _repetitionTime( 1.0 ),
               _fileCount( 0 ),
               _mosaic( false ),
               _noFlip( false ),
               _noDemosaic( false )
{

  _resolution = dcm::Vector3d< double >( 1.0, 1.0, 1.0 );
  _rowVec = dcm::Vector3d< double >( 1.0, 0.0, 0.0 );
  _colVec = dcm::Vector3d< double >( 0.0, 1.0, 0.0 );
  _origin = dcm::Vector3d< double >( 0.0, 0.0, 0.0 );

  initialize();

}


dcm::DataInfo::DataInfo( const dcm::DataInfo& other )
             : _depth( other._depth ),
               _spp( other._spp ),
               _bitsStored( other._bitsStored ),
               _pixelRepresentation( other._pixelRepresentation ),
               _invertLut( other._invertLut ),
               _minimum( other._minimum ),
               _maximum( other._maximum ),
               _width( other._width ),
               _height( other._height ),
               _slices( other._slices ),
               _frames( other._frames ),
               _resolution( other._resolution ),
               _spacingBetweenSlices( other._spacingBetweenSlices ),
               _repetitionTime( other._repetitionTime ),
               _fileCount( other._fileCount ),
               _rowVec( other._rowVec ),
               _colVec( other._colVec ),
               _origin( other._origin ),
               _patientOrientation( other._patientOrientation ),
               _datasetHeader( other._datasetHeader ),
               _boundingBox( other._boundingBox ),
               _mosaic( other._mosaic ),
               _noFlip( other._noFlip ),
               _noDemosaic( other._noDemosaic )
{

  initialize();

}


dcm::DataInfo::~DataInfo()
{
}


void dcm::DataInfo::clear()
{

  _depth = 8;
  _spp = 1;
  _bpp = 1;
  _bitsStored = 8;
  _pixelRepresentation = 0;
  _invertLut = false;
  _minimum = 0;
  _maximum = 0;
  _width = 1;
  _height = 1;
  _slices = 1;
  _frames = 1;
  _sliceSize = 0;
  _volumeSize = 0;
  _datasetSize = 0;
  _resolution = dcm::Vector3d< double >( 1.0, 1.0, 1.0 );
  _spacingBetweenSlices = 1.0;
  _repetitionTime = 1.0;
  _fileCount = 0;
  _rowVec = dcm::Vector3d< double >( 1.0, 0.0, 0.0 );
  _colVec = dcm::Vector3d< double >( 0.0, 1.0, 0.0 );
  _origin = dcm::Vector3d< double >( 0.0, 0.0, 0.0 );
  _datasetHeader.clear();
  _boundingBox = dcm::BoundingBox< int32_t >();
  _mosaic = false;
  _noFlip = false;
  _noDemosaic = false;

}


void dcm::DataInfo::initialize()
{

  _bpp = ( _depth >> 3 ) * _spp;

  if ( !_maximum )
  {

    _maximum = ( 1 << _depth ) - 1;

  }

  _sliceSize = _width * _height;
  _volumeSize = _sliceSize * _slices;
  _datasetSize = _volumeSize * _frames;

  if ( ( _boundingBox.getLowerX() == 0 ) &&
       ( _boundingBox.getLowerY() == 0 ) &&
       ( _boundingBox.getLowerZ() == 0 ) &&
       ( _boundingBox.getUpperX() == 0 ) &&
       ( _boundingBox.getUpperY() == 0 ) &&
       ( _boundingBox.getUpperZ() == 0 ) )
  {

    dcm:: Vector3d< int32_t > size = _patientOrientation.getOnDiskSize();

    _boundingBox = dcm::BoundingBox< int32_t >( 0, size.x - 1,
                                                0, size.y - 1,
                                                0, size.z - 1,
                                                0, _frames - 1 );

  }

}
