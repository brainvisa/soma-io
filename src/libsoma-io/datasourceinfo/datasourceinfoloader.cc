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

//--- soma io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>  // class declaration
#include <soma-io/datasourceinfo/datasourceinfo.h>        // returned by check
#include <soma-io/checker/formatchecker.h>                          // check()
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/io/reader.h>              // readMinf: reader<GenericObject>
//--- cartobase --------------------------------------------------------------
#include <cartobase/exception/ioexcept.h>                 // launch exceptions
#include <cartobase/exception/file.h>                     // launch exceptions
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/plugin/plugin.h>                          // loads plugins
#include <cartobase/stream/fileutil.h>              // used to find extensions
#include <cartobase/config/verbose.h>                       // verbosity level
//--- system -----------------------------------------------------------------
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <map>
#include <set>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "DSILOADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//    U T I L I T I E S
//============================================================================

namespace
{

  struct Private_Static
  {
    Private_Static();
    map<string, rc_ptr<FormatChecker> >  formats;
    multimap<string, string>             extensions;
  };


  Private_Static::Private_Static()
  {
  }


  inline Private_Static & getstatic()
  {
    static  unique_ptr<Private_Static>	ps( new Private_Static );
    return  *ps;
  }

}


struct DataSourceInfoLoader::Private
{
  Private();

  State   state;
  string  errormsg;
  int     errorcode;
};


DataSourceInfoLoader::Private::Private()
  : state( Unchecked ), errorcode( -1 )
{
}

//============================================================================
//    C R E A T O R S
//============================================================================

DataSourceInfoLoader::DataSourceInfoLoader()
  : d( new Private )
{
}


DataSourceInfoLoader::~DataSourceInfoLoader()
{
  delete d;
}

//============================================================================
//    M E T H O D S
//============================================================================

DataSourceInfoLoader::State DataSourceInfoLoader::state() const
{
  return d->state;
}


string DataSourceInfoLoader::errorMessage() const
{
  return d->errormsg;
}


void DataSourceInfoLoader::launchException()
{
  if( d->state != Error || d->errorcode < 0 )
    return;	// no error (?)
  string	defmsg;
  if( d->errormsg.empty() )
    defmsg = "could not identify format";
  io_error::launchExcept( d->errorcode, d->errormsg, defmsg );
}


void DataSourceInfoLoader::registerFormat( const string & fmtid, 
                                           FormatChecker* format, 
                                           const vector<string> & extensions )
{
  Private_Static	& ps = getstatic();
  ps.formats[ fmtid ].reset( format );

  vector<string>::const_iterator	ie, ee = extensions.end();
  for( ie=extensions.begin(); ie!=ee; ++ie )
    ps.extensions.insert( pair<string, string>( *ie, fmtid ) );
}


FormatChecker* DataSourceInfoLoader::formatInfo( const string & format )
{
  Private_Static	& ps = getstatic();

  map<string, rc_ptr<FormatChecker> >::const_iterator	i 
    = ps.formats.find( format );
  if( i == ps.formats.end() )
    return( 0 );
  return( i->second.get() );
}


set<string> DataSourceInfoLoader::extensions( const string & format )
{
  Private_Static        & ps = getstatic();
  multimap<string, string>::const_iterator ie, ee = ps.extensions.end();
  set<string> out_exts;
  for( ie=ps.extensions.begin(); ie!=ee; ++ie )
    if( ie->second == format )
      out_exts.insert( ie->first );
  return out_exts;
}

set<string> DataSourceInfoLoader::formats( const string & ext )
{
  Private_Static        & ps = getstatic();
  pair<multimap<string, string>::const_iterator, 
       multimap<string, string>::const_iterator> ir = ps.extensions.equal_range(
                                                                       ext
                                                                    );
  set<string> out_formats;
  for(multimap<string, string>::const_iterator it=ir.first; it!=ir.second; ++it)
    out_formats.insert(it->second);
  
  return out_formats;
}

set<string> DataSourceInfoLoader::formats(
    const string & ext,
    IOObjectTypesDictionary::FormatInfo & format_info
)
{
  const set<string> formats_ext = DataSourceInfoLoader::formats(ext);
  const set<string> formats_info = format_info();
  vector<string> formats(
      max(formats_info.size(), formats_ext.size())
  );
  vector<string>::iterator it = set_intersection(
      formats_info.begin(), formats_info.end(),
      formats_ext.begin(), formats_ext.end(),
      formats.begin()
  );       
  formats.resize(it - formats.begin());
    
  // Soma formats were found in the intersection that matches object type,
  // data type and extension requirements       
  return set<string>(formats.begin(), formats.end());
}

set<string> DataSourceInfoLoader::readFormats(const string & ext,
                                              const string & data_type_name)
{
  if (!data_type_name.empty()) {
    IOObjectTypesDictionary::FormatInfo format_info 
             = IOObjectTypesDictionary::readTypes()[data_type_name];
           
    return DataSourceInfoLoader::formats(ext, format_info);
  }
  
  return set<string>();
}

set<string> DataSourceInfoLoader::writeFormats(const string & ext,
                                               const string & data_type_name)
{
  if (!data_type_name.empty()) {
    IOObjectTypesDictionary::FormatInfo format_info 
             = IOObjectTypesDictionary::writeTypes()[data_type_name];
           
    return DataSourceInfoLoader::formats(ext, format_info);
  }
  
  return set<string>();
}


namespace
{
  // set identified extension and format in DataSourceInfo
  void setFileExtension( DataSourceInfo & dsi, const string & url,
                         const string & format,
                         const multimap<string, string> & ext_map )
  {
    dsi.setIdentifiedFormat( format );
    multimap<string, string>::const_iterator ie, ee = ext_map.end();
    size_t size = 0, flen = url.length(), elen;
    string longest;
    for( ie=ext_map.begin(); ie!=ee; ++ie )
      if( ie->second == format )
      {
        elen = ie->first.length();
        if( elen > size
            && url.substr( flen - elen - 1, elen + 1 )
               == string(".") + ie->first )
        {
          longest = ie->first;
          size = elen;
        }
      }
    if( size > 0 )
      dsi.setIdentifiedFileExtension( longest );
  }

}

//============================================================================
//    C H E C K I N G   F O R M A T
//============================================================================

//--- useful typedef ---------------------------------------------------------
  typedef std::multimap<std::string,std::string> multi_S;
  typedef std::set<std::string> set_S;
  typedef std::pair<std::multimap<std::string, std::string>::const_iterator, 
      std::multimap<std::string, std::string>::const_iterator> pair_cit_S;
//----------------------------------------------------------------------------

DataSourceInfo DataSourceInfoLoader::check( DataSourceInfo dsi,
                                            Object options,
                                            int passbegin, int passend )
{
  localMsg( "check()" );

  // If dsi already complete : returns it ////////////////////////////////////
  std::string format;
  if( options.get() ) {                                           // try options
    options->getProperty( "format", format );
    localMsg( "options format: " + format );
  }
  
  if( !dsi.header().isNone()
      && dsi.capabilities().isInit() 
      && dsi.list().typecount() > 1
      && ( format.empty() || format == dsi.identifiedFormat() ) )
    return dsi;

  // Init ////////////////////////////////////////////////////////////////////
  static bool plugs = false;
  if( !plugs ) {
    plugs = true;
    PluginLoader::load();
  }

  d->state = Unchecked;
  d->errorcode = -1;
  d->errormsg = "";

  if( !options.get() )
    options = carto::Object::value( carto::PropertySet() );

  //// Reading URI ///////////////////////////////////////////////////////////
  std::string url = FileUtil::uriFilename( dsi.list().dataSource()->url() );
  carto::Object urioptions 
    = FileUtil::uriOptions( dsi.list().dataSource()->url() );
  if( urioptions.get() )
    options->copyProperties( urioptions );

  // find filename extension if it's a file //////////////////////////////////
  string  ext;
  list<string> exts;
  string cut_url = FileUtil::basename( url );
  int     excp = 0;
  localMsg( "filename: " + url );
  do
  {
    ext = FileUtil::extension( cut_url );
    if( !ext.empty() )
    {
      if( !exts.empty() )
        exts.push_front( ext + "." + *exts.begin() );
      else
        exts.push_front( ext );
      localMsg( "ext: " + ext );
      cut_url = cut_url.substr( 0, cut_url.length() - 1 - ext.length() );
    }
  }
  while( !ext.empty() );

  // Check compatible formats ////////////////////////////////////////////////
  set<string>            tried;
  FormatChecker          *reader;
  set<string>::iterator  notyet = tried.end();
  Private_Static         &ps = getstatic();

  pair_cit_S               iext;
  multi_S::const_iterator  ie, ee;
  DataSource*              pds = dsi.list().dataSource().get();
  soma::offset_t           dspos = pds->at();

  /***************************************************************************
   * Following code allows to deal with format hint as with readers
   * and writers. I guess it could be useful for pyramidal tiff files
   * which can be read by both classical tiff checker (maybe?) and
   * openslide (and surely other formats with several plugin implems).
   * However, I do not have such a tiff file to test it so I leave it
   * commented here. If it worked, I guess it would be needed to change
   * passes numbering here and where it is used (aims Finder), or it can
   * be left with the same number as pass 1 considering they should both
   * be called at the same time (numbering was supposed to avoid testing
   * all soma checkers before compatible aims finders)
   *
   * It is also useful after a 1st attempt, to try another format, in
   * FormatReader.
   **************************************************************************/

  //// Pass 0 : priority to format hint //////////////////////////////////////
  if( format.empty() && dsi.header() ) {                           // try header
    dsi.header()->getProperty( "format", format );
    localMsg( "header format property: " + format );
  }
  
  if( format.empty() && dsi.header() ) {                           // try deprecated name
    dsi.header()->getProperty( "file_type", format );
    localMsg( "header file_type property: " + format );
  }
  
  localMsg( "format: " + format );

  if( passbegin <= 0 && passend >= 0 && !format.empty() )
  {
    reader = formatInfo( format );
    if( reader )
    {
      localMsg( "0. trying " + format + "..." );
      try
      {
        d->state = Ok;
        DataSourceInfo dsi2 = reader->check( dsi, *this, options );
        setFileExtension( dsi2, url, format, ps.extensions );
        return dsi2;
      }
      catch( exception & e )
      {
        localMsg( "0. failed : " + string( e.what() ) );
        d->state = Error;
        io_error::keepExceptionPriority( e, excp, d->errorcode,
                                         d->errormsg );
        pds->at( dspos );
      }
      tried.insert( format );
    }
  }

  //// Pass 1 : try every matching format until one works ////////////////////
  if( passbegin <= 1 && passend >= 1 )
  {
    list<string>::const_iterator iel, eel = exts.end();
    for( iel=exts.begin(); iel!=eel; ++iel )
    {
      ext = *iel;
      iext = ps.extensions.equal_range( ext );
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie )
        if( tried.find( ie->second ) == notyet )
        {
          localMsg( "1. trying " + (*ie).second + "..." );
          reader = formatInfo( ie->second );
          if( reader )
          {
            try
            {
              d->state = Ok;
              DataSourceInfo dsi2 = reader->check( dsi, *this, options );
              setFileExtension( dsi2, url, ie->second, ps.extensions );
              return dsi2;
            }
            catch( exception & e )
            {
              localMsg( "1. failed : " + string( e.what() ) );
              d->state = Error;
              io_error::keepExceptionPriority( e, excp, d->errorcode,
                                              d->errormsg );
              pds->at( dspos );
            }
            tried.insert( ie->second );
          }
        }
    }
  }

  //// Pass 2 : not found or none works: try readers with no extension ///////
  if( passbegin <= 2 && passend >= 2 && !exts.empty() )
  {
    localMsg( "not found yet... pass2..." );
    iext = ps.extensions.equal_range( "" );
    for( ie=iext.first, ee=iext.second; ie!=ee; ++ie )
      if( tried.find( ie->second ) == notyet )
      {
        localMsg( "2. trying " + (*ie).second + "..." );
        reader = formatInfo( ie->second );
        if( reader )
        {
          try
          {
            d->state = Ok;
            DataSourceInfo dsi2 = reader->check( dsi, *this, options );
            setFileExtension( dsi2, url, ie->second, ps.extensions );
            return dsi2;
          }
          catch( exception & e )
          {
            localMsg( "2. failed : " + string( e.what() ) );
            d->state = Error;
            io_error::keepExceptionPriority( e, excp, d->errorcode,
                                             d->errormsg );
            pds->at( dspos );
          }
          tried.insert( ie->second );
        }
      }
  }

  //// Pass 3 : still not found ? well, try EVERY format this time... ////////
  if( passbegin <= 3 && passend >= 3 )
  {
    localMsg( "not found yet... pass3..." );
    iext.first = ps.extensions.begin();
    iext.second = ps.extensions.end();
    for( ie=iext.first, ee=iext.second; ie!=ee; ++ie )
      if( tried.find( ie->second ) == notyet )
      {
        reader = formatInfo( ie->second );
        if( reader )
        {
          localMsg( "3. trying " + (*ie).second + "..." );
          try
          {
            d->state = Ok;
            DataSourceInfo dsi2 = reader->check( dsi, *this, options );
            setFileExtension( dsi2, url, ie->second, ps.extensions );
            return dsi2;
          }
          catch( exception & e )
          {
            localMsg( "3. failed : " + string( e.what() ) );
            d->state = Error;
            io_error::keepExceptionPriority( e, excp, d->errorcode,
                                            d->errormsg );
            pds->at( dspos );
          }
          tried.insert( ie->second );
        }
      }
  }

  //// End : still not succeeded, it's hopeless... ///////////////////////////
  localMsg( "not found at all, giving up" );
  d->state = Error;
  if( d->errorcode < 0 ) {
    d->errorcode = 0;
    d->errormsg = url + " : could not identify format";
  }
  
  launchException();
  abort();  // fix warning about control reaching end of non-void function
}

//============================================================================
//    R E A D I N G   M I N F
//============================================================================

SyntaxSet & DataSourceInfoLoader::minfSyntax()
{
  static SyntaxSet  pheader_syntax;
  if( pheader_syntax.empty() ) {
    // we don't absolutely need a syntax, but some pieces of code may 
    // expect it
    Syntax	&sx = pheader_syntax[ "__generic__" /*"PythonHeader"*/ ];

    sx[ "_borderWidth" ] = Semantic( "int", false, true );
    sx[ "sizeX"        ] = Semantic( "int", false, true );
    sx[ "sizeY"        ] = Semantic( "int", false, true );
    sx[ "sizeZ"        ] = Semantic( "int", false, true );
    sx[ "sizeT"        ] = Semantic( "int", false, true );

    sx[ "data_type" ] = Semantic( "string" );
    sx[ "object_type" ] = Semantic( "string" );
    sx[ "disk_data_type" ] = Semantic( "string", false, true );
    sx[ "possible_data_type" ] = Semantic( "string_vector" );
    sx[ "ascii" ] = Semantic( "int", false, true );
    sx[ "byte_swapping" ] = Semantic( "int", false, true );
    sx[ "spm_normalized" ] = Semantic( "bool" );
    sx[ "origin" ] = Semantic( "float_vector" );
    sx[ "spm_origin" ] = Semantic( "float_vector" );
    //sx[ "origin" ] = Semantic( DataTypeCode<vector<float> >::name() );
    sx[ "transfer_syntax" ] = Semantic( "string" );
    sx[ "manufacturer" ] = Semantic( "string" );
    sx[ "modality" ] = Semantic( "string" );
    sx[ "patient_id" ] = Semantic( "string" );
    sx[ "study_id" ] = Semantic( "string" );
    sx[ "series_number" ] = Semantic( "int" );
    sx[ "volume_dimension" ] = Semantic( "int_vector" );
    sx[ "voxel_size" ] = Semantic( "float_vector" );
    sx[ "filter_param" ] = Semantic( "float_vector" );
    sx[ "bits_allocated" ] = Semantic( "int" );
    sx[ "time_resolution" ] = Semantic( "float" );
    sx[ "te" ] = Semantic( "float" );
    sx[ "tr" ] = Semantic( "float" );
    sx[ "flip_angle" ] = Semantic( "float" );
    sx[ "slice_thickness" ] = Semantic( "float" );
    sx[ "inter_slices" ] = Semantic( "float" );
    sx[ "isotope_halflife" ] = Semantic( "float" );
    sx[ "category" ] = Semantic( "string" );
    sx[ "b_value" ] = Semantic( "float" );
    sx[ "scale_factor" ] = Semantic( "float" );
    sx[ "scale_offset" ] = Semantic( "float" );
    sx[ "nb_t_pos" ] = Semantic( "int" );
    sx[ "process_code" ] = Semantic( "int" );
    sx[ "filter_code" ] = Semantic( "int" );
    sx[ "polygon_dimension" ] = Semantic( "int" );
    sx[ "vertex_number" ] = Semantic( "int" );
    sx[ "start_time" ] = Semantic( "int_vector" );
    sx[ "duration_time" ] = Semantic( "int_vector" );
    sx[ "isotope" ] = Semantic( "string" );
    sx[ "image_unit" ] = Semantic( "string" );
    sx[ "radiopharmaceutical" ] = Semantic( "string" );
    sx[ "process_list" ] = Semantic( "string" );
    sx[ "zero_start_time" ] = Semantic( "S32" );
    sx[ "original_filename" ] = Semantic( "string" );
    sx[ "filter_type" ] = Semantic( "string" );
    sx[ "scale_factor_applied" ] = Semantic( "bool", false, true );
    sx[ "vox_units" ] = Semantic( "string" );
    sx[ "cal_units" ] = Semantic( "string" );
    sx[ "db_name" ] = Semantic( "string" );
    sx[ "aux_file" ] = Semantic( "string" );
    sx[ "generated" ] = Semantic( "string" );
    sx[ "scannum" ] = Semantic( "string" );
    sx[ "exp_date" ] = Semantic( "string" );
    sx[ "exp_time" ] = Semantic( "string" );
    sx[ "SPM_data_type" ] = Semantic( "string", false, true );
    sx[ "orient" ] = Semantic( "int" );
    sx[ "views" ] = Semantic( "int" );
    sx[ "start_field" ] = Semantic( "int" );
    sx[ "field_skip" ] = Semantic( "int" );
    sx[ "omax" ] = Semantic( "int" );
    sx[ "omin" ] = Semantic( "int" );
    sx[ "smax" ] = Semantic( "int" );
    sx[ "smin" ] = Semantic( "int" );
    sx[ "minimum" ] = Semantic( "int" );
    sx[ "maximum" ] = Semantic( "int" );
    sx[ "history" ] = Semantic( "string_vector" );
    sx[ "ecat_file_type" ] = Semantic( "string" );
    sx[ "ecat_system_type" ] = Semantic( "short" );
    sx[ "ecat_file_type" ] = Semantic( "short" );
    sx[ "ecat_start_scan_time" ] = Semantic( "unsigned int" );
    sx[ "ecat_acquisition_type" ] = Semantic( "" );
    sx[ "ecat_acquisition_mode" ] = Semantic( "short" );
    sx[ "ecat_original_file_name" ] = Semantic( "string" );
    sx[ "ecat_radiopharmaceutical" ] = Semantic( "string" );
    sx[ "ecat_plane_separation" ] = Semantic( "float" );
    sx[ "ecat_bin_size" ] = Semantic( "float" );
    sx[ "ecat_corrections_applied" ] = Semantic( "short" );
    sx[ "ecat_prompts" ] = Semantic( "int_vector" );
    sx[ "ecat_delayed" ] = Semantic( "int_vector" );
    sx[ "ecat_multiples" ] = Semantic( "int_vector" );
    sx[ "ecat_prompts" ] = Semantic( "int_vector" );
    sx[ "ecat_net_trues" ] = Semantic( "int_vector" );
    sx[ "ecat_tot_avg_uncor" ] = Semantic( "float_vector" );
    sx[ "ecat_tot_avg_cor" ] = Semantic( "float_vector" );
    sx[ "ecat_total_coin_rate" ] = Semantic( "int_vector" );
    sx[ "ecat_loss_correction_fctr" ] = Semantic( "float_vector" );
    sx[ "ecat_uncor_singles" ] = Semantic( "float_vector" );
    sx[ "ecat_start_time" ] = Semantic( "int_vector" );
    sx[ "ecat_duration_time" ] = Semantic( "int_vector" );
    sx[ "ecat_x_resolution" ] = Semantic( "float" );
    sx[ "storage_to_memory" ] = Semantic( "float_vector" );
    sx[ "referentials" ] = Semantic( "string_vector" );

    sx[ "textures" ] = Semantic( "vector of texture of FLOAT", false, true );
    sx[ "_texture_palettes" ] = Semantic( "map of int RcVolume of RGBA",
                                          false, true );

    // VECTOR OF VECTOR (MULTIRESOLUTION)
    sx[ "resolutions_dimension" ] = Semantic( "int_vector_vector " );

    pheader_syntax[ "int_vector" ][ "" ] = Semantic( "int" );
    pheader_syntax[ "float_vector" ][ "" ] = Semantic( "float" );
  }

  return pheader_syntax;
}


Object DataSourceInfoLoader::readMinf( DataSource & ds, Object base,
                                       Object options )
{
  Object  minf;
  if( base.get() )
    minf = base;
  else
    minf = Object::value( PropertySet() );

  string  filename = ds.url();
  if( filename.empty() )
    return minf;
  if( FileUtil::extension( filename ) != "minf" )
    filename += ".minf";

  Object  opts = Object::value( PropertySet() );
  if (options.get()){
    localMsg( "Read minf using options ..." );
    opts->copyProperties( options );
  }
  else {
    localMsg( "Read minf using default options ..." );
  }
  if( !opts->hasProperty( "syntaxset" ) )
    opts->setProperty( "syntaxset", minfSyntax() );

  try {
    // byte_swapping, object_type, data_type and item_type read from a minf
    // are properties that can not override those read from data header.
    // That is the reason why we store it, then read minf and finally restore
    // these properties.
    Reader<GenericObject>	rg( filename );
    rg.setOptions( opts );
    bool hasbs = minf->hasProperty( "byte_swapping" );
    bool hasot = minf->hasProperty( "object_type" );
    bool hasdt = minf->hasProperty( "data_type" );
    bool hasit = minf->hasProperty( "item_type" );
    int bs = 0;
    string ot = "";
    string dt = "";
    string it = "";
    
    // Store forbidden minf properties original values
    if( hasbs )
      hasbs = minf->getProperty( "byte_swapping", bs );
    
    if( hasot )
      hasot = minf->getProperty( "object_type", ot );
    
    if( hasdt )
      hasdt = minf->getProperty( "data_type", dt );
    
    if( hasit )
      hasit = minf->getProperty( "item_type", it );
    
    // Read minf
    rg.read( *minf );
    
    // Remove forbidden properties
    // and reset to original values
    if( hasbs )
      minf->setProperty( "byte_swapping", bs );
    
    else if( minf->hasProperty( "byte_swapping" ) )
      minf->removeProperty( "byte_swapping" );

    if( hasot )
      minf->setProperty( "object_type", ot );
    
    else if( minf->hasProperty( "object_type" ) )
      minf->removeProperty( "object_type" );
    
    if( hasdt )
      minf->setProperty( "data_type", dt );
    
    else if( minf->hasProperty( "data_type" ) )
      minf->removeProperty( "data_type" );
    
    if( hasit )
      minf->setProperty( "item_type", it );
    
    else if( minf->hasProperty( "item_type" ) )
      minf->removeProperty( "item_type" );
    
    return minf;
  } catch( exception & e ){
  } catch( ... ) {
    cerr << "Unknown exception" << endl;
  }

  return minf;

}

#undef localMsg
