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
#include <soma-io/checker/gisformatchecker.h>             // class declaration
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
//--- system -----------------------------------------------------------------
#include <stdio.h>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii -> used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "GISFORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   U T I L I T I E S
//============================================================================
namespace
{

  //--- helper for reading attributes ----------------------------------------
  template<typename T> 
  static void inHelper( Object & object, const string & semantic, 
                        DataSource & ds )
  {
    if( !ds.isOpen() )
      return;

    T i = 0;
    ds >> i;
    object->setProperty( semantic, i );
  }

  template<typename T> 
  static void inVecHelper( Object & object, const string & semantic, 
                           DataSource & ds )
  {
    vector<T> iv;
    while (1)
      {
        if( !ds.isOpen() )
          return;

        int c = ds.getch();
        while (c != '\n' && c != '\0' && c != EOF 
               && (c == ' ' || c == '\t'))
          c = ds.getch();
        if (c == '\n' || c == '\0' || c == EOF)
          break;
        else
          ds.ungetch( c );
        T i;
        ds >> i;
        iv.push_back(i);
      }
    object->setProperty( semantic, iv );
  }

}

//============================================================================
//   P R I V A T E   M E T H O D S
//============================================================================
/*** BUILDING DATASOURCELIST *************************************************
 * This method builds a DataSourceList from the initial datasource.
 * It tries to find .dim, .ima and .minf files
 * If one or several of those files doesn't exist, it writes the initial
 * DataSource instead. Thus, the list returned contains at least one ds for
 * each of the following keywords : "dim", "ima", "minf", "default", but
 * in the worst case they can all be the initial ds.
 ****************************************************************************/
void GisFormatChecker::_buildDSList( DataSourceList & dsl ) const
{
  DataSource* pds = dsl.dataSource().get();
  string dimname, imaname, minfname;

  dimname = imaname = minfname = FileUtil::uriFilename( pds->url() );
  if( dimname.empty() ) {
    // we suppose ds is a dim file and a imafile
    dsl.addDataSource( "dim", rc_ptr<DataSource>( pds ) );
    dsl.addDataSource( "ima", rc_ptr<DataSource>( pds ) );
  } else {
    string ext = FileUtil::extension( dimname );
    string basename = FileUtil::removeExtension( dimname );
    
    if( ext == "ima" ) {
      dimname = basename + ".dim";
    } else if( ext == "dim" ) {
      imaname = basename + ".ima";
    } else {
      ext.clear();
    }
    
    if( ext.empty() ) {
      basename = dimname;
      dimname += ".dim";
      imaname += ".ima";
    }
    minfname = imaname + ".minf";
    
    if( dimname == pds->url() ) {
      // if dimname is original url
      dsl.addDataSource( "dim", rc_ptr<DataSource>( pds ) );
    } else {
      if( FileUtil::fileStat( dimname ).find( '+' ) != string::npos ) {
        // if dim file exists
        dsl.addDataSource( "dim", rc_ptr<DataSource>
            ( new FileDataSource( dimname ) ) );
      } else {
        // we suppose original url is dim
        dsl.addDataSource( "dim", rc_ptr<DataSource>( pds ) );
      }
    }
    if( imaname == pds->url() ) {
      // if imaname is original url
      dsl.addDataSource( "ima", rc_ptr<DataSource>( pds ) );
    } else {
      if( FileUtil::fileStat( imaname ).find( '+' ) != string::npos ) {
        // if ima file exists
        dsl.addDataSource( "ima", rc_ptr<DataSource>
            ( new FileDataSource( imaname ) ) );
      } else {
        // we suppose original url is ima
        dsl.addDataSource( "ima", rc_ptr<DataSource>( pds ) );
      }
    }
  }
  localMsg( "dim: " + dsl.dataSource( "dim" )->url() );
  localMsg( "ima: " + dsl.dataSource( "ima" )->url() );
  
  //// Minf DataSource
  if( FileUtil::fileStat( minfname ).find( '+' ) != string::npos ) {
    // if minf file exist
    dsl.addDataSource( "minf", rc_ptr<DataSource>
        ( new FileDataSource( minfname ) ) );
  } else {
    // we suppose original url is minf
    dsl.addDataSource( "minf", rc_ptr<DataSource>( pds ) );
  }
  
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
Object GisFormatChecker::_buildHeader( DataSource* hds ) const
{
  FileDataSource* fds = dynamic_cast<FileDataSource *>( hds );
  string          fname = hds->url();
  
  if( !hds->isOpen() )
    hds->open( DataSource::Read );
  if( !hds->isOpen() )
    io_error::launchErrnoExcept( fname );
  
  DefaultAsciiItemReader<int>     iir;
  DefaultAsciiItemReader<string>  sir;
  DefaultAsciiItemReader<float>   fir;

  string         token, type, byteord, binar;
  vector<int>    dims;
  int            dim;
  Object         hdr = Object::value( PropertySet() );  // header
  int            c;

  //--- reading sizex, sizey, sizez, sizet -----------------------------------
  if( !StreamUtil::skip( *hds, " \t\n\r" ) )
    throw wrong_format_error( fname );
  c = hds->getch();
  hds->ungetch( c );
  while( c != '\n' && c != '\r' )
  {
    if( iir.read( *hds, &dim ) != 1 )
      throw wrong_format_error( fname );
    else
      dims.push_back( dim );
    if( !StreamUtil::skip( *hds, " \t" ) )
      throw wrong_format_error( fname );
    c = hds->getch();
    hds->ungetch( c );
  }
  size_t ndim = dims.size();
  if( ndim < 3 )
    ndim = 3;
  vector<float>  vs( ndim, 1. );

  const Syntax  &sx = DataSourceInfoLoader::minfSyntax()[ "__generic__" ];

  //--- reading tokens -------------------------------------------------------
  while( hds->isOpen() ) {
    if( !StreamUtil::skip( *hds, " \t\n\r" ) )
      break;
    c = hds->getch();
    if( c != '-' ) {
      hds->ungetch( c );
      break;
    }
    if( sir.read( *hds, &token ) != 1 ) {
      if( token.empty() )
        break;
      else
        throw wrong_format_error( fname );
    }
    if( !StreamUtil::skip( *hds, " \t" ) )
      throw wrong_format_error( fname );
    
    if( token == "type" )
      sir.read( *hds, &type );
    else if (token == "dx" )
      fir.read( *hds, &vs[0] );
    else if (token == "dy" )
      fir.read( *hds, &vs[1] );
    else if (token == "dz" )
      fir.read( *hds, &vs[2] );
    else if (token == "dt" )
    {
      if( vs.size() < 4 )
        vs.resize( 4 );
      fir.read( *hds, &vs[3] );
    }
    else if (token == "dx4" )
    {
      if( vs.size() < 5 )
        vs.resize( 5 );
      fir.read( *hds, &vs[4] );
    }
    else if (token == "dx5" )
    {
      if( vs.size() < 6 )
        vs.resize( 6 );
      fir.read( *hds, &vs[5] );
    }
    else if (token == "dx6" )
    {
      if( vs.size() < 7 )
        vs.resize( 7 );
      fir.read( *hds, &vs[6] );
    }
    else if (token == "dx7" )
    {
      if( vs.size() < 8 )
        vs.resize( 8 );
      fir.read( *hds, &vs[7] );
    }
    else if (token == "bo" )
      sir.read( *hds, &byteord );
    else if (token == "om" )
      sir.read( *hds, &binar );
    else if ( !token.empty() ) {
      Syntax::const_iterator  ix = sx.find( token );
      if( ix == sx.end() ) {
        string  attval;
        StreamUtil::getline( *hds, attval );
        hdr->setProperty( token, attval );
      } else {
        typedef void (*Helper)( Object &, const string &, DataSource & );
        typedef map<string, Helper> HelperSet;
        
        static HelperSet      hs;
        if( hs.empty() ) {
          hs[ "int" ] = &inHelper<int>;
          hs[ "float" ] = &inHelper<float>;
          hs[ "bool" ] = &inHelper<bool>;
          hs[ "string" ] = &inHelper<string>;
          hs[ "int_vector" ] = &inVecHelper<int>;
          hs[ "float_vector" ] = &inVecHelper<float>;
          //hs[ "string_vector" ] = &inVecHelper<string>;
        }
        
        HelperSet::const_iterator ih = hs.find( ix->second.type );
        if( ih == hs.end() ) {
          string  attval;
          StreamUtil::getline( *hds, attval );
          hdr->setProperty( token, attval );
        } 
        else
          ih->second( hdr, token, *hds );
      }
    }
    else
      break;
  }
  
  if( type.empty() ) {
    localMsg( "Not a GIS header: " + fname );
    throw format_mismatch_error( "Not a GIS header", fname );
  }

  if( fds && fds->at() != fds->size() - 1 ) {
    localMsg( "GIS header not entirely read - garbage at end? " + fname );
    throw format_mismatch_error( "GIS header not entirely read - garbage at " 
                                 "end ?", fname );
  }

  hdr->setProperty( "volume_dimension", dims );
  if( dims.size() >= 1 )
  {
    hdr->setProperty( "sizeX", dims[0] );
    if( dims.size() >= 2 )
    {
      hdr->setProperty( "sizeY", dims[1] );
      if( dims.size() >= 3 )
      {
        hdr->setProperty( "sizeZ", dims[2] );
        if( dims.size() >= 4 )
          hdr->setProperty( "sizeT", dims[3] );
      }
    }
  }
  
  hdr->setProperty( "format", string( "GIS" ) );
  hdr->setProperty( "voxel_size", vs );
  // the following shape is more logical, but incompatible with AIMS.
  // hdr->setProperty( "object_type", string( "Volume of " ) + type );
  hdr->setProperty( "object_type", string( "Volume" ) );
  hdr->setProperty( "data_type", type );
  bool  ascii = ( binar == "ascii" );
  hdr->setProperty( "ascii", (int) ascii );
  if( !ascii )
    if( byteord.length() >= 4 ) {
      uint32_t  bo = *( (uint32_t*) byteord.c_str() );
      hdr->setProperty( "byte_swapping", (int) ( bo != SOMAIO_BYTE_ORDER ) );
    }
  
  return hdr;
}

//============================================================================
//   P U B L I C   M E T H O D S
//============================================================================

GisFormatChecker::~GisFormatChecker()
{
}

DataSourceInfo GisFormatChecker::check( DataSourceInfo dsi, 
                                        DataSourceInfoLoader & /* f */,
                                        Object options) const
{
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
        || dsi.header()->getProperty( "format" )->getString() != "GIS" )
      throw wrong_format_error( "Not a GIS header", 
                                dsi.list().dataSource()->url() );

  //--- build datasourcelist -------------------------------------------------
  if( dolist ) {
    localMsg( "Building list..." );
    _buildDSList( dsi.list() );
  }
  //--- build header ---------------------------------------------------------  
  if( doread ) {
    localMsg( "Reading header..." );
    DataSource* hds = dsi.list().dataSource( "dim" ).get();    
    dsi.header() = _buildHeader( hds );
    
    localMsg( "Reading MINF..." );
    string obtype = dsi.header()->getProperty( "object_type" )->getString();
    string dtype;
    dsi.header()->getProperty( "data_type", dtype );
    DataSource* minfds = dsi.list().dataSource( "minf" ).get();
    DataSourceInfoLoader::readMinf( *minfds, dsi.header(), options );
    localMsg( "MINF read" );
    // Fixes wrong dimension properties
    vector<int> dims( 4, 1 );
    if( dsi.header()->getProperty( "volume_dimension", dims ) ) {
      int32_t dsize = dims.size();
      for(int32_t d = 0; d < dsize; ++d)
          if (dims[d] <= 0)
              dims[d] = 1;
      
      if( dims.size() < 4 )
      {
        dims.resize( 4, 1 );
        dsi.header()->setProperty( "volume_dimension", dims );
      }
    }
    
    int32_t size = 0;
    if (dsi.header()->getProperty("sizeX", size))
      if (size <= 0)
          dsi.header()->setProperty("sizeX", 1);
    
    if (dsi.header()->getProperty("sizeY", size))
      if (size <= 0)
          dsi.header()->setProperty("sizeY", 1);
    
    if (dsi.header()->getProperty("sizeZ", size))
      if (size <= 0)
          dsi.header()->setProperty("sizeZ", 1);
    
    if (dsi.header()->getProperty("sizeT", size))
      if (size <= 0)
          dsi.header()->setProperty("sizeT", 1);
    
    dsi.header()->setProperty( "object_type", obtype );
    if( !dtype.empty() )
      dsi.header()->setProperty( "data_type", dtype );
  }
  
  //--- write capabilities ---------------------------------------------------
  if( docapa ) {
    localMsg ("Writing capabilities..." );
    dsi.capabilities().setMemoryMapping( false );
    try {
      if( !(bool) dsi.header()->getProperty( "byte_swapping" )->getScalar() 
          && !(bool) dsi.header()->getProperty( "ascii" )->getScalar() )
        dsi.capabilities().setMemoryMapping( true );
    } catch( ... ) {}
    dsi.capabilities().setDataSource( dsi.list().dataSource( "ima" ) );
    dsi.capabilities().setThreadSafe( true ); /* TODO */
    dsi.capabilities().setOrdered( true );
    dsi.capabilities().setSeekVoxel( true );
    dsi.capabilities().setSeekLine( true );
    dsi.capabilities().setSeekSlice( true );
    dsi.capabilities().setSeekVolume( true );
    dsi.capabilities().setRandomAccessEfficient( true );
    dsi.capabilities().setHandleStrides( true );
  }
  //--------------------------------------------------------------------------
  localMsg( "Checking done" );
  return dsi;
}

#undef localMsg
