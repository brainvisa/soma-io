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
#include <soma-io/checker/niftiformatchecker.h> // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/datasourcelist.h>
#include <soma-io/datasource/filedatasource.h>  // because we use file sources
#include <soma-io/nifticlib/niftilib/nifti2_io.h>
#include <soma-io/checker/niftistructwrapper.h>
#include <soma-io/checker/transformation.h>
#include <soma-io/nifticlib/niftiapihelpers_p.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // header
#include <cartobase/object/property.h>                               // header
#include <cartobase/stream/fileutil.h>             // to manipulate file names
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
//--- system -----------------------------------------------------------------
#include <stdio.h>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii -> used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "NIFTI1FORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   U T I L I T I E S
//============================================================================
string StandardReferentials::mniTemplateReferential()
{
  return "Talairach-MNI template-SPM";
}


string StandardReferentials::acPcReferential()
{
  return "Talairach-AC/PC-Anatomist";
}


string StandardReferentials::talairachReferential()
{
  return "Talairach";
}


string StandardReferentials::commonScannerBasedReferential()
{
  return "Scanner-based anatomical coordinates";
}


string StandardReferentials::mniTemplateReferentialID()
{
  return "803552a6-ac4d-491d-99f5-b938392b674b";
}


string StandardReferentials::acPcReferentialID()
{
  return "a2a820ac-a686-461e-bcf8-856400740a6c";
}


string StandardReferentials::commonScannerBasedReferentialID()
{
  return "a397f441-a525-298a-c8f9-26011047eadf";
}

namespace soma
{

  string NiftiReferential( int xform_code )
  {
    switch( xform_code )
    {
    case NIFTI_XFORM_UNKNOWN:
      // No transformation is provided, this should never be returned.
      return "Arbitrary coordinates";
      break;
    case NIFTI_XFORM_SCANNER_ANAT:
      return "Scanner-based anatomical coordinates";
      break;
    case NIFTI_XFORM_ALIGNED_ANAT:
      return "Coordinates aligned to another file or to anatomical truth";
      break;
    case NIFTI_XFORM_TALAIRACH:
      return StandardReferentials::talairachReferential();
      break;
    case NIFTI_XFORM_MNI_152:
      return StandardReferentials::mniTemplateReferential();
      break;
    default:
      // A transformation is provided but its target referential is unknown.
      return "Unknown transformation";
      break;
    }
  }

  int NiftiReferential( const std::string&  ref )
  {
    if( ref == "Scanner-based anatomical coordinates" )
      return NIFTI_XFORM_SCANNER_ANAT;
    else if( ref 
        == "Coordinates aligned to another file or to anatomical truth" )
      return NIFTI_XFORM_ALIGNED_ANAT;
    else if( ref == StandardReferentials::acPcReferential() )
      return NIFTI_XFORM_TALAIRACH;
    else if( ref == StandardReferentials::mniTemplateReferential() )
      return NIFTI_XFORM_MNI_152;
    else
      return NIFTI_XFORM_UNKNOWN;
  }
}


//============================================================================
//   P R I V A T E   M E T H O D S
//============================================================================
/*** BUILDING DATASOURCELIST *************************************************
 * This method builds a DataSourceList from the initial datasource.
 * It tries to find .nii, .nii.gz, .img, .hdr and .minf files
 * If one or several of those files doesn't exist, it writes the initial
 * DataSource instead. Thus, the list returned contains at least one ds for
 * each of the following keywords : "nii", "hdr", "minf", "default", but
 * in the worst case they can all be the initial ds.
 ****************************************************************************/
void NiftiFormatChecker::_buildDSList( DataSourceList & dsl ) const
{
  DataSource* pds = dsl.dataSource().get();
  string hdrname, dataname, minfname, basename;
  // dataname is the name of the main data file (.nii, .nii.gz or .img)

  dataname = FileUtil::uriFilename( pds->url() );
  if( dataname.empty() )
  {
    // we suppose ds is a nii file
    dsl.addDataSource( "nii", rc_ptr<DataSource>( pds ) );
    basename = FileUtil::removeExtension( pds->url() );
  }
  else
  {
    string ext = FileUtil::extension( dataname );
    basename = FileUtil::removeExtension( dataname );

    if( ext == "nii" )
    {
      // all OK
    }
    else if( ext == "hdr" )
    {
      dataname = basename + ".img";
      hdrname = basename + ".hdr";
    }
    else if( ext == "gz" )
    {
      string ext2 = FileUtil::extension( basename );
      if( ext2 == "nii" )
      {
        ext = "nii.gz";
        basename = FileUtil::removeExtension( basename );
        dataname = basename + ".nii.gz";
      }
      else if( ext2 == "img" )
      {
        ext = "img.gz";
        basename = FileUtil::removeExtension( basename );
        dataname = basename + ".img.gz";
      }
      else
      {
        // probably a problem... but we'll try like this
        ext.clear();
      }
    }
    else if( ext == "img" )
    {
      dataname = basename + ".img";
      hdrname = basename + ".hdr";
    }
    else
    {
      // probably a problem... but we'll try like this
      ext.clear();
    }

    if( ext.empty() )
    {
      if( FileUtil::fileStat( dataname + ".nii" ).find( '+' ) )
      {
        basename = dataname;
        dataname = dataname + ".nii";
        ext = "nii";
      }
      else if( FileUtil::fileStat( dataname + ".nii.gz" ).find( '+' ) )
      {
        basename = dataname;
        dataname = dataname + ".nii.gz";
        ext = "nii.gz";
      }
      else if( FileUtil::fileStat( dataname + ".img" ).find( '+' ) )
      {
        basename = dataname;
        dataname = dataname + ".img";
        ext = "img";
      }
      else if( FileUtil::fileStat( dataname + ".img.gz" ).find( '+' ) )
      {
        basename = dataname;
        dataname = dataname + ".img.gz";
        ext = "img.gz";
      }
    }
    minfname = dataname + ".minf";

    if( hdrname == pds->url() )
    {
      // if hdrname is original url
      dsl.addDataSource( "hdr", rc_ptr<DataSource>( pds ) );
    }
    else
    {
      if( !hdrname.empty()
        && FileUtil::fileStat( hdrname ).find( '+' ) != string::npos )
      {
        // if hdr file exists
        dsl.addDataSource( "hdr", rc_ptr<DataSource>
            ( new FileDataSource( hdrname ) ) );
      }
    }
    if( dataname == pds->url() )
    {
      // if dataname is original url
      dsl.addDataSource( "nii", rc_ptr<DataSource>( pds ) );
    }
    else
    {
      if( FileUtil::fileStat( dataname ).find( '+' ) != string::npos )
      {
        // if ima file exists
        dsl.addDataSource( "nii", rc_ptr<DataSource>
            ( new FileDataSource( dataname ) ) );
      }
      else
      {
        // we suppose original url is nii
        dsl.addDataSource( "nii", rc_ptr<DataSource>( pds ) );
      }
    }
  }
//   localMsg( "hdr: " + dsl.dataSource( "hdr" )->url() );
  localMsg( "nii: " + dsl.dataSource( "nii" )->url() );

  /* chexck for .bval and .bvec additional files (diffusion imaging, output of
     dcm2nii) */
  string bvecfile = basename + ".bvec";
  string bvalfile = basename + ".bval";
  if( FileUtil::fileStat( bvecfile ).find( '+' ) != string::npos
    && FileUtil::fileStat( bvalfile ).find( '+' ) != string::npos )
  {
    dsl.addDataSource( "bvec", rc_ptr<DataSource>
        ( new FileDataSource( bvecfile ) ) );
    dsl.addDataSource( "bval", rc_ptr<DataSource>
        ( new FileDataSource( bvalfile ) ) );
  }

  // Minf DataSource
  if( !minfname.empty()
      && FileUtil::fileStat( minfname ).find( '+' ) != string::npos )
  {
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
 * This method builds a header from a .hdr / .nii DataSource.
 * The argument is given by check(...) and is supposed to be a nifti file.
 ****************************************************************************/
Object NiftiFormatChecker::_buildHeader( DataSource* hds ) const
{
  FileDataSource* fds = dynamic_cast<FileDataSource *>( hds );
  string          fname = hds->url();

  nifti_image *nim = NULL;

  // niftilib's global debug level for status reporting
  nifti_set_debug_level( 0 );

  // check that given file is NIFTI and not ANALYZE
  if( api->is_nifti_file( fname.c_str() ) <= 0 )
    throw wrong_format_error( fname );

  // read header using niftilib without loading data
  nim = api->nifti_image_read( fname.c_str() , 0 );
  if( !nim )
  {
    io_error::launchErrnoExcept( fname );
    throw wrong_format_error( fname );
  }

  Object         hdr = Object::value( PropertySet() );  // header

  /***********************/
  /* DATA DIMENSIONALITY */
  /***********************/

  /* Dimensions of grid array */
  vector<int> dims;
  for( int i=1; i<=nim->ndim; ++i )
    dims.push_back( nim->dim[i] );
  while( dims.size() < 4 ) // complete to 4D
    dims.push_back( 1 );
  // space: x,y,z, time: t and 3 extra
  // dim[8]: dim[0]=ndim, dim[1]=nx, etc.

  /* Grid spacings */
  vector<float> vs;
  vs.reserve( nim->ndim >= 3 ? nim->ndim : 3 );
  for( int i=1; i<=nim->ndim; ++i )
    vs.push_back( nim->pixdim[i] );
  for( int i=vs.size(); i<3; ++i )
    vs.push_back( 1. );
  // space: x,y,z, time: t and 3 extra; index 0 is used for qfac
  // pixdim[8]: pixdim[1]=dx, etc.

  /****************/
  /* TYPE OF DATA */
  /****************/

  /* Type of data in voxels: DT_* code */
  string type;
  short  sz = 0;
  switch( nim->datatype )
  {
  case DT_INT8:
    type = "S8";
    sz = 8;
    break;
  case DT_INT16:
    type = "S16";
    sz = 16;
    break;
  case DT_INT32:
    type = "S32";
    sz = 32;
    break;
  case DT_FLOAT32:
    type = "FLOAT";
    sz = 32;
    break;
  case DT_DOUBLE:
    type = "DOUBLE";
    sz = 64;
    break;
  case DT_UINT8:
    type = "U8";
    sz = 8;
    break;
  case DT_UINT16:
    type = "U16";
    sz = 16;
    break;
  case DT_UINT32:
    type = "U32";
    sz = 32;
    break;
  case DT_RGB:
    type = "RGB";
    sz = sizeof( VoxelRGB ) * 8;
    break;
  case DT_RGBA32:
    type = "RGBA";
    sz = 32;
    break;
  case DT_COMPLEX64:
    type = "CFLOAT";
    sz = 64;
    break;
  case DT_COMPLEX128:
    type = "CDOUBLE";
    sz = 128;
    break;
  default:
    cerr << "Unrecognized datatype code in NIFTI file \"" << fname
        << "\"" << endl;
  }
  hdr->setProperty( "disk_data_type", type );
  hdr->setProperty( "bits_allocated", (int) sz );

  if ( sz != nim->nbyper * 8 )
    cerr << "Number of bytes per voxel doesn't match datatype in NIFTI file \"" << fname << "\"" << endl;

  /******************************/
  /* DATA SCALING and DATA_TYPE */
  /******************************/

  /* Scaling parameter: slope, intercept */
  string dt = type;
  if( ( nim->scl_slope != 0.0 )
       && ( ( nim->scl_slope != 1.0 ) || ( nim->scl_inter != 0.0 ) ) )
  {
    hdr->setProperty( "scale_factor", (float) nim->scl_slope );
    hdr->setProperty( "scale_offset", (float) nim->scl_inter );
    if( type != "DOUBLE" )
      dt = "FLOAT";
  }
  hdr->setProperty( "data_type", dt );
  hdr->setProperty( "scale_factor_applied", false );
  vector<string> vt;
  vt.push_back( dt );
  if( dt != type )
    vt.push_back( type );
  if( dt != "FLOAT" )
    vt.push_back( "FLOAT" );
  if( dt != "DOUBLE" )
    vt.push_back( "DOUBLE" );
  hdr->setProperty( "possible_data_types", vt );

  /* Calibration parameter: min, max */
  hdr->setProperty( "cal_min", (float) nim->cal_min);
  hdr->setProperty( "cal_max", (float) nim->cal_max);

  /*************************************************/
  /* MRI-SPECIFIC SPATIAL AND TEMPORAL INFORMATION */
  /*************************************************/

  /* Indexes (1,2,3, or 0) for MRI */
  hdr->setProperty( "freq_dim", (int) nim->freq_dim);
  /* Directions in dim[]/pixdim[] */
  hdr->setProperty( "phase_dim", (int) nim->phase_dim);
  /* Directions in dim[]/pixdim[] */
  hdr->setProperty( "slice_dim", (int) nim->slice_dim);

  /* Code for slice timing pattern */
  hdr->setProperty( "slice_code", (int) nim->slice_code);
  /* Index for start of slices */
  hdr->setProperty( "slice_start", (int) nim->slice_start);
  /* Index for end of slices */
  hdr->setProperty( "slice_end", (int) nim->slice_end);
  /* Time between individual slices */
  hdr->setProperty( "slice_duration", (float) nim->slice_duration);

  /**********************************************/
  /* 3D IMAGE ORIENTATION AND LOCATION IN SPACE */
  /**********************************************/
  vector< string > referentials;
  vector< vector < float > > transformations;

  /* Transformation from NIFTI (i,j,k) indexes to AIMS (x,y,z) indexes */
  /* NIFTI transformations are +x=Right, +y=Anterior, +z=Superior while */
  /* AIMS coordinates are +x=Left, +y=Posterior, +z=Inferior */

  int idir, jdir, kdir;
  vector< float > storage_to_memory( 16 );
  if ( nim->qform_code > NIFTI_XFORM_UNKNOWN )
    nifti_mat44_to_orientation( nim->qto_xyz , &idir,&jdir,&kdir ) ;
  else if ( nim->sform_code > NIFTI_XFORM_UNKNOWN )
  {
    nifti_mat44_to_orientation( nim->sto_xyz , &idir,&jdir,&kdir ) ;
  }
  else
  {
    // default is left-to-right, anterior-to-posterior and inferior-to-superior
    idir = NIFTI_L2R;
    jdir = NIFTI_P2A;
    kdir = NIFTI_I2S;
  }
  storage_to_memory[15] = 1;
  if( idir == NIFTI_R2L || idir == NIFTI_L2R )
  {
    storage_to_memory[0] = ( idir == NIFTI_R2L ) ? 1 : -1;
    storage_to_memory[3] = ( idir == NIFTI_R2L ) ? 0 : nim->dim[1]-1;
    if( jdir == NIFTI_A2P || jdir == NIFTI_P2A )
    {
      // simplest case: no axe swapping
      // i -> x, j -> y, k -> z
      storage_to_memory[5]  = ( jdir == NIFTI_A2P ) ? 1 : -1;
      storage_to_memory[7]  = ( jdir == NIFTI_A2P ) ? 0 : nim->dim[2]-1;
      storage_to_memory[10] = ( kdir == NIFTI_S2I ) ? 1 : -1;
      storage_to_memory[11] = ( kdir == NIFTI_S2I ) ? 0 : nim->dim[3]-1;
    }
    else
    {
      // i -> x, j -> z, k -> y
      storage_to_memory[6]  = ( kdir == NIFTI_A2P ) ? 1 : -1;
      storage_to_memory[7]  = ( kdir == NIFTI_A2P ) ? 0 : nim->dim[3]-1;
      storage_to_memory[9]  = ( jdir == NIFTI_S2I ) ? 1 : -1;
      storage_to_memory[11] = ( jdir == NIFTI_S2I ) ? 0 : nim->dim[2]-1;
    }
  }
  else
  {
    if( jdir == NIFTI_A2P || jdir == NIFTI_P2A )
    {
      // i -> z, j -> y, k -> x
      storage_to_memory[2]  = ( kdir == NIFTI_R2L ) ? 1 : -1;
      storage_to_memory[3]  = ( kdir == NIFTI_R2L ) ? 0 : nim->dim[3]-1;
      storage_to_memory[5]  = ( jdir == NIFTI_A2P ) ? 1 : -1;
      storage_to_memory[7]  = ( jdir == NIFTI_A2P ) ? 0 : nim->dim[2]-1;
      storage_to_memory[8]  = ( idir == NIFTI_S2I ) ? 1 : -1;
      storage_to_memory[11] = ( idir == NIFTI_S2I ) ? 0 : nim->dim[1]-1;
    }
    else
    {
      if( kdir == NIFTI_S2I || kdir == NIFTI_I2S )
      {
        // i -> y, j -> x, k -> z
        storage_to_memory[1]  = ( jdir == NIFTI_R2L ) ? 1 : -1;
        storage_to_memory[3]  = ( jdir == NIFTI_R2L ) ? 0 : nim->dim[2]-1;
        storage_to_memory[4]  = ( idir == NIFTI_A2P ) ? 1 : -1;
        storage_to_memory[7]  = ( idir == NIFTI_A2P ) ? 0 : nim->dim[1]-1;
        storage_to_memory[10] = ( kdir == NIFTI_S2I ) ? 1 : -1;
        storage_to_memory[11] = ( kdir == NIFTI_S2I ) ? 0 : nim->dim[3]-1;
      }
      else
      {
        if( idir == NIFTI_A2P || idir == NIFTI_P2A )
        {
          // i -> y, j -> z, k-> x
          storage_to_memory[2]  = ( kdir == NIFTI_R2L ) ? 1 : -1;
          storage_to_memory[3]  = ( kdir == NIFTI_R2L ) ? 0 : nim->dim[3]-1;
          storage_to_memory[4]  = ( idir == NIFTI_A2P ) ? 1 : -1;
          storage_to_memory[7]  = ( idir == NIFTI_A2P ) ? 0 : nim->dim[1]-1;
          storage_to_memory[9]  = ( jdir == NIFTI_S2I ) ? 1 : -1;
          storage_to_memory[11] = ( jdir == NIFTI_S2I ) ? 0 : nim->dim[2]-1;
        }
        else
        {
          // i -> z, j -> x, k -> y
          storage_to_memory[1]  = ( jdir == NIFTI_R2L ) ? 1 : -1;
          storage_to_memory[3]  = ( jdir == NIFTI_R2L ) ? 0 : nim->dim[2]-1;
          storage_to_memory[6]  = ( kdir == NIFTI_A2P ) ? 1 : -1;
          storage_to_memory[7]  = ( kdir == NIFTI_A2P ) ? 0 : nim->dim[3]-1;
          storage_to_memory[8]  = ( idir == NIFTI_S2I ) ? 1 : -1;
          storage_to_memory[11] = ( idir == NIFTI_S2I ) ? 0 : nim->dim[1]-1;
        }
      }
    }
  }

  hdr->setProperty( "storage_to_memory", storage_to_memory );

  AffineTransformation3d s2m( storage_to_memory );

  Point3df pdim = s2m.transform( Point3df( dims[0], dims[1], dims[2] ) )
      - s2m.transform( Point3df( 0, 0, 0 ) );
  dims[0] = (int) rint( fabs( pdim[0] ) );
  dims[1] = (int) rint( fabs( pdim[1] ) );
  dims[2] = (int) rint( fabs( pdim[2] ) );
//   hdr->setProperty( "volume_dimension", dims );
  hdr->setProperty( "sizeX", dims[0] );
  hdr->setProperty( "sizeY", dims[1] );
  hdr->setProperty( "sizeZ", dims[2] );
  hdr->setProperty( "sizeT", dims[3] );

  Point3df pvs = s2m.transform( Point3df( vs[0], vs[1], vs[2] ) )
      - s2m.transform( Point3df( 0, 0, 0 ) );
  vs[0] = fabs( pvs[0] );
  vs[1] = fabs( pvs[1] );
  vs[2] = fabs( pvs[2] );
  hdr->setProperty( "voxel_size", vs );

  AffineTransformation3d vsM;
  vsM.matrix[0] = vs[0]; // voxel size in memory orientation
  vsM.matrix(1,1) = vs[1];
  vsM.matrix(2,2) = vs[2];

  /* !!! As I understand, both qform and sform are transforming voxels to mm
     (Denis 2008/08/21) */

  /* Codes for (x,y,z) space meaning and associated transform */
  if( nim->qform_code > NIFTI_XFORM_UNKNOWN )
  {
    referentials.push_back( NiftiReferential( nim->qform_code ) );
    AffineTransformation3d qto_xyz;
    qto_xyz.matrix(0,3) = nim->qto_xyz.m[0][3];
    qto_xyz.matrix(1,3) = nim->qto_xyz.m[1][3];
    qto_xyz.matrix(2,3) = nim->qto_xyz.m[2][3];
    for (int i=0;i<3;++i)
      for (int j=0;j<3;++j)
        qto_xyz.matrix(i,j) = nim->qto_xyz.m[i][j];
    qto_xyz = qto_xyz * ( vsM * s2m ).inverse();
    transformations.push_back( qto_xyz.toVector() );
  }
  if( nim->sform_code > NIFTI_XFORM_UNKNOWN )
  {
    referentials.push_back( NiftiReferential( nim->sform_code ) );
    AffineTransformation3d sto_xyz;
    sto_xyz.matrix[12] = nim->sto_xyz.m[0][3];
    sto_xyz.matrix[13] = nim->sto_xyz.m[1][3];
    sto_xyz.matrix[14] = nim->sto_xyz.m[2][3];
    for (int i=0;i<3;++i)
      for (int j=0;j<3;++j)
        sto_xyz.matrix[j*4+i] = nim->sto_xyz.m[i][j];
    sto_xyz = sto_xyz  * ( vsM * s2m ).inverse();
    transformations.push_back( sto_xyz.toVector() );
  }

  hdr->setProperty( "referentials", referentials );
  hdr->setProperty( "transformations", transformations );

  /********************************************/
  /* UNITS OF SPATIAL AND TEMPORAL DIMENSIONS */
  /********************************************/

  /* Time coordinate offset */
  hdr->setProperty( "toffset", (float) nim->toffset );

  /* dx,dy,dz units: NIFTI_UNITS_* code */
  hdr->setProperty( "xyz_units", (int) nim->xyz_units );
  /* dt       units: NIFTI_UNITS_* code */
  hdr->setProperty( "time_units", (int) nim->time_units );

  /********************************/
  /* INTERPRETATION OF VOXEL DATA */
  /********************************/

  if( nim->intent_code > NIFTI_INTENT_NONE )
  {
    /* Statistic type (or something) */
    hdr->setProperty( "intent_code", (int) nim->intent_code );
    /* Intent parameters */
    hdr->setProperty( "intent_p1", (float) nim->intent_p1 );
    hdr->setProperty( "intent_p2", (float) nim->intent_p2 );
    hdr->setProperty( "intent_p3", (float) nim->intent_p3 );
    /* Optional description of intent data */
    hdr->setProperty( "intent_name", string( nim->intent_name ) );
  }

  /**********************************/
  /* DESCRIPTION AND AUXILIARY FILE */
  /**********************************/

  /* Optional text to describe dataset */
  hdr->setProperty( "descrip", string( nim->descrip ) );
  /* Auxiliary filename */
  hdr->setProperty( "aux_file", string( nim->aux_file ) );

  /*************************************/
  /* INTERNAL HEADERS (TO BE REMOVED)? */
  /*************************************/

  /* Header filename (.hdr or .nii) */
  /* setProperty( "fname", string( nim->fname ) ); */
  /* Image filename  (.img or .nii) */
  /* setProperty( "iname", string( nim->iname ) ); */
  /* Offset into iname where data starts */
  //setProperty( "iname_offset", (int) nim->iname_offset );
  /* Swap unit in image data (might be 0) */
  //setProperty( "swapsize", (int) nim->swapsize );
  /* Byte order on disk (MSB_ or LSB_FIRST) */
  //setProperty( "byteorder", (int) nim->byteorder );
  /* ANALYZE (0), NIFTI-1 1 file (1), NIFTI-1 2 files (2), NIFTI-ASCII (3) */
  hdr->setProperty( "nifti_type", (int) nim->nifti_type );

  /*********************/
  /* HEADER EXTENSIONS */
  /*********************/

  if( nim->num_ext )
  {
    vector< int > extcode( nim->num_ext );
        vector< vector< char > > extdata( nim->num_ext );
    cerr << formatName() << " extensions are present in header but will be ignored: \"" << fname << "\"" << endl;
    for (int i=0; i<nim->num_ext;++i)
    {
      extcode.push_back( nim->ext_list[i].ecode );
      vector< char > edata( nim->ext_list[i].esize );
      memcpy( &edata[0],nim->ext_list[i].edata,nim->ext_list[i].esize );
      extdata.push_back( edata );
    }
    hdr->setProperty( "extcode", extcode );
    hdr->setProperty( "extdata", extdata );
  }

  /* keep the nifti structur in the header, because it will be used during
     reading. */
  hdr->setProperty( "_nifti_structure",
                    Object::value( rc_ptr<NiftiStructWrapper>(
                      new NiftiStructWrapper( nim ) ) ) );

  // the following shape is more logical, but incompatible with AIMS.
  // hdr->setProperty( "object_type", string( "Volume of " ) + dt );
  hdr->setProperty( "object_type", string( "Volume" ) );
  hdr->setProperty( "format", formatName() );

  return hdr;
}


void NiftiFormatChecker::setApi( NiftiApiHelpers *napi )
{
  api = napi;
}

//============================================================================
//   P U B L I C   M E T H O D S
//============================================================================

NiftiFormatChecker::NiftiFormatChecker()
  : FormatChecker(), api( 0 )
{
}


NiftiFormatChecker::~NiftiFormatChecker()
{
  delete api;
}

DataSourceInfo NiftiFormatChecker::check( DataSourceInfo dsi,
                                           DataSourceInfoLoader & /* f */,
                                           Object options ) const
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
        || dsi.header()->getProperty( "format" )->getString() != formatName() )
      throw wrong_format_error( string( "Not a " ) + formatName() + " header",
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
    DataSource* hds = 0;
    try
    {
      hds = dsi.list().dataSource( "hdr" ).get();
    }
    catch(...)
    {
    }
    if( !hds )
      hds = dsi.list().dataSource( "nii" ).get();
    if( !hds )
      return dsi;

    dsi.header() = _buildHeader( hds );
    // move _nifti_structure to privateIOData
    if( dsi.header()->hasProperty( "_nifti_structure" ) )
    {
      dsi.privateIOData()->setProperty( "nifti_structure",
        dsi.header()->getProperty( "_nifti_structure" ) );
      dsi.header()->removeProperty( "_nifti_structure" );
    }

    // read bvec/bval files if present
    try
    {
      DataSource* bvecfile = dsi.list().dataSource( "bvec" ).get();
      DataSource* bvalfile = dsi.list().dataSource( "bval" ).get();
      if( bvecfile && bvalfile )
      {
        _readDiffusionVectors( bvecfile, bvalfile, dsi.header() );
      }
    }
    catch( ... )
    {
    }

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
  if( docapa )
  {
    localMsg ("Writing capabilities..." );
    dsi.capabilities().setMemoryMapping( false );
//     try
//     {
//       if( !(bool) dsi.header()->getProperty( "byte_swapping" )->getScalar() )
//         dsi.capabilities().setMemoryMapping( true );
//     }
//     catch( ... ) {}

    dsi.capabilities().setDataSource( dsi.list().dataSource( "nii" ) );
    dsi.capabilities().setThreadSafe( true ); /* TODO */
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


void NiftiFormatChecker::_readDiffusionVectors( DataSource* bvecfile,
                                                DataSource* bvalfile,
                                                Object header ) const
{
  bvalfile->open( DataSource::Read );
  vector<float> bvals;
  while( (*bvalfile) )
  {
    float val;
    AsciiDataSourceTraits<float>::read( *bvalfile, val );
    if( !(*bvalfile) )
      break;
    StreamUtil::skip( *bvalfile );
    bvals.push_back( val );
  }
  bvalfile->close();

  bvecfile->open( DataSource::Read );
  vector<float> bvecs;
  while( (*bvecfile) )
  {
    float val;
    AsciiDataSourceTraits<float>::read( *bvecfile, val );
    if( !(*bvecfile) )
      break;
    StreamUtil::skip( *bvecfile );
    bvecs.push_back( val );
  }
  bvecfile->close();

  if( bvecs.size() != bvals.size() * 3 )
  {
    cerr << "B values / diffusion_directions inconsistency\n";
    return;
  }

  /* .bvec file is stored in the same orientation as disk storage, thus
     needs to undergo storage_to_memory transform. */
  std::vector< float > storage_to_memory;
  header->getProperty( "storage_to_memory", storage_to_memory );
  AffineTransformation3d s2m( storage_to_memory );

  unsigned i, n = bvals.size();
  vector<vector<float> > bvecs2( n );
  for( unsigned i=0; i<n; ++i )
  {
    Point3df vdir1( bvecs[i], bvecs[i+n], bvecs[i+n*2] );
    Point3df vdir2 = s2m.transform( vdir1 )
      - s2m.transform( Point3df( 0, 0, 0 ) );
    vector<float> dir( 3 );
    dir[0] = vdir2[0];
    dir[1] = vdir2[1];
    dir[2] = vdir2[2];
    bvecs2[i] = dir;
  }

  header->setProperty( "b_values", bvals );
  header->setProperty( "diffusion_directions", bvecs2 );
}


//============================================================================
// Nifti1FormatChecker
//============================================================================

Nifti1FormatChecker::Nifti1FormatChecker()
  : NiftiFormatChecker()
{
  setApi( new Nifti1ApiHelpers );
}


Nifti1FormatChecker::~Nifti1FormatChecker()
{
}


//============================================================================
// Nifti2FormatChecker
//============================================================================

Nifti2FormatChecker::Nifti2FormatChecker()
  : NiftiFormatChecker()
{
  setApi( new Nifti2ApiHelpers );
}


Nifti2FormatChecker::~Nifti2FormatChecker()
{
}


//============================================================================
// NiftiStructWrapper
//============================================================================
/*** Instantiate NiftiStructWrapper destructor
 * and compile the corresponding generic object
 ****************************************************************************/

NiftiStructWrapper::~NiftiStructWrapper()
{
  /* free the nifti structure */
  if( nim )
  {
    nifti_image_free( nim );
    nim = 0;
  }
}

//============================================================================
// NiftiFileWrapper
//============================================================================
/*** Instantiate NiftiFileWrapper destructor
 ****************************************************************************/

NiftiFileWrapper::~NiftiFileWrapper()
{
  if( znzfile )
  {
    znzclose( znzfile );
    znzfile = 0;
  }
}

#include <cartobase/object/object_d.h>

INSTANTIATE_GENERIC_OBJECT_TYPE( carto::rc_ptr<NiftiStructWrapper> )

#undef localMsg

