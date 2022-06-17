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
#include <soma-io/checker/mincformatchecker.h>             // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/datasourcelist.h>
#include <soma-io/datasource/filedatasource.h>  // because we use file sources
#include <soma-io/reader/itemreader.h>                  // to read in the file
#include <soma-io/utilities/asciidatasourcetraits.h>     // to read datasource
#include <soma-io/writer/pythonwriter.h>
#include <soma-io/transformation/affinetransformation3d_base.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // header
#include <cartobase/object/property.h>                               // header
#include <cartobase/stream/fileutil.h>             // to manipulate file names
#include <cartobase/thread/mutex.h>
#include <cartobase/stream/fdinhibitor.h>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "MINCFORMATCHECKER" )
// localMsg must be undef at end of file

#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

//----------------------------------------------------------------------------
//MINC I/O support
#define OMPI_SKIP_MPICXX // avoid including mpi c++ stuff in an extern "C" context
extern "C"
{
#include <minc.h>
#include <volume_io.h>
}
#undef OMPI_SKIP_MPICXX

#ifndef VIO_FLOOR
// most recent type changes - is there a version anywhere ?
#define VIO_OK OK
typedef unsigned int aims_misize_t;
#else
typedef misize_t aims_misize_t;
#endif
#ifndef VIO_MAX_DIMENSIONS
// happens in some versions of minc...
#define VIO_MAX_DIMENSIONS MAX_DIMENSIONS
#endif

using namespace soma;
using namespace carto;
using namespace std;


//This function is used to make MINC functions behave silently
void MincFormatChecker::my_empty_print_error(char *message)
{
  /* do nothing. This is just to avoid the warning...*/
  message=NULL;
}


Mutex & MincFormatChecker::mincMutex()
{
  // Must be initialized (generally in main thread) before using concurrently
  static Mutex mutex( Mutex::Recursive );
  return mutex;
}


namespace
{
  inline float checkbounds( double x )
  {
    if( std::isnan(x) || std::isnan(float(x))
        || x == numeric_limits<double>::infinity()
        || x < -numeric_limits<float>::max()
        || x > numeric_limits<float>::max() )
      return 1.F;
    return float(x);
  }
}


//============================================================================
//   P R I V A T E   M E T H O D S
//============================================================================
/*** BUILDING DATASOURCELIST *************************************************
 * This method builds a DataSourceList from the initial datasource.
 * It tries to find .mnc and .minf files
 * If one or several of those files don't exist, it writes the initial
 * DataSource instead. Thus, the list returned contains at least one ds for
 * each of the following keywords : "mnc", "mnc.gz", "minf", "default", but
 * in the worst case they can all be the initial ds.
 ****************************************************************************/
void MincFormatChecker::_buildDSList( DataSourceList & dsl ) const
{
  DataSource* pds = dsl.dataSource().get();
  string mncname, minfname;

  mncname = minfname = FileUtil::uriFilename( pds->url() );
  if( mncname.empty() )
  {
    // we suppose ds is a mnc file
    dsl.addDataSource( "mnc", rc_ptr<DataSource>( pds ) );
  }
  else
  {
    string ext = FileUtil::extension( mncname );
    string basename = FileUtil::removeExtension( mncname );

    if( ext == "gz" )
    {
      ext = FileUtil::extension( basename ) + "." + ext;
      basename = FileUtil::removeExtension( basename );
    }
    
    if( ext == ".nii" )
      // refuse reading nifti: we handle it in our reader.
      throw wrong_format_error( pds->url() );

    if( ext.empty() )
    {
      basename = mncname;
      mncname += ".mnc";
    }
    minfname = mncname + ".minf";
    
    if( mncname == pds->url() )
    {
      // if mncname is original url
      dsl.addDataSource( "mnc", rc_ptr<DataSource>( pds ) );
    }
    else
    {
      if( FileUtil::fileStat( mncname ).find( '+' ) != string::npos ) {
        // if dim file exists
        dsl.addDataSource( "mnc", rc_ptr<DataSource>
            ( new FileDataSource( mncname ) ) );
      }
      else
      {
        // we suppose original url is dim
        dsl.addDataSource( "mnc", rc_ptr<DataSource>( pds ) );
      }
    }
  }
  localMsg( "mnc: " + dsl.dataSource( "mnc" )->url() );
  
  //// Minf DataSource
  if( FileUtil::fileStat( minfname ).find( '+' ) != string::npos )
  {
    // if minf file exist
    dsl.addDataSource( "minf", rc_ptr<DataSource>
        ( new FileDataSource( minfname ) ) );
  }
  else
  {
    // we suppose original url is minf
    dsl.addDataSource( "minf", rc_ptr<DataSource>( pds ) );
  }
  
  localMsg( "minf: " + dsl.dataSource( "minf" )->url() );
}


// Reads a MINC attribute and writes it as an AIMS attribute.
// A MINC attribute is always associated to a MINC variable.
//
// Parameters:
// sx : The AIMS syntax defining the types of the AIMS attributes.
// mincid : MINC file id.
// minc_var : Name of the MINC variable.
// minc_att : Name of the MINC attribute.
// aism_att_name : Name of the AIMS attribute.
//
// Return value:
//  0 : success
// -1 : no such variable
// -2 : no such attribute
// -3 : wrong attribute type
int MincFormatChecker::readMincAttribute( Object hdr, Syntax &sx, int mincid,
                                          string minc_var, string minc_att,
                                          string aims_att_name ) const
{
  //Get variable id from name
  int varid=ncvarid(mincid, minc_var.c_str());

  // If this variable does not exist
  if(varid==-1)
  {
    return(-1);
  }

  nc_type att_datatype;
  int att_length;
  // Get attribute id, type and length from name
  int attid = ncattinq( mincid, varid, minc_att.c_str(), &att_datatype,
                        &att_length );

  //If this attribute does not exist
  if(attid==-1)
  {
    return(-2);
  }

  // If this MINC attribute is a string
  if (att_datatype == NC_CHAR)
  {

    // If the corresponding AIMS attribute is not a string according to the syntax sx
    if(sx[aims_att_name].type!="string")
    {
      return(-3);
    }

    char *cdata;
    cdata = (char*)malloc((att_length+1)*sizeof(char));
    miattgetstr(mincid, varid, (char*)minc_att.c_str(), att_length+1, cdata);

    std::string str_cdata=cdata;
    hdr->setProperty( aims_att_name, str_cdata );

    free(cdata);
  }
  // The attribute is then of numerical type.
  else
  {
    double *ddata;
    ddata = (double*)malloc(att_length * sizeof(double));
    miattget(mincid, varid, (char*)minc_att.c_str(), NC_DOUBLE, att_length,
             ddata, NULL);

    // If the attribute length is 1, then it is stored as a single float
    if(att_length==1)
    {

      // If the corresponding AIMS attribute is not a float according to the syntax sx
      if(sx[aims_att_name].type!="float")
      {
        free(ddata);
        return(-3);
      }

      float float_ddata=ddata[0];
       hdr->setProperty( aims_att_name, float_ddata );
    }
    // The attribute is then a vector of floats.
    else
    {

      // If the corresponding AIMS attribute is not a vector of floats according to the syntax sx
      if(sx[aims_att_name].type!="float_vector")
      {
        free(ddata);
        return(-3);
      }
      vector<float> vec_ddata;

      for (int iiatt=0; iiatt<att_length; iiatt++)
      {
        vec_ddata.push_back(ddata[iiatt]);
      }

       hdr->setProperty( aims_att_name, vec_ddata );

    }
    free(ddata);
  }
  return(0);
}

// Read the MINC history and store it in the AIMS attribute "MINC_history".
// In the future, an history attribute might be added to AIMS and then it would be good to use it also to store the MINC history.
//
// Parameters:
// mincid : MINC file id.
//
// Return value:
//  0 : success
// -1 : no history attribute
// -3 : wrong attribute type

int MincFormatChecker::readMincHistory( Object hdr, const string & fname,
                                        int mincid ) const
{
  int varid=NC_GLOBAL;

  nc_type att_datatype;
  int att_length;
  int attid=ncattinq(mincid, varid, "history", &att_datatype, &att_length);

  // If there is no "history" attribute
  if(attid==-1)
  {
    return(-1);
  }
  std::string history;
  // If the "history" attribute is not of type "string"

  if (att_datatype == NC_CHAR)
  {
    char *cdata;
    cdata = (char*)malloc((att_length+1)*sizeof(char));
    miattgetstr(mincid, varid, const_cast<char*>( "history" ), att_length+1, cdata);

    history=cdata;
    free(cdata);
  }
  else
  {
    return(-3);
  }

  // Add a new line to history specifying the date and AIMS conversion
  time_t tod;
  tod = time(NULL);
  char *char_time = ctime(&tod);

  char_time[strlen(char_time)-1]='\0';

  history = history + char_time + ">>> AIMS read MINC file (" + fname + ")\n";

   hdr->setProperty("MINC_history",history);
  return(0);

}

/* Here we duplicate a bit of libminc, because these functions are static and
   we need to call them manually: in libminc, with ignore_offsets is set to
   TRUE (hard-coded) thus translation information is dropped and
   transformations are wrong for mgz/mgh formats. */

#define MGH_MAX_DIMS 4          /* Maximum number of dimensions */
#define MGH_N_SPATIAL VIO_N_DIMENSIONS /* Number of spatial dimensions */
#define MGH_N_COMPONENTS 4      /* Number of transform components. */
#define MGH_N_XFORM (MGH_N_COMPONENTS * MGH_N_SPATIAL)

#define MGH_HEADER_SIZE 284 /* Total number of bytes in the header. */
#define MGH_EXTRA_SIZE 194   /* Number of "unused" bytes in the header. */

// WARNING: dependency over niftilib for znzlib, the lib is in the nifti
// plugin code
#include <soma-io/nifticlib/znzlib/znzlib.h>
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <Winsock2.h>
#else
#  include <arpa/inet.h> /* for ntohl and ntohs */
#endif

extern "C" {

struct mgh_header {
  int version;                  /**< Must be 0x00000001. */
  int sizes[MGH_MAX_DIMS];      /**< Dimension sizes, fastest-varying FIRST. */
  int type;                     /**< One of the MGH_TYPE_xxx values. */
  int dof;                      /**< Degrees of freedom, if used. */
  short goodRASflag;            /**< True if spacing and dircos valid. */
  float spacing[MGH_N_SPATIAL]; /**< Dimension spacing.  */
  float dircos[MGH_N_COMPONENTS][MGH_N_SPATIAL]; /**< Dimension transform. */
};

/**
 * Trivial function to swap floats if necessary.
 * \param f The big-endian 4-byte float value.
 * \return The float value in host byte order.
 */
static float
swapFloat(float f)
{
  union {
    float f;
    int i;
  } sl;

  sl.f = f;

  sl.i = ntohl(sl.i);

  return sl.f;
}

/**
 * Read an MGH header from an open file stream.
 * \param fp The open file (may be a pipe).
 * \param hdr_ptr A pointer to the header structure to fill in.
 * \returns TRUE if successful.
 */
static VIO_BOOL
mgh_header_from_file(znzFile fp, struct mgh_header *hdr_ptr)
{
  int i, j;
  char dummy[MGH_HEADER_SIZE];

  /* Read in the header. We do this piecemeal because the mgh_header
   * struct will not be properly aligned on most systems, so a single
   * fread() WILL NOT WORK.
   */
  if (znzread(&hdr_ptr->version, sizeof(int), 1, fp) != 1 ||
      znzread(hdr_ptr->sizes, sizeof(int), MGH_MAX_DIMS, fp) != MGH_MAX_DIMS ||
      znzread(&hdr_ptr->type, sizeof(int), 1, fp) != 1 ||
      znzread(&hdr_ptr->dof, sizeof(int), 1, fp) != 1 ||
      znzread(&hdr_ptr->goodRASflag, sizeof(short), 1, fp) != 1 ||
      /* The rest of the fields are optional, but we can safely read them
       * now and check goodRASflag later to see if we should really trust
       * them.
       */
      znzread(hdr_ptr->spacing, sizeof(float), MGH_N_SPATIAL, fp) != MGH_N_SPATIAL ||
      znzread(hdr_ptr->dircos, sizeof(float), MGH_N_XFORM, fp) != MGH_N_XFORM ||
      znzread(dummy, 1, MGH_EXTRA_SIZE, fp) != MGH_EXTRA_SIZE)
  {
    print_error((char *) "Problem reading MGH file header.");
    return FALSE;
  }

  /* Successfully read all of the data. Now we have to convert it to the
   * machine's byte ordering.
   */
  hdr_ptr->version = ntohl(hdr_ptr->version);
  for (i = 0; i < MGH_MAX_DIMS; i++)
  {
    hdr_ptr->sizes[i] = ntohl(hdr_ptr->sizes[i]);
  }
  hdr_ptr->type = ntohl(hdr_ptr->type);
  hdr_ptr->dof = ntohl(hdr_ptr->dof);
  hdr_ptr->goodRASflag = ntohs(hdr_ptr->goodRASflag);

  if (hdr_ptr->version != 1)
  {
    print_error((char *) "Must be MGH version 1.\n");
    return FALSE;
  }

  if (hdr_ptr->goodRASflag)
  {
    for (i = 0; i < MGH_N_SPATIAL; i++)
    {
      hdr_ptr->spacing[i] = swapFloat(hdr_ptr->spacing[i]);
      for (j = 0; j < MGH_N_COMPONENTS; j++)
      {
        hdr_ptr->dircos[j][i] = swapFloat(hdr_ptr->dircos[j][i]);
      }
    }
  }
  else
  {
    /* Flag is zero, so just use the defaults.
     */
    for (i = 0; i < MGH_N_SPATIAL; i++)
    {
      /* Default spacing is 1.0.
       */
      hdr_ptr->spacing[i] = 1.0;

      /* Assume coronal orientation.
       */
      for (j = 0; j < MGH_N_COMPONENTS; j++)
      {
        hdr_ptr->dircos[j][i] = 0.0;
      }
      hdr_ptr->dircos[0][0] = -1.0;
      hdr_ptr->dircos[1][2] = -1.0;
      hdr_ptr->dircos[2][1] = 1.0;
    }
  }
  return TRUE;
}

}


/*** BUILDING HEADER *********************************************************
 * This method builds a header from a .mnc DataSource.
 * The argument is given by check(...) and is supposed to be a .dim file.
 * However, since the DSList is constructed even if no .dim file is found,
 * it can be an absolutely different format. In this case, the method should 
 * launch an exception at one point which should be caught by DSILoader, 
 * thus knowing this checker cannot read the entry file.
 ****************************************************************************/
Object MincFormatChecker::_buildHeader( DataSource* hds ) const
{
//   FileDataSource* fds = dynamic_cast<FileDataSource *>( hds );
  string          fname = hds->url();
  
//   if( !hds->isOpen() )
//     hds->open( DataSource::Read );
//   if( !hds->isOpen() )
//     io_error::launchErrnoExcept( fname );

  // cout << "MincHeader::read(), file : " << _name << endl;

  // Work around BUG in netCDF which incorrectly uses assert and
  // aborts on empty files
  struct stat   st;
  stat( fname.c_str(), &st ); // succeeds because it has already been done
  if( st.st_size == 0 )
    throw eof_error( fname );

  // MINC does not support windows filenames, so we replace it using linux
  // separators
  fname = FileUtil::linuxFilename( fname );

  VIO_Volume volume;
  vector<VIO_STR> dim_names( VIO_MAX_DIMENSIONS );
  VIO_STR fileName = create_string ( const_cast<char *>( fname.c_str() ) );

  dim_names[0] = create_string( const_cast<char *>( MIxspace ) );
  dim_names[1] = create_string( const_cast<char *>( MIyspace ) );
  dim_names[2] = create_string( const_cast<char *>( MIzspace ) );
  dim_names[3] = create_string( const_cast<char *>( MItime ) );
  for( unsigned i=4; i<VIO_MAX_DIMENSIONS; ++i )
    dim_names[i] = 0;

  volume_input_struct input_info;
  //Read the header of the MINC file. This volume is not allocated and its content is not read from disk.
  set_print_error_function(my_empty_print_error);

  mincMutex().lock();
  // avoid printing anything from Minc/NetCDF lib
  milog_init( CARTOBASE_STREAM_NULLDEVICE );
  milog_set_verbosity(0);
  int   status = 0;

  Object         hdr = Object::value( PropertySet() );  // header

  try
  {
    // suppress stderr output to avoid netcdf messages
    fdinhibitor fdi( 2 );
    fdi.close();

    status = start_volume_input( fileName, 0, &dim_names[0],
                                MI_ORIGINAL_TYPE, TRUE,
                                0.0, 0.0, TRUE, &volume,
                                (minc_input_options *) NULL, &input_info );
    milog_init("stderr");

    if(status != VIO_OK)
      throw wrong_format_error( fileName );

#ifdef MINC_NIFTI_SUPPORT
    if( input_info.file_format == NII_FORMAT )
      // refuse reading nifti: we handle it in our reader.
      throw wrong_format_error( fileName );
#endif

    string _type;
    if(volume->nc_data_type==NC_BYTE && volume->signed_flag==FALSE)
    {
      _type = "U8";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_BYTE && volume->signed_flag==TRUE)
    {
      _type = "S8";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_SHORT && volume->signed_flag==FALSE)
    {
      _type = "U16";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_SHORT && volume->signed_flag==TRUE)
    {
      _type = "S16";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_LONG && volume->signed_flag==FALSE)
    {
      _type = "U32";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_LONG && volume->signed_flag==TRUE)
    {
      _type = "S32";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_FLOAT)
    {
      _type = "FLOAT";
      //std::cout << _type << "\n";
    }
    else if(volume->nc_data_type==NC_DOUBLE)
    {
      _type = "DOUBLE";
      //std::cout << _type << "\n";
    }
    else
      throw wrong_format_error( fileName );

    //Read the volume size
    int n_dimensions = get_volume_n_dimensions(volume);
    int       sizes[VIO_MAX_DIMENSIONS];
    vector<int> dims( n_dimensions, 1 );
    vector<float> vs( n_dimensions, 1. );
    get_volume_sizes( volume, sizes );
    if( n_dimensions >= 4 )
      dims[3] = sizes[3];
    if( n_dimensions >= 3 )
      dims[2] = sizes[2];
    if( n_dimensions >= 2 )
      dims[1] = sizes[1];
    if( n_dimensions >= 1 )
      dims[0] = sizes[0];

    vector< vector<float> > transs;
    vector<string> refs;
    AffineTransformation3dBase s2m;

    //Read voxel size
    //In MINC, voxel size can be positive or negative. Here we take the absolute value of the voxel size and the case of negative increment steps (negative voxel sizes) is treated when the volume is read (in MincReader).
    vs[3] = fabs(volume->separations[3]);
    bool is_buggy_mgh = false;
    if( fname.substr( fname.length() - 4, 4 ) == ".mgz"
        || fname.substr( fname.length() - 4, 4 ) == ".mgh" )
    {
      // freesurfer mgz / mgh files seem to be in a different order
      // (is this a bug in minc io or in our interpretation ?)
      is_buggy_mgh = true;

      struct mgh_header hdr;
      znzFile           fp;
      VIO_General_transform *mnc_native_xform
        = get_voxel_to_world_transform(volume);

      // we must re-open and re-read the file header because some info
      // (offsets in transform) has been dropped
      if ((fp = znzopen(fileName, "rb", 1)) == NULL)
      {
        cerr << "Unable to open file\n";
      }

      mgh_header_from_file( fp, &hdr );
      znzclose(fp);

      AffineTransformation3dBase f2sb;
      for( int i=0; i<3; ++i )
      {
        for( int j=0; j<3; ++j )
        {
          f2sb.matrix()(i, j) = hdr.dircos[j][i] * hdr.spacing[j];
        }
        f2sb.matrix()(i, 3) = hdr.dircos[3][i];
      }
      // cout << "f2sb:\n" << f2sb << endl;
      Point3df p = f2sb.transform(hdr.sizes[0] / 2.f, hdr.sizes[1] / 2.f, hdr.sizes[2] / 2.f ) - f2sb.transform(0.f, 0.f, 0.f);
      f2sb.matrix()(0, 3) -= p[0];
      f2sb.matrix()(1, 3) -= p[1];
      f2sb.matrix()(2, 3) -= p[2];
      // cout << "f2sb:\n" << f2sb << endl;
      // f2sb is the matrix exactly in the nifti converted by mri_convert,
      // with the same voxel order.

      // we have to combine storage_to_memory and vs
      // BUT the storage_to_memory calculation from minc (later) is wrong here.
      // we do it from mgh data instead.
      for( int i=0; i<3; ++i )
        for( int j=0; j<3; ++j )
        {
          float m = hdr.dircos[j][i];
          if( fabs(m) < 0.5 )
            m = 0.f;
          else
            m = -(int)(m / fabs(m));
          s2m.matrix()( i, j ) = m;
          if( m < 0 )
            s2m.matrix()(i, 3) += hdr.sizes[j] - 1;
        }
      // now we can transform the voxel sizes
      p = s2m.transform( hdr.spacing[0], hdr.spacing[1], hdr.spacing[2] )
        - s2m.transform( 0.f, 0.f, 0.f );

      vs[0] = fabs(p[0]);
      vs[1] = fabs(p[1]);
      vs[2] = fabs(p[2]);

      // mem to scanner-based
      AffineTransformation3dBase m2sb;
      // get back to mm (vox) -> mm (world)
      AffineTransformation3dBase vst;
      vst.matrix()(0, 0) = 1. / vs[0];
      vst.matrix()(1, 1) = 1. / vs[1];
      vst.matrix()(2, 2) = 1. / vs[2];
      m2sb = f2sb * s2m.inverse() * vst;
      // fill in directly transforms because s2m is unreliable here.
      refs.push_back( "Scanner-based anatomical coordinates" );
      transs.push_back( m2sb.toVector() );
    }
    else
    {
      vs[2] = fabs(volume->separations[2]);
      vs[1] = fabs(volume->separations[1]);
      vs[0] = fabs(volume->separations[0]);
    }

    if(n_dimensions==5)
    {
      throw wrong_format_error( fileName );
    }
    if(n_dimensions<=3)
    {
      dims[3] = 1;
      vs[3] = 1.0;
      if(n_dimensions<=2)
      {
        dims[2] = 1;
        vs[2] = 1.0;
        if(n_dimensions==1)
        {
          dims[1] = 1;
          vs[1] = 1.0;
        }
      }
    }

    //Read voxel range and real range. Only for debugging purpose: the values are not stored in the class.
    VIO_Real vmin,vmax,rmin,rmax;
    get_volume_voxel_range(volume,&vmin,&vmax);
    //std::cout << "Voxel range : " << vmin << ";" <<vmax<<"\n";
    get_volume_real_range(volume,&rmin,&rmax);
    // std::cout << "Real range : " << rmin << ";" <<rmax<<"\n";

    vector<string> types;
    types.push_back( "U8" );
    types.push_back( "S8" );
    types.push_back( "U16" );
    types.push_back( "S16" );
    types.push_back( "U32" );
    types.push_back( "S32" );
    types.push_back( "FLOAT" );
    types.push_back( "DOUBLE" );

    if( ( rmin != vmin || rmax != vmax ) && _type != "DOUBLE" )
    {
      // voxel and pixel ranges differ: read as float preferably
      types.erase( find( types.begin(), types.end(), "FLOAT" ) );
      types.erase( find( types.begin(), types.end(), _type ) );
      types.insert( types.begin(), "FLOAT" );
      types.insert( types.begin() + 1, _type );
      _type = "FLOAT";
    }
    else
    {
      types.erase( find( types.begin(), types.end(), _type ) );
      types.insert( types.begin(), _type );
    }

    string ext = FileUtil::extension( fname );
    if( ext == "mgh" || ext == "mgz" )
      // freesurfer ext: advertise a different format
      hdr->setProperty( "format", string( "FREESURFER (MINC)" ) );
    else
      hdr->setProperty( "format", string( "MINC" ) );
    hdr->setProperty( "volume_dimension", dims );
    hdr->setProperty( "voxel_size", vs );

    /* WARNING:
       I don't understand why axes 0 and 2 are flipped, here and in
       s2m later, when we are using "direct" order in dims and vs.
     */
    vector<float> minc_vs;
    minc_vs.push_back( checkbounds( volume->separations[0] ) );
    minc_vs.push_back( checkbounds( volume->separations[1] ) );
    minc_vs.push_back( checkbounds( volume->separations[2] ) );
    minc_vs.push_back( checkbounds( volume->separations[3] ) );
    hdr->setProperty( "MINC_voxel_size", minc_vs );

    hdr->setProperty( "data_type", _type );
    hdr->setProperty( "object_type", string( "Volume" ) );

    //OC : Meta-info: should we add something to read the rest of the minc header ?

    //1) Voxel to world transform
    //2) Space name
    //3) All other attributes


    //1) Voxel to world transform
    //Handle positive/negative voxel size.
    if( !is_buggy_mgh )
    {
      s2m = AffineTransformation3dBase();
      s2m.matrix()( 0, 0 )
          = -(int)(volume->separations[2]/fabs(volume->separations[2]));
      s2m.matrix()( 1, 1 )
          = -(int)(volume->separations[1]/fabs(volume->separations[1]));
      s2m.matrix()( 2, 2 )
          = -(int)(volume->separations[0]/fabs(volume->separations[0]));

      if( s2m.matrix()( 0, 0 ) < 0 )
        s2m.matrix()( 0, 3 ) = dims[0]-1;
      if( s2m.matrix()( 1, 1 ) < 0 )
        s2m.matrix()( 1, 3 ) = dims[1]-1;
      if( s2m.matrix()( 2, 2 ) < 0 )
        s2m.matrix()( 2, 3 ) = dims[2]-1;
    }
    hdr->setProperty( "storage_to_memory", s2m.toVector() );

    VIO_General_transform *gt=get_voxel_to_world_transform(volume);

    if(gt->type!=LINEAR)
    {
      string transfo_type="NOT SUPPORTED !!";
      hdr->setProperty ( "MINC_general_transform:type", transfo_type );
      //std::cout << "MINC Non-linear voxel to world transform are not yet supported !!\n";
    }
    else
    {
      string transfo_type="linear";
      hdr->setProperty ( "MINC_general_transform:type", transfo_type );
      //std::cout << gt->inverse_flag << "\n";

      //std::cout   << "Writing MINC_general_transform:inverse_flag\n";
      int inv_flag=gt->inverse_flag;
      hdr->setProperty ( "MINC_general_transform:inverse_flag",  inv_flag );

      vector<float> transfo( 16 );
      if( gt->inverse_flag==FALSE )
      {
        for( int i=0;i<4;i++ )
        {
          for( int j=0; j<4; j++ )
          {
            transfo[i*4+j] = Transform_elem( *(gt->linear_transform), i, j );
          }
        }
      }
      else
      {
        for( int i=0; i<4; i++ )
        {
          for( int j=0; j<4; j++ )
          {
            transfo[i*4+j]
                = Transform_elem( *(gt->inverse_linear_transform), i, j );
          }
        }
      }
      AffineTransformation3dBase tr( transfo );
      if( !is_buggy_mgh )  // freesurfer trans are processed differently
      {
        tr *= s2m.inverse();
        AffineTransformation3dBase vsmi;
        vsmi.matrix()(0,0) = 1./vs[0];
        vsmi.matrix()(1,1) = 1./vs[1];
        vsmi.matrix()(2,2) = 1./vs[2];
        tr *= vsmi;
        transs.push_back( tr.toVector() );
        // check to recognize MNI template referential
        if( dims[0] * vs[0] == 182 && dims[1] * vs[1] == 218
            && dims[2] * vs[2] == 182
            && tr.transform( Point3df( 0, 0, 0 ) ) == Point3df( 90, 90, 108 ) )
          refs.push_back( "Talairach-MNI template-SPM" );
        else
          refs.push_back( "Scanner-based anatomical coordinates" );
      }
      hdr->setProperty( "transformations", transs );
      hdr->setProperty( "referentials", refs );
    }

    //2) Space name
    std::string space_name=get_volume_space_type(volume);
    hdr->setProperty( "MINC_space_type", space_name);

    //3) Other attributes

    // std::cout << "MincHeader::read() 4\n";
    if( input_info.file_format == MNC_FORMAT
#ifdef MINC_MNC2_SUPPORT
        || input_info.file_format == MNC2_FORMAT
#endif
      )
    {
      // only allowed for "real" minc
      Minc_file minc_file = get_volume_input_minc_file(&input_info);
      if( !minc_file )
        throw wrong_format_error( fileName );

      int mincid = get_minc_file_id(minc_file );

      //ncopts=NC_VERBOSE;
      ncopts=0;

      SyntaxSet &s = DataSourceInfoLoader::minfSyntax();
      Syntax &sx = s[ "__generic__" /*"PythonHeader"*/ ];

      readMincAttribute( hdr, sx, mincid, "patient", "varid", "MINC_patient:varid");
      readMincAttribute( hdr, sx, mincid, "patient", "vartype", "MINC_patient:vartype");
      readMincAttribute( hdr, sx, mincid, "patient", "version", "MINC_patient:version");
      readMincAttribute( hdr, sx, mincid, "patient", "full_name", "patient_id");
      readMincAttribute( hdr, sx, mincid, "patient", "identification", "MINC_patient:identification");
      readMincAttribute( hdr, sx, mincid, "patient", "birthdate", "MINC_patient:birthdate");
      readMincAttribute( hdr, sx, mincid, "patient", "sex", "MINC_patient:sex");
      readMincAttribute( hdr, sx, mincid, "patient", "weight", "MINC_patient:weight");

      readMincAttribute( hdr, sx, mincid, "study", "varid", "MINC_study:varid");
      readMincAttribute( hdr, sx, mincid, "study", "vartype", "MINC_study:vartype");
      readMincAttribute( hdr, sx, mincid, "study", "version", "MINC_study:version");
      readMincAttribute( hdr, sx, mincid, "study", "start_time", "MINC_study:start_time");
      readMincAttribute( hdr, sx, mincid, "study", "modality", "modality");
      readMincAttribute( hdr, sx, mincid, "study", "institution", "MINC_study:institution");
      readMincAttribute( hdr, sx, mincid, "study", "station_id", "MINC_study:station_id");
      readMincAttribute( hdr, sx, mincid, "study", "procedure", "MINC_study:procedure");
      readMincAttribute( hdr, sx, mincid, "study", "study_id", "study_id");
      readMincAttribute( hdr, sx, mincid, "study", "acquisition_id", "MINC_study:acquisition_id");

      readMincAttribute( hdr, sx, mincid, "acquisition", "varid", "MINC_acquisition:varid");
      readMincAttribute( hdr, sx, mincid, "acquisition", "vartype", "MINC_acquisition:vartype");
      readMincAttribute( hdr, sx, mincid, "acquisition", "version", "MINC_acquisition:version");
      readMincAttribute( hdr, sx, mincid, "acquisition", "scanning_sequence", "MINC_acquisition:scanning_sequence");
      readMincAttribute( hdr, sx, mincid, "acquisition", "repetition_time", "tr");
      readMincAttribute( hdr, sx, mincid, "acquisition", "echo_time", "te");
      readMincAttribute( hdr, sx, mincid, "acquisition", "inversion_time", "MINC_acquisition:inversion_time");
      readMincAttribute( hdr, sx, mincid, "acquisition", "flip_angle", "flip_angle");
      readMincAttribute( hdr, sx, mincid, "acquisition", "num_averages", "MINC_acquisition:num_averages");
      readMincAttribute( hdr, sx, mincid, "acquisition", "imaging_frequency", "MINC_acquisition:imaging_frequency");
      readMincAttribute( hdr, sx, mincid, "acquisition", "imaged_nucleus", "MINC_acquisition:imaged_nucleus");
      readMincAttribute( hdr, sx, mincid, "acquisition", "comments", "MINC_acquisition:comments");

      readMincAttribute( hdr, sx, mincid, "image-min", "units", "MINC_image-min:units");
      readMincAttribute( hdr, sx, mincid, "image-max", "units", "MINC_image-max:units");


      readMincAttribute( hdr, sx, mincid, "patient", "age", "MINC_patient:age");

      readMincAttribute( hdr, sx, mincid, "study", "start_date", "MINC_study:start_date");
      readMincAttribute( hdr, sx, mincid, "study", "manufacturer", "MINC_study:manufacturer");
      readMincAttribute( hdr, sx, mincid, "study", "model", "MINC_study:model");
      readMincAttribute( hdr, sx, mincid, "study", "field_value", "MINC_study:field_value");
      readMincAttribute( hdr, sx, mincid, "study", "software_version", "MINC_study:software_version");
      readMincAttribute( hdr, sx, mincid, "study", "serial_no", "MINC_study:serial_no");
      readMincAttribute( hdr, sx, mincid, "study", "performing_physician", "MINC_study:performing_physician");
      readMincAttribute( hdr, sx, mincid, "study", "operator", "MINC_study:operator");
      readMincAttribute( hdr, sx, mincid, "study", "calibration_date", "MINC_study:calibration_date");

      readMincAttribute( hdr, sx, mincid, "acquisition", "acquisition_id", "MINC_acquisition:acquisition_id");
      readMincAttribute( hdr, sx, mincid, "acquisition", "start_time", "MINC_acquisition:start_time");
      readMincAttribute( hdr, sx, mincid, "acquisition", "protocol_name", "MINC_acquisition:protocol_name");
      readMincAttribute( hdr, sx, mincid, "acquisition", "receive_coil", "MINC_acquisition:receive_coil");
      readMincAttribute( hdr, sx, mincid, "acquisition", "transmit_coil", "MINC_acquisition:transmit_coil");
      readMincAttribute( hdr, sx, mincid, "acquisition", "echo_number", "MINC_acquisition:echo_number");
      readMincAttribute( hdr, sx, mincid, "acquisition", "slice_thickness", "MINC_acquisition:slice_thickness");
      readMincAttribute( hdr, sx, mincid, "acquisition", "num_slices", "MINC_acquisition:num_slices");
      readMincAttribute( hdr, sx, mincid, "acquisition", "num_dyn_scans", "MINC_acquisition:num_dyn_scans");
      readMincAttribute( hdr, sx, mincid, "acquisition", "window_center", "MINC_acquisition:window_center");
      readMincAttribute( hdr, sx, mincid, "acquisition", "window_width", "MINC_acquisition:window_width");
      readMincAttribute( hdr, sx, mincid, "acquisition", "num_phase_enc_steps", "MINC_acquisition:num_phase_enc_steps");
      readMincAttribute( hdr, sx, mincid, "acquisition", "percent_sampling", "MINC_acquisition:percent_sampling");
      readMincAttribute( hdr, sx, mincid, "acquisition", "percent_phase_fov", "MINC_acquisition:percent_phase_fov");
      readMincAttribute( hdr, sx, mincid, "acquisition", "pixel_bandwidth", "MINC_acquisition:pixel_bandwidth");
      readMincAttribute( hdr, sx, mincid, "acquisition", "phase_enc_dir", "MINC_acquisition:phase_enc_dir");
      readMincAttribute( hdr, sx, mincid, "acquisition", "SAR", "MINC_acquisition:SAR");
      readMincAttribute( hdr, sx, mincid, "acquisition", "mr_acq_type", "MINC_acquisition:mr_acq_type");
      readMincAttribute( hdr, sx, mincid, "acquisition", "image_type", "MINC_acquisition:image_type");
      readMincAttribute( hdr, sx, mincid, "acquisition", "MrProt_dump", "MINC_acquisition:MrProt_dump");

      readMincAttribute( hdr, sx, mincid, "processing", "transformation0-filename", "MINC_processing:transformation0-filename");
      readMincAttribute( hdr, sx, mincid, "processing", "transformation0-filedata", "MINC_processing:transformation0-filedata");

      readMincHistory( hdr, fname, mincid );
    }

    //Free memory
    delete_volume(volume);
    delete_string(fileName);
    delete_string(dim_names[0]);
    delete_string(dim_names[1]);
    delete_string(dim_names[2]);
    delete_string(dim_names[3]);
    delete_general_transform(gt);
    delete_volume_input(&input_info);
  }
  catch( ... )
  {
    milog_init("stderr");
    mincMutex().unlock();
    throw;
  }
  // std::cout << "MincHeader::read() done\n";

  mincMutex().unlock();

  return hdr;
}

//============================================================================
//   P U B L I C   M E T H O D S
//============================================================================

MincFormatChecker::~MincFormatChecker()
{
}

DataSourceInfo MincFormatChecker::check( DataSourceInfo dsi,
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
        || ( dsi.header()->getProperty( "format" )->getString() != "MINC"
             && dsi.header()->getProperty( "format" )->getString()
                != "FREESURFER-MINC" ) )
      throw wrong_format_error( "Not a MINC header",
                                dsi.list().dataSource()->url() );

  //--- build datasourcelist -------------------------------------------------
  if( dolist )
  {
    localMsg( "Building list..." );
    _buildDSList( dsi.list() );
  }
  //--- build header ---------------------------------------------------------
  if( doread )
  {
    localMsg( "Reading header..." );
    DataSource* hds = dsi.list().dataSource( "mnc" ).get();
    dsi.header() = _buildHeader( hds );
    std::string format = dsi.header()->getProperty( "format" )->getString();

    localMsg( "Reading minf..." );
    DataSource* minfds = dsi.list().dataSource( "minf" ).get();
    DataSourceInfoLoader::readMinf( *minfds, dsi.header(), options );
    // set back format
    dsi.header()->setProperty( "format", format );

    // Fixes wrong dimension properties
    vector<int> dims( 4, 1 );
    if( dsi.header()->getProperty( "volume_dimension", dims ) )
    {
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
  }

  //--- write capabilities ---------------------------------------------------
  if( docapa )
  {
    localMsg ("Writing capabilities..." );
    dsi.capabilities().setMemoryMapping( false );
    try
    {
      if( !(bool) dsi.header()->getProperty( "byte_swapping" )->getScalar() 
          && !(bool) dsi.header()->getProperty( "ascii" )->getScalar() )
        dsi.capabilities().setMemoryMapping( true );
    }
    catch( ... )
    {}
    dsi.capabilities().setDataSource( dsi.list().dataSource( "mnc" ) );
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
