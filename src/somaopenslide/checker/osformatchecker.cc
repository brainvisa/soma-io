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

//--- plugin -----------------------------------------------------------------
#include <soma-io/checker/osformatchecker.h>              // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/datasourcelist.h>
#include <soma-io/datasource/filedatasource.h>  // because we use file sources
#include <soma-io/reader/itemreader.h>               // to read in the file
#include <soma-io/utilities/asciidatasourcetraits.h>     // to read datasource
#include <soma-io/writer/pythonwriter.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // header
#include <cartobase/object/property.h>                               // header
#include <cartobase/stream/fileutil.h>             // to manipulate file names
#include <cartobase/config/verbose.h>                       // verbosity level
//--- system -----------------------------------------------------------------
#include <soma-io/config/os_config.h>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii -> used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#include <cartobase/type/string_conversion.h>
#define localMsg( message ) cartoCondMsg( 4, message, "OSFORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   P R I V A T E   M E T H O D S
//============================================================================
//--- BUILDING DATASOURCELIST ------------------------------------------------
void OSFormatChecker::_buildDSList( DataSourceList & dsl ) const
{
  DataSource* pds = dsl.dataSource().get();
  string imaname, minfname;
  
  imaname = FileUtil::uriFilename( pds->url() );
  if( imaname == pds->url() )
    dsl.addDataSource( "ima", rc_ptr<DataSource>( pds ) );
  else
    dsl.addDataSource( "ima", rc_ptr<DataSource>( new FileDataSource( imaname ) ) );
  
  if( !imaname.empty() ) {
    minfname = imaname + ".minf";
  }
  
  //// Minf DataSource
  if( !minfname.empty() && 
      FileUtil::fileStat( minfname ).find( '+' ) != string::npos ) {
    dsl.addDataSource( "minf", rc_ptr<DataSource>
        ( new FileDataSource( minfname ) ) );
  }
  
  localMsg( "ima: " + dsl.dataSource( "ima" )->url() );
  if( !dsl.empty( "minf" ) )
  {
    localMsg( "minf: " + dsl.dataSource( "minf" )->url() );
  }
}

//--- READING HEADER ---------------------------------------------------------
Object OSFormatChecker::_buildHeader( DataSource* hds, Object options ) const
{
  string  fname = hds->url();
  
  openslide_t *osimage;
  localMsg( "Opening image... " );
  
  if( !(osimage = openslide_open( fname.c_str() )) ) {
    localMsg( "OpenSlide can't open file: " + fname );
    localMsg( "OpenSlide: not recognized format" );
    throw format_mismatch_error( "Not a OpenSlide header", fname );
  }
  if( openslide_get_error( osimage ) ) {
    localMsg( "OpenSlide can't open file : " + fname );
    cout << "OpenSlide: " + string( openslide_get_error( osimage ) ) << endl;
    openslide_close( osimage );
    throw format_mismatch_error( "Not a OpenSlide header", fname );
  }
  localMsg( "Image opened." );
  
  int32_t i;
  
  // resolutions sizes
  localMsg( "Reading sizes..." );
  int32_t rcount = openslide_get_level_count( osimage );
  int64_t sx, sy;
  localMsg( " -> " + carto::toString( rcount ) + " levels." )
  vector<vector<int> > rsizes( rcount, vector<int>( 4, 1 ) );
  for( i=0; i<rcount; i++ ) {
    openslide_get_level_dimensions( osimage, i, &sx, &sy );
    rsizes[i][0] = (int)sx;
    rsizes[i][1] = (int)sy;
  }

  // tiles sizes
  localMsg( "Reading tile sizes..." );    
  vector<vector<int> > tsizes( rcount, vector<int>( 4, 1 ) );
  for(i=0; i<rcount;++i) {
    try {
      carto::stringTo<int>(
                        std::string(
                            openslide_get_property_value(
                                osimage, 
                                std::string("openslide.level[" 
                                + carto::toString(i) 
                                + "].tile-width")
                                .c_str())),
                        tsizes[i][0]);
      carto::stringTo<int>(
                        std::string(
                            openslide_get_property_value(
                                osimage, 
                                std::string("openslide.level[" 
                                + carto::toString(i) 
                                + "].tile-height")
                                .c_str())),
                        tsizes[i][1]);
      
      localMsg( "Level " + carto::toString(i) + " tile sizes: " 
                + carto::toString(tsizes[i][0]) 
                + "x" + carto::toString(tsizes[i][1]));
    } catch( ... ) {
    }  
  }
  
  int32_t resolution = -1;
  try {
    if( options.get() ) {
      resolution = options->getProperty( "resolution_level" )->getScalar();
    }
  } catch( ... ) {
  }
  
  if (resolution < 0) {
    // Try to solve negative level values
    resolution += rcount;
  }
  
  if ((resolution < 0) || (resolution >= rcount)) {
      resolution = rcount - 1;
  }
  
  // chosen resolution's downsampling
  float ds = rsizes[0][0]/rsizes[resolution][0];
  
  // chosen resolution's voxel size
  localMsg( "Reading voxel size..." );
  vector<float>  vs(4, 1.);
  const char *mppx = openslide_get_property_value( osimage, "openslide.mpp-x" );
  const char *mppy = openslide_get_property_value( osimage, "openslide.mpp-y" );
  if( mppx )
    vs[0] = atof( mppx )/1000*ds;
  else
    vs[0] = 1;
  if( mppy )
    vs[1] = atof( mppy )/1000*ds;
  else
    vs[1] = 1;
  
  openslide_close( osimage );
  localMsg( "Image closed." );
  
  string type = "RGBA";
  
  Object  data = Object::value( PropertySet() );  // data contains both header
                                                  // and private read data
  // Set header properties
  Object  hdrdata = Object::value( PropertySet() );  // header
  hdrdata->setProperty( "sizeX", (int)rsizes[resolution][0] );
  hdrdata->setProperty( "sizeY", (int)rsizes[resolution][1] );
  hdrdata->setProperty( "sizeZ", (int)rsizes[resolution][2] );
  hdrdata->setProperty( "sizeT", (int)rsizes[resolution][3] );
  vector<int> dims;
  dims.reserve( rsizes[resolution].size() );
  vector<int>::const_iterator irs, ers = rsizes[resolution].end();
  for( irs=rsizes[resolution].begin(); irs!=ers; ++irs )
    dims.push_back( (int) *irs );
  hdrdata->setProperty( "volume_dimension", dims );
  hdrdata->setProperty( "resolutions_dimension", rsizes );
  hdrdata->setProperty( "format", string( "OpenSlide" ) );
  hdrdata->setProperty( "voxel_size", vs );
  hdrdata->setProperty( "object_type", "Volume" );
  hdrdata->setProperty( "data_type", type );
  
  // Set private data properties
  Object  privatedata = Object::value( PropertySet() );  // private data
  privatedata->setProperty( "tile_size", tsizes );
  
  // Set header and private data in data returned
  data->setProperty( "header", hdrdata );
  data->setProperty( "privatedata", privatedata );
  
  return data;
}

//============================================================================
//   P U B L I C   M E T H O D S
//============================================================================

OSFormatChecker::~OSFormatChecker()
{
}

DataSourceInfo OSFormatChecker::check( DataSourceInfo dsi, 
                                       DataSourceInfoLoader & /* f */,
                                       Object options ) const
{
  bool doread = dsi.header().isNone() ;
  bool dolist = dsi.list().typecount() == 1 ;
  bool docapa = !dsi.capabilities().isInit();
  
  //--- read uri -------------------------------------------------------------
  std::string uri = dsi.list().dataSource()->url();
  carto::Object urioptions = FileUtil::uriOptions( uri );
  if( urioptions.get() ) {
    options->copyProperties( urioptions );
  }
  
  //--- test header format ---------------------------------------------------
  if( !doread )
    if( !dsi.header()->hasProperty( "format" ) 
        || dsi.header()->getProperty( "format" )->getString() != "OpenSlide" )
      throw wrong_format_error( "Not a OpenSlide header", 
                                dsi.list().dataSource()->url() );
  
  //--- build datasourcelist -------------------------------------------------
  if( dolist ) {
    localMsg( "Building list..." );
    _buildDSList( dsi.list() );
  }
  //--- build header ---------------------------------------------------------
  if( doread ) {
    localMsg( "Reading header..." );
    DataSource* hds = dsi.list().dataSource( "ima" ).get();
    carto::Object data = _buildHeader( hds, options );
    
    dsi.header() = data->getProperty( "header" );
    dsi.privateIOData() = data->getProperty( "privatedata" );   
    
    localMsg( "Reading minf..." );
    if( !dsi.list().empty( "minf" ) ) {
      string obtype = dsi.header()->getProperty( "object_type" )->getString();
      DataSource* minfds = dsi.list().dataSource( "minf" ).get();
      DataSourceInfoLoader::readMinf( *minfds, dsi.header() );
      dsi.header()->setProperty( "object_type", obtype );
    }
    
  }
  //--- write capabilities ---------------------------------------------------
  if( docapa ) {
    localMsg( "Writing capabilities..." );
    dsi.capabilities().setMemoryMapping( false );
    dsi.capabilities().setDataSource( dsi.list().dataSource( "ima" ) );
    dsi.capabilities().setThreadSafe( false ); /* TODO */
    dsi.capabilities().setOrdered( true );
    dsi.capabilities().setSeekVoxel( true );
    dsi.capabilities().setSeekLine( true );
    dsi.capabilities().setSeekSlice( true );
    dsi.capabilities().setSeekVolume( true );
    dsi.capabilities().setRandomAccessEfficient( false );
    dsi.capabilities().setHandleStrides( true );
  }
  //--------------------------------------------------------------------------
  localMsg( "Checking done" );
  return dsi;
}

#undef localMsg
