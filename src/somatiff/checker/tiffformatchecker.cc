/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

//--- tiff library -----------------------------------------------------------
extern "C" {
#include <tiffio.h>
}
//--- plugin -----------------------------------------------------------------
#include <soma-io/checker/tiffformatchecker.h>             // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/datasourcelist.h>
#include <soma-io/datasource/filedatasource.h>  // because we use file sources
#include <soma-io/reader/itemreader.h>                  // to read in the file
#include <soma-io/utilities/asciidatasourcetraits.h>     // to read datasource
#include <soma-io/writer/pythonwriter.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // header
#include <cartobase/object/property.h>                               // header
#include <cartobase/stream/fileutil.h>             // to manipulate file names
#include <cartobase/exception/ioexcept.h>
//--- system -----------------------------------------------------------------
#include <stdio.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "TIFFFORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   U T I L I T I E S
//============================================================================

//============================================================================
//   P R I V A T E   M E T H O D S
//============================================================================
string TiffFormatChecker::_getTiffFileName(const string filename) const
{
  string ext = FileUtil::extension(filename);
  if (ext.empty()) {
    if (FileUtil::fileStat( filename + ".tiff" ).find( '+' ) != string::npos ) {
      return filename + ".tiff";
    }
    else {
      return filename + ".tif";
    }
  }
  else {
    return filename;
  }
}

string TiffFormatChecker::_getMinfFileName(const string filename) const
{
  return filename + ".minf";
}

void TiffFormatChecker::_buildMultiFileFormatInfo( DataSourceInfo & dsi ) const
{
  DataSource* pds = dsi.list().dataSource().get();
  string tiffname = _getTiffFileName( FileUtil::uriFilename( pds->url() ) );
  MultiFileFormatInfo mfi = MultiFileFormat::info(tiffname);
  dsi.privateIOData()->setProperty( "tiff_info", mfi );
}

/*** BUILDING DATASOURCELIST *************************************************
 * This method builds a DataSourceList from the initial datasource.
 * It tries to find .dim, .ima and .minf files
 * If one or several of those files doesn't exist, it writes the initial
 * DataSource instead. Thus, the list returned contains at least one ds for
 * each of the following keywords : "dim", "ima", "minf", "default", but
 * in the worst case they can all be the initial ds.
 ****************************************************************************/
void TiffFormatChecker::_buildDSList( DataSourceList & dsl, const MultiFileFormatInfo & mfi ) const
{
  DataSource* pds = dsl.dataSource().get();
  string defaultname, tiffname, minfname, ext;
  
  // Get the default file name from the given url
  defaultname = tiffname = minfname = FileUtil::uriFilename( pds->url() );
  tiffname = _getTiffFileName( defaultname );
  ext = carto::stringLower(FileUtil::extension(tiffname));
  
  if( defaultname.empty() ) {
    // we suppose ds is a dim file and a imafile
    dsl.addDataSource( "0", rc_ptr<DataSource>( pds ) );
  }
  else if (( ext == "tif") || (ext == "tiff")) {
    // Find filenames from the MultiFileFormatInfo
    const vector<string> & filenames = MultiFileFormat::filenames(mfi);
    
    if (filenames.size() > 0) {
      vector<string>::const_iterator it, ie = filenames.end();
      uint32_t i;
      for (it = filenames.begin(), i = 0; it != ie; ++it, ++i)
        dsl.addDataSource( carto::toString(i), rc_ptr<DataSource>( new FileDataSource( *it ) ) );
    }
    else {
      dsl.addDataSource( "0", rc_ptr<DataSource>( pds ) );
    }
  }
  
  localMsg( "tiff: " + dsl.dataSource( "0" )->url() );
  minfname = _getMinfFileName(dsl.dataSource( "0" )->url());
  dsl.addDataSource( "minf", rc_ptr<DataSource>
                              ( new FileDataSource( minfname ) ) );

  localMsg( "minf: " + dsl.dataSource( "minf" )->url() );
}

/*** BUILDING HEADER *********************************************************
 * This method builds a header from a .dim DataSource.
 * The argument is given by check(...) and is supposed to be a .dim file.
 * However, since the DSList is constructed even if no .dim file is found,
 * it can be an absolutely different format. In this case, the method should 
 * launch an exception at one point which should be caught by DSILoader, 
 * thus knowing this checker cannot read the entry file.
 ****************************************************************************/
Object TiffFormatChecker::_buildHeader( DataSource* hds, const MultiFileFormatInfo & mfi ) const
{
  //FileDataSource* fds = dynamic_cast<FileDataSource *>( hds );
  Object          hdr = Object::value( PropertySet() );  // header
  string          filename = hds->url();
  ushort          typesize, mbps, spp, unit, sampleformat, photometric;
  vector<int32_t> dims(4, 1);
  float           voxelSizeX = 1.0,
                  voxelSizeY = 1.0,
                  voxelSizeZ = 1.0,
                  voxelSizeT = 1.0;
  string          type, diskdatatype;
  ostringstream   oss;
  
  TIFFSetErrorHandler( 0 );
  TIFFSetWarningHandler( 0 );

  localMsg("buildHeader: " + filename )
  TIFF* tif = TIFFOpen( filename.c_str(), "r" );
  if( !tif )
    throw format_mismatch_error( "TIFF file not readable", filename );
    //io_error::launchErrnoExcept( filename );
    //throw file_not_found_error( filename );

  vector<string> pt;
  dims[2] = TIFFNumberOfDirectories(tif);
  
  TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &mbps);
  TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
  TIFFGetFieldDefaulted(tif, TIFFTAG_RESOLUTIONUNIT, &unit);
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &dims[0]);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &dims[1]);
  TIFFGetField(tif, TIFFTAG_XRESOLUTION, &voxelSizeX);
  TIFFGetField(tif, TIFFTAG_YRESOLUTION, &voxelSizeY);
  if( TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleformat) == 0 )
    sampleformat = SAMPLEFORMAT_UINT;
  if( TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) == 0 )
    photometric = PHOTOMETRIC_MINISBLACK;
            
  TIFFClose(tif);
  std::string unitname(std::string(unit == RESUNIT_INCH ? "inch" : 
                       (unit == RESUNIT_CENTIMETER ? "cm" : 
                       "not specified")));
  localMsg("Number of pixels per "
           + unitname + " is ["
           + carto::toString(voxelSizeX) + ", "
           + carto::toString(voxelSizeY) + "]");

  // Number of pixels per centimeters is converted to size of a pixel 
  // in mm or inches
  voxelSizeX = 10 / voxelSizeX;
  voxelSizeY = 10 / voxelSizeY;

  // If size was stored using inches, we convert it to mm 
  if (unit == RESUNIT_INCH) {
    voxelSizeX *= 2.54;
    voxelSizeY *= 2.54;
  }
  
  localMsg("Voxel size (in mm) is ["
           + carto::toString(voxelSizeX) + ", "
           + carto::toString(voxelSizeY) + "]");
    
  type = "";
  diskdatatype = "";

  switch( spp ) {
    case 1:
    case 2:
      switch( sampleformat )
      {
        case SAMPLEFORMAT_VOID:
          switch ( photometric ) {
              case PHOTOMETRIC_PALETTE :
                  // Indexed colormap
                  type = "RGBA";
                  diskdatatype = "RGBA";
                  pt.push_back( "RGBA" );
                  pt.push_back( "RGB" );
                  break;
                  
              default:
                  type = "unknown";
                  diskdatatype = "unknown";
                  break;
          }
          break;
          
        case SAMPLEFORMAT_UINT:
          switch ( photometric ) {
            case PHOTOMETRIC_PALETTE :
              // Indexed colormap
              type = "RGBA";
              diskdatatype = "RGBA";
              pt.push_back( type );
              pt.push_back( "RGB" );
              break;
              
            default:
              typesize = ( spp * mbps );
              if ( ( typesize % 8 ) > 0 ) {
                // Get the next multiple of 8
                typesize = ( ( typesize / 8 ) + 1 ) * 8;
              }
              oss << ( typesize );
              type = "U" + oss.str();
              diskdatatype = type;
              pt.push_back( type );
              pt.push_back( "RGB" );
              pt.push_back( "RGBA" );
              break;
          }
          break;
          
        case SAMPLEFORMAT_INT:
          typesize = ( spp * mbps );
          if ( ( typesize % 8 ) > 0 ) {
            // Get the next multiple of 8
            typesize = ( ( typesize / 8 ) + 1 ) * 8;
          }
          oss << ( typesize );
          type = "S" + oss.str();
          diskdatatype = type;
          pt.push_back( type );
          pt.push_back( "RGB" );
          pt.push_back( "RGBA" );
          break;
          
        case SAMPLEFORMAT_IEEEFP:
          if( mbps == 32 )
            type = "FLOAT";
          else
            type = "DOUBLE";
          diskdatatype = type;
          pt.push_back( type );
          break;
          
        case SAMPLEFORMAT_COMPLEXINT:
        case SAMPLEFORMAT_COMPLEXIEEEFP:
          if( mbps == 32 ) // not sure it is 32 or 64...
            type = "CFLOAT";
          else
            type = "CDOUBLE";
          diskdatatype = type;
          pt.push_back( type );
          break;
        default: // undefined
          type = "unknown";
          diskdatatype = type;
          break;
      }
      break;
      
    case 3:
      type = "RGB";
      diskdatatype = "RGBA";
      pt.push_back( "RGB" );
      pt.push_back( "RGBA" );
      break;
      
    case 4:
      type = "RGBA";
      diskdatatype = "RGBA";
      pt.push_back( "RGBA" );
      pt.push_back( "RGB" );
      break;
    default:
      break;
  }
  
  // Update dimensions using filenames definitions
  if (mfi.initialized)
    MultiFileFormat::updateDimensions(mfi, dims);
    
  // Create voxel size vectors
  vector<float> vs;
  vs.push_back( voxelSizeX );
  vs.push_back( voxelSizeY );
  vs.push_back( voxelSizeZ );
  vs.push_back( voxelSizeT );

  hdr->setProperty( "sizeX", dims[0] );
  hdr->setProperty( "sizeY", dims[1] );
  hdr->setProperty( "sizeZ", dims[2] );
  hdr->setProperty( "sizeT", dims[3] );
  hdr->setProperty( "format", string( "TIFF" ) );
  hdr->setProperty( "voxel_size", vs );
  hdr->setProperty( "object_type", string( "Volume" ) );
  hdr->setProperty( "data_type", type );
  hdr->setProperty( "disk_data_type", diskdatatype );
  if( !pt.empty() )
    hdr->setProperty( "possible_data_types", pt );

  return hdr;
}

//============================================================================
//   P U B L I C   M E T H O D S
//============================================================================

TiffFormatChecker::~TiffFormatChecker()
{
}

DataSourceInfo TiffFormatChecker::check( DataSourceInfo dsi, 
                                         DataSourceInfoLoader & /* f */,
                                         Object options) const
{
  MultiFileFormatInfo mfi;
  bool domultifileformatinfo = !dsi.privateIOData()->hasProperty( "tiff_info" );
  bool doread = dsi.header().isNone() ;
  bool dolist = dsi.list().typecount() == 1 ;
  bool docapa = !dsi.capabilities().isInit();

  //--- read uri -------------------------------------------------------------
  // we don't use any options right now, but we may later ?
  // then, uncomment the following lines to get options from the uri.
//   std::string uri = dsi.list().dataSource( "default", 0 )->url();
//   carto::Object urioptions = FileUtil::uriOptions( uri );
//   if( urioptions.get() ) {
//     options->copyProperties( urioptions );
//   }
  
  //--- test header format ---------------------------------------------------
  if( !doread )
    if( !dsi.header()->hasProperty( "format" ) 
        || dsi.header()->getProperty( "format" )->getString() != "TIFF" )
      throw wrong_format_error( "Not a TIFF header", 
                                dsi.list().dataSource()->url() );
  
  //--- build multifileinfo --------------------------------------------------
  if (domultifileformatinfo) {
    localMsg( "Building multifile format info..." );
    _buildMultiFileFormatInfo( dsi );
  }
  
  //--- build datasourcelist -------------------------------------------------
  if( dolist ) {
    localMsg( "Building list..." );
    dsi.privateIOData()->getProperty( "tiff_info", mfi );
    _buildDSList( dsi.list(), mfi );
  }
  
  //--- build header ---------------------------------------------------------
  if( doread ) {
    localMsg( "Reading header..." );
    // Uses the first datasource to build header
    DataSource* hds = dsi.list().dataSource( "0" ).get();
    dsi.privateIOData()->getProperty( "tiff_info", mfi );
    dsi.header() = _buildHeader( hds, mfi );
    
    localMsg( "Reading minf..." );
    string obtype = dsi.header()->getProperty( "object_type" )->getString();
    string dtype;
    dsi.header()->getProperty( "data_type", dtype );
    DataSource* minfds = dsi.list().dataSource( "minf" ).get();
    DataSourceInfoLoader::readMinf( *minfds, dsi.header(), options );
    dsi.header()->setProperty( "object_type", obtype );
    if( !dtype.empty() )
      dsi.header()->setProperty( "data_type", dtype );
  }
  //--- write capabilities ---------------------------------------------------
  if( docapa ) {
    localMsg ("Writing capabilities..." );
    dsi.capabilities().setDataSource( dsi.list().dataSource( "0" ) );
    dsi.capabilities().setMemoryMapping( false );
    dsi.capabilities().setThreadSafe( true ); /* TODO */
    dsi.capabilities().setOrdered( true );
    dsi.capabilities().setSeekVoxel( true );
    dsi.capabilities().setSeekLine( true );
    dsi.capabilities().setSeekSlice( true );
    dsi.capabilities().setSeekVolume( true );
  }
  //--------------------------------------------------------------------------
  localMsg( "Checking done" );
  return dsi;
}

#undef localMsg
