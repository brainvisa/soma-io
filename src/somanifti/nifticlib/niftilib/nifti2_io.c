/* The strdup function is needed in nifti1_io.c by the patch by Denis Riviere,
 * 2012/11/14. We need to define _XOPEN_SOURCE >= 500 before including
 * <string.h> because strdup is not defined by default in strict ANSI mode
 * (-std=c89). */
#define _XOPEN_SOURCE 500
#include <string.h>

#define _NIFTI1_IO_C_  /* to allow some conditional defs in nifti1_io.h */
#include "nifti2_io.h"

/* inlcude the .c for nifti-1
   this is to allow the modified code for nifti-2 to use static functions
   of nifti1_io.c
*/
#include "nifti1_io.c"


#if 0  /* deactivate this code */
/*! global nifti options structure - init with defaults */
static nifti_global_options g_opts = {
        1, /* debug level                                         */
        0, /* skip_blank_ext    - skip extender if no extensions  */
        1  /* allow_upper_fext  - allow uppercase file extensions */
};
#endif

static int   need_nhdr2_swap    (int64_t dim0, int hdrsize);
static int nifti2_read_extensions( nifti_image *nim, znzFile fp, int remain );


void  swap_nifti2_header ( struct nifti_2_header *h , int is_nifti )
{
   nifti_swap_4bytes(1, &h->sizeof_hdr);

   nifti_swap_4bytes(8, h->dim);
   nifti_swap_8bytes(1, &h->intent_p1);
   nifti_swap_8bytes(1, &h->intent_p2);
   nifti_swap_8bytes(1, &h->intent_p3);

   nifti_swap_4bytes(1, &h->intent_code);
   nifti_swap_2bytes(1, &h->datatype);
   nifti_swap_2bytes(1, &h->bitpix);
   nifti_swap_8bytes(1, &h->slice_start);

   nifti_swap_8bytes(8, h->pixdim);

   nifti_swap_8bytes(1, &h->vox_offset);
   nifti_swap_8bytes(1, &h->scl_slope);
   nifti_swap_8bytes(1, &h->scl_inter);
   nifti_swap_8bytes(1, &h->slice_end);

   nifti_swap_8bytes(1, &h->cal_max);
   nifti_swap_8bytes(1, &h->cal_min);
   nifti_swap_8bytes(1, &h->slice_duration);
   nifti_swap_8bytes(1, &h->toffset);

   nifti_swap_4bytes(1, &h->qform_code);
   nifti_swap_4bytes(1, &h->sform_code);

   nifti_swap_8bytes(1, &h->quatern_b);
   nifti_swap_8bytes(1, &h->quatern_c);
   nifti_swap_8bytes(1, &h->quatern_d);
   nifti_swap_8bytes(1, &h->qoffset_x);
   nifti_swap_8bytes(1, &h->qoffset_y);
   nifti_swap_8bytes(1, &h->qoffset_z);

   nifti_swap_8bytes(4, h->srow_x);
   nifti_swap_8bytes(4, h->srow_y);
   nifti_swap_8bytes(4, h->srow_z);

   return ;
}


struct nifti_2_header   nifti2_convert_nim2nhdr(const nifti_image* nim)
{
   struct nifti_2_header nhdr;

   memset(&nhdr,0,sizeof(nhdr)) ;  /* zero out header, to be safe */


   /**- load the ANALYZE-7.5 generic parts of the header struct */

   nhdr.sizeof_hdr = sizeof(nhdr) ;

   nhdr.dim[0] = nim->ndim ;
   nhdr.dim[1] = nim->nx ; nhdr.dim[2] = nim->ny ; nhdr.dim[3] = nim->nz ;
   nhdr.dim[4] = nim->nt ; nhdr.dim[5] = nim->nu ; nhdr.dim[6] = nim->nv ;
   nhdr.dim[7] = nim->nw ;

   nhdr.pixdim[0] = 0.0 ;
   nhdr.pixdim[1] = nim->dx ; nhdr.pixdim[2] = nim->dy ;
   nhdr.pixdim[3] = nim->dz ; nhdr.pixdim[4] = nim->dt ;
   nhdr.pixdim[5] = nim->du ; nhdr.pixdim[6] = nim->dv ;
   nhdr.pixdim[7] = nim->dw ;

   nhdr.datatype = nim->datatype ;
   nhdr.bitpix   = 8 * nim->nbyper ;

   if( nim->cal_max > nim->cal_min ){
     nhdr.cal_max = nim->cal_max ;
     nhdr.cal_min = nim->cal_min ;
   }

   if( nim->scl_slope != 0.0 ){
     nhdr.scl_slope = nim->scl_slope ;
     nhdr.scl_inter = nim->scl_inter ;
   }

   if( nim->descrip[0] != '\0' ){
     memcpy(nhdr.descrip ,nim->descrip ,79) ; nhdr.descrip[79] = '\0' ;
   }
   if( nim->aux_file[0] != '\0' ){
     memcpy(nhdr.aux_file ,nim->aux_file ,23) ; nhdr.aux_file[23] = '\0' ;
   }

   /**- Load NIFTI specific stuff into the header */

   if( nim->nifti_type > NIFTI_FTYPE_ANALYZE ){ /* then not ANALYZE */

     if( nim->nifti_type == NIFTI_FTYPE_NIFTI1_1 ) strcpy(nhdr.magic,"n+2") ;
     else                                          strcpy(nhdr.magic,"ni2") ;

     nhdr.pixdim[1] = fabs(nhdr.pixdim[1]) ; nhdr.pixdim[2] = fabs(nhdr.pixdim[2]) ;
     nhdr.pixdim[3] = fabs(nhdr.pixdim[3]) ; nhdr.pixdim[4] = fabs(nhdr.pixdim[4]) ;
     nhdr.pixdim[5] = fabs(nhdr.pixdim[5]) ; nhdr.pixdim[6] = fabs(nhdr.pixdim[6]) ;
     nhdr.pixdim[7] = fabs(nhdr.pixdim[7]) ;

     nhdr.intent_code = nim->intent_code ;
     nhdr.intent_p1   = nim->intent_p1 ;
     nhdr.intent_p2   = nim->intent_p2 ;
     nhdr.intent_p3   = nim->intent_p3 ;
     if( nim->intent_name[0] != '\0' ){
       memcpy(nhdr.intent_name,nim->intent_name,15) ;
       nhdr.intent_name[15] = '\0' ;
     }

     nhdr.vox_offset  = (float) nim->iname_offset ;
     nhdr.xyzt_units  = SPACE_TIME_TO_XYZT( nim->xyz_units, nim->time_units ) ;
     nhdr.toffset     = nim->toffset ;

     if( nim->qform_code > 0 ){
       nhdr.qform_code = nim->qform_code ;
       nhdr.quatern_b  = nim->quatern_b ;
       nhdr.quatern_c  = nim->quatern_c ;
       nhdr.quatern_d  = nim->quatern_d ;
       nhdr.qoffset_x  = nim->qoffset_x ;
       nhdr.qoffset_y  = nim->qoffset_y ;
       nhdr.qoffset_z  = nim->qoffset_z ;
       nhdr.pixdim[0]  = (nim->qfac >= 0.0) ? 1.0 : -1.0 ;
     }

     if( nim->sform_code > 0 ){
       nhdr.sform_code = nim->sform_code ;
       nhdr.srow_x[0]  = nim->sto_xyz.m[0][0] ;
       nhdr.srow_x[1]  = nim->sto_xyz.m[0][1] ;
       nhdr.srow_x[2]  = nim->sto_xyz.m[0][2] ;
       nhdr.srow_x[3]  = nim->sto_xyz.m[0][3] ;
       nhdr.srow_y[0]  = nim->sto_xyz.m[1][0] ;
       nhdr.srow_y[1]  = nim->sto_xyz.m[1][1] ;
       nhdr.srow_y[2]  = nim->sto_xyz.m[1][2] ;
       nhdr.srow_y[3]  = nim->sto_xyz.m[1][3] ;
       nhdr.srow_z[0]  = nim->sto_xyz.m[2][0] ;
       nhdr.srow_z[1]  = nim->sto_xyz.m[2][1] ;
       nhdr.srow_z[2]  = nim->sto_xyz.m[2][2] ;
       nhdr.srow_z[3]  = nim->sto_xyz.m[2][3] ;
     }

     nhdr.dim_info = FPS_INTO_DIM_INFO( nim->freq_dim ,
                                        nim->phase_dim , nim->slice_dim ) ;
     nhdr.slice_code     = nim->slice_code ;
     nhdr.slice_start    = nim->slice_start ;
     nhdr.slice_end      = nim->slice_end ;
     nhdr.slice_duration = nim->slice_duration ;
   }

   return nhdr;
}


nifti_2_header * nifti2_read_header(const char *hname, int *swapped, int check)
{
   nifti_2_header   nhdr, * hptr;
   znzFile          fp;
   int              bytes, lswap;
   char           * hfile;
   char             fname[] = { "nifti_read_header" };

   /* determine file name to use for header */
   hfile = nifti_findhdrname(hname);
   if( hfile == NULL ){
      if( g_opts.debug > 0 )
         LNI_FERR(fname,"failed to find header file for", hname);
      return NULL;
   } else if( g_opts.debug > 1 )
      fprintf(stderr,"-d %s: found header filename '%s'\n",fname,hfile);

   fp = znzopen( hfile, "rb", nifti_is_gzfile(hfile) );
   if( znz_isnull(fp) ){
      if( g_opts.debug > 0 ) LNI_FERR(fname,"failed to open header file",hfile);
      free(hfile);
      return NULL;
   }

   free(hfile);  /* done with filename */

   if( has_ascii_header(fp) == 1 ){
      znzclose( fp );
      if( g_opts.debug > 0 )
         LNI_FERR(fname,"ASCII header type not supported",hname);
      return NULL;
   }

   /* read the binary header */
   bytes = (int)znzread( &nhdr, 1, sizeof(nhdr), fp );
   znzclose( fp );                      /* we are done with the file now */

   if( bytes < (int)sizeof(nhdr) ){
      if( g_opts.debug > 0 ){
         LNI_FERR(fname,"bad binary header read for file", hname);
         fprintf(stderr,"  - read %d of %d bytes\n",bytes, (int)sizeof(nhdr));
      }
      return NULL;
   }

   /* now just decide on byte swapping */
   lswap = need_nhdr2_swap(nhdr.dim[0], nhdr.sizeof_hdr); /* swap data flag */
   if( check && lswap < 0 ){
      LNI_FERR(fname,"bad nifti_2_header for file", hname);
      return NULL;
   } else if ( lswap < 0 ) {
      lswap = 0;  /* if swapping does not help, don't do it */
      if(g_opts.debug > 1) fprintf(stderr,"-- swap failure, none applied\n");
   }

   if( lswap ) {
      /*if ( g_opts.debug > 3 ) disp_nifti_2_header("-d nhdr pre-swap: ", &nhdr);*/
      swap_nifti2_header( &nhdr , NIFTI_VERSION(nhdr) ) ;
   }

   /*
   if ( g_opts.debug > 2 ) disp_nifti_2_header("-d nhdr post-swap: ", &nhdr);
   */

   if ( check && ! nifti2_hdr_looks_good(&nhdr) ){
      LNI_FERR(fname,"nifti_2_header looks bad for file", hname);
      return NULL;
   }

   /* all looks good, so allocate memory for and return the header */
   hptr = (nifti_2_header *)malloc(sizeof(nifti_2_header));
   if( ! hptr ){
      fprintf(stderr,"** nifti2_read_hdr: failed to alloc nifti_2_header\n");
      return NULL;
   }

   if( swapped ) *swapped = lswap;  /* only if they care <sniff!> */

   memcpy(hptr, &nhdr, sizeof(nifti_2_header));

   return hptr;
}


#undef  ERREX
#define ERREX(msg)                                           \
 do{ fprintf(stderr,"** ERROR: nifti2_convert_nhdr2nim: %s\n", (msg) ) ;  \
     return NULL ; } while(0)

nifti_image    * nifti2_convert_nhdr2nim(struct nifti_2_header nhdr,
                                         const char * fname)
{
   int   ii , doswap , ioff ;
   int   is_nifti , is_onefile ;
   nifti_image *nim;

   nim = (nifti_image *)calloc( 1 , sizeof(nifti_image) ) ;
   if( !nim ) ERREX("failed to allocate nifti image");

   /* be explicit with pointers */
   nim->fname = NULL;
   nim->iname = NULL;
   nim->data = NULL;

   /**- check if we must swap bytes */

   doswap = need_nhdr2_swap(nhdr.dim[0], nhdr.sizeof_hdr); /* swap data flag */

   if( doswap < 0 ){
      free(nim);
      if( doswap == -1 ) ERREX("bad dim[0]") ;
      ERREX("bad sizeof_hdr") ;  /* else */
   }

   /**- determine if this is a NIFTI-1 compliant header */

   is_nifti = NIFTI_VERSION(nhdr) ;
   /*
    * before swapping header, record the Analyze75 orient code
    */
   if(!is_nifti)
     {
     free(nim);
     return NULL;
     }
   if( doswap ) {
      /*
      if ( g_opts.debug > 3 ) disp_nifti_2_header("-d ni1 pre-swap: ", &nhdr);
      */
      swap_nifti2_header( &nhdr , is_nifti ) ;
   }

   /*
   if ( g_opts.debug > 2 ) disp_nifti_2_header("-d nhdr2nim : ", &nhdr);
   */

   if( nhdr.datatype == DT_BINARY ||
       nhdr.datatype == DT_UNKNOWN  )    ERREX("bad datatype") ;

   if( nhdr.dim[1] <= 0 )                ERREX("bad dim[1]") ;

   /* fix bad dim[] values in the defined dimension range */
   for( ii=2 ; ii <= nhdr.dim[0] ; ii++ )
     if( nhdr.dim[ii] <= 0 ) nhdr.dim[ii] = 1 ;

   /* fix any remaining bad dim[] values, so garbage does not propagate */
   /* (only values 0 or 1 seem rational, otherwise set to arbirary 1)   */
   for( ii=nhdr.dim[0]+1 ; ii <= 7 ; ii++ )
     if( nhdr.dim[ii] != 1 && nhdr.dim[ii] != 0) nhdr.dim[ii] = 1 ;

#if 0  /* rely on dim[0], do not attempt to modify it   16 Nov 2005 [rickr] */

   /**- get number of dimensions (ignoring dim[0] now) */
   for( ii=7 ; ii >= 2 ; ii-- )            /* loop backwards until we  */
     if( nhdr.dim[ii] > 1 ) break ;        /* find a dim bigger than 1 */
   ndim = ii ;
#endif

   /**- set bad grid spacings to 1.0 */

   for( ii=1 ; ii <= nhdr.dim[0] ; ii++ ){
     if( nhdr.pixdim[ii] == 0.0         ||
         !IS_GOOD_FLOAT(nhdr.pixdim[ii])  ) nhdr.pixdim[ii] = 1.0 ;
   }

  is_onefile = is_nifti && NIFTI_ONEFILE(nhdr) ;

  if( is_nifti ) nim->nifti_type = (is_onefile) ? NIFTI_FTYPE_NIFTI1_1
                                                : NIFTI_FTYPE_NIFTI1_2 ;

  ii = nifti_short_order() ;
  if( doswap )   nim->byteorder = REVERSE_ORDER(ii) ;
  else           nim->byteorder = ii ;


  /**- set dimensions of data array */

  nim->ndim = nim->dim[0] = nhdr.dim[0];
  nim->nx   = nim->dim[1] = nhdr.dim[1];
  nim->ny   = nim->dim[2] = nhdr.dim[2];
  nim->nz   = nim->dim[3] = nhdr.dim[3];
  nim->nt   = nim->dim[4] = nhdr.dim[4];
  nim->nu   = nim->dim[5] = nhdr.dim[5];
  nim->nv   = nim->dim[6] = nhdr.dim[6];
  nim->nw   = nim->dim[7] = nhdr.dim[7];

  for( ii=1, nim->nvox=1; ii <= nhdr.dim[0]; ii++ )
     nim->nvox *= nhdr.dim[ii];

  /**- set the type of data in voxels and how many bytes per voxel */

  nim->datatype = nhdr.datatype ;

  nifti_datatype_sizes( nim->datatype , &(nim->nbyper) , &(nim->swapsize) ) ;
  if( nim->nbyper == 0 ){ free(nim); ERREX("bad datatype"); }

  /**- set the grid spacings */

  nim->dx = nim->pixdim[1] = nhdr.pixdim[1] ;
  nim->dy = nim->pixdim[2] = nhdr.pixdim[2] ;
  nim->dz = nim->pixdim[3] = nhdr.pixdim[3] ;
  nim->dt = nim->pixdim[4] = nhdr.pixdim[4] ;
  nim->du = nim->pixdim[5] = nhdr.pixdim[5] ;
  nim->dv = nim->pixdim[6] = nhdr.pixdim[6] ;
  nim->dw = nim->pixdim[7] = nhdr.pixdim[7] ;

  /**- compute qto_xyz transformation from pixel indexes (i,j,k) to (x,y,z) */

  if( !is_nifti || nhdr.qform_code <= 0 ){
    /**- if not nifti or qform_code <= 0, use grid spacing for qto_xyz */

    nim->qto_xyz.m[0][0] = nim->dx ;  /* grid spacings */
    nim->qto_xyz.m[1][1] = nim->dy ;  /* along diagonal */
    nim->qto_xyz.m[2][2] = nim->dz ;

    /* off diagonal is zero */

    nim->qto_xyz.m[0][1]=nim->qto_xyz.m[0][2]=nim->qto_xyz.m[0][3] = 0.0;
    nim->qto_xyz.m[1][0]=nim->qto_xyz.m[1][2]=nim->qto_xyz.m[1][3] = 0.0;
    nim->qto_xyz.m[2][0]=nim->qto_xyz.m[2][1]=nim->qto_xyz.m[2][3] = 0.0;

    /* last row is always [ 0 0 0 1 ] */

    nim->qto_xyz.m[3][0]=nim->qto_xyz.m[3][1]=nim->qto_xyz.m[3][2] = 0.0;
    nim->qto_xyz.m[3][3]= 1.0 ;

    nim->qform_code = NIFTI_XFORM_UNKNOWN ;

    if( g_opts.debug > 1 ) fprintf(stderr,"-d no qform provided\n");
  } else {
    /**- else NIFTI: use the quaternion-specified transformation */

    nim->quatern_b = FIXED_FLOAT( nhdr.quatern_b ) ;
    nim->quatern_c = FIXED_FLOAT( nhdr.quatern_c ) ;
    nim->quatern_d = FIXED_FLOAT( nhdr.quatern_d ) ;

    nim->qoffset_x = FIXED_FLOAT(nhdr.qoffset_x) ;
    nim->qoffset_y = FIXED_FLOAT(nhdr.qoffset_y) ;
    nim->qoffset_z = FIXED_FLOAT(nhdr.qoffset_z) ;

    nim->qfac = (nhdr.pixdim[0] < 0.0) ? -1.0 : 1.0 ;  /* left-handedness? */

    nim->qto_xyz = nifti_quatern_to_mat44(
                      nim->quatern_b, nim->quatern_c, nim->quatern_d,
                      nim->qoffset_x, nim->qoffset_y, nim->qoffset_z,
                      nim->dx       , nim->dy       , nim->dz       ,
                      nim->qfac                                      ) ;

    nim->qform_code = nhdr.qform_code ;

    if( g_opts.debug > 1 )
       nifti_disp_matrix_orient("-d qform orientations:\n", nim->qto_xyz);
  }

  /**- load inverse transformation (x,y,z) -> (i,j,k) */

  nim->qto_ijk = nifti_mat44_inverse( nim->qto_xyz ) ;

  /**- load sto_xyz affine transformation, if present */

  if( !is_nifti || nhdr.sform_code <= 0 ){
    /**- if not nifti or sform_code <= 0, then no sto transformation */

    nim->sform_code = NIFTI_XFORM_UNKNOWN ;

    if( g_opts.debug > 1 ) fprintf(stderr,"-d no sform provided\n");

  } else {
    /**- else set the sto transformation from srow_*[] */

    nim->sto_xyz.m[0][0] = nhdr.srow_x[0] ;
    nim->sto_xyz.m[0][1] = nhdr.srow_x[1] ;
    nim->sto_xyz.m[0][2] = nhdr.srow_x[2] ;
    nim->sto_xyz.m[0][3] = nhdr.srow_x[3] ;

    nim->sto_xyz.m[1][0] = nhdr.srow_y[0] ;
    nim->sto_xyz.m[1][1] = nhdr.srow_y[1] ;
    nim->sto_xyz.m[1][2] = nhdr.srow_y[2] ;
    nim->sto_xyz.m[1][3] = nhdr.srow_y[3] ;

    nim->sto_xyz.m[2][0] = nhdr.srow_z[0] ;
    nim->sto_xyz.m[2][1] = nhdr.srow_z[1] ;
    nim->sto_xyz.m[2][2] = nhdr.srow_z[2] ;
    nim->sto_xyz.m[2][3] = nhdr.srow_z[3] ;

    /* last row is always [ 0 0 0 1 ] */

    nim->sto_xyz.m[3][0]=nim->sto_xyz.m[3][1]=nim->sto_xyz.m[3][2] = 0.0;
    nim->sto_xyz.m[3][3]= 1.0 ;

    nim->sto_ijk = nifti_mat44_inverse( nim->sto_xyz ) ;

    nim->sform_code = nhdr.sform_code ;

    if( g_opts.debug > 1 )
       nifti_disp_matrix_orient("-d sform orientations:\n", nim->sto_xyz);
  }

  /**- set miscellaneous NIFTI stuff */

  if( is_nifti ){
    nim->scl_slope   = FIXED_FLOAT( nhdr.scl_slope ) ;
    nim->scl_inter   = FIXED_FLOAT( nhdr.scl_inter ) ;

    nim->intent_code = nhdr.intent_code ;

    nim->intent_p1 = FIXED_FLOAT( nhdr.intent_p1 ) ;
    nim->intent_p2 = FIXED_FLOAT( nhdr.intent_p2 ) ;
    nim->intent_p3 = FIXED_FLOAT( nhdr.intent_p3 ) ;

    nim->toffset   = FIXED_FLOAT( nhdr.toffset ) ;

    memcpy(nim->intent_name,nhdr.intent_name,15); nim->intent_name[15] = '\0';

    nim->xyz_units  = XYZT_TO_SPACE(nhdr.xyzt_units) ;
    nim->time_units = XYZT_TO_TIME (nhdr.xyzt_units) ;

    nim->freq_dim  = DIM_INFO_TO_FREQ_DIM ( nhdr.dim_info ) ;
    nim->phase_dim = DIM_INFO_TO_PHASE_DIM( nhdr.dim_info ) ;
    nim->slice_dim = DIM_INFO_TO_SLICE_DIM( nhdr.dim_info ) ;

    nim->slice_code     = nhdr.slice_code  ;
    nim->slice_start    = nhdr.slice_start ;
    nim->slice_end      = nhdr.slice_end   ;
    nim->slice_duration = FIXED_FLOAT(nhdr.slice_duration) ;
  }

  /**- set Miscellaneous ANALYZE stuff */

  nim->cal_min = FIXED_FLOAT(nhdr.cal_min) ;
  nim->cal_max = FIXED_FLOAT(nhdr.cal_max) ;

  memcpy(nim->descrip ,nhdr.descrip ,79) ; nim->descrip [79] = '\0' ;
  memcpy(nim->aux_file,nhdr.aux_file,23) ; nim->aux_file[23] = '\0' ;

   /**- set ioff from vox_offset (but at least sizeof(header)) */

   is_onefile = is_nifti && NIFTI_ONEFILE(nhdr) ;

   if( is_onefile ){
     ioff = (int)nhdr.vox_offset ;
     if( ioff < (int) sizeof(nhdr) ) ioff = (int) sizeof(nhdr) ;
   } else {
     ioff = (int)nhdr.vox_offset ;
   }
   nim->iname_offset = ioff ;


   /**- deal with file names if set */
   if (fname!=NULL) {
       nifti_set_filenames(nim,fname,0,0);
       if (nim->iname==NULL)  { ERREX("bad filename"); }
   } else {
     nim->fname = NULL;
     nim->iname = NULL;
   }

   /* clear extension fields */
   nim->num_ext = 0;
   nim->ext_list = NULL;

   return nim;
}


int    nifti2_hdr_looks_good (const nifti_2_header * hdr)
{
   int is_nifti, c, errs = 0;

   /* check dim[0] and sizeof_hdr */
   if( need_nhdr2_swap(hdr->dim[0], hdr->sizeof_hdr) < 0 ){
      if( g_opts.debug > 0 )
         fprintf(stderr,"** bad nhdr fields: dim0, sizeof_hdr = %ld, %d\n",
                 hdr->dim[0], hdr->sizeof_hdr);
      errs++;
   }

   /* check the valid dimension sizes (maybe dim[0] is bad) */
   for( c = 1; c <= hdr->dim[0] && c <= 7; c++ )
      if( hdr->dim[c] <= 0 ){
         if( g_opts.debug > 0 )
            fprintf(stderr,"** bad nhdr field: dim[%d] = %ld\n",c,hdr->dim[c]);
         errs++;
      }

   is_nifti = NIFTI_VERSION(*hdr);      /* determine header type */

   if( is_nifti ){      /* NIFTI */

      if( ! nifti_datatype_is_valid(hdr->datatype, 1) ){
         if( g_opts.debug > 0 )
            fprintf(stderr,"** bad NIFTI datatype in hdr, %d\n",hdr->datatype);
         errs++;
      }

   }

   if( errs ) return 0;  /* problems */

   if( g_opts.debug > 2 ) fprintf(stderr,"-d nifti header looks good\n");

   return 1;   /* looks good */
}


static int need_nhdr2_swap( int64_t dim0, int hdrsize )
{
   int64_t d0    = dim0;     /* so we won't have to swap them on the stack */
   int   hsize = hdrsize;

   if( d0 != 0 ){     /* then use it for the check */
      if( d0 > 0 && d0 <= 7 ) return 0;

      nifti_swap_8bytes(1, &d0);        /* swap? */
      if( d0 > 0 && d0 <= 7 ) return 1;

      if( g_opts.debug > 1 ){
         fprintf(stderr,"** NIFTI: bad swapped d0 = %ld, unswapped = ", d0);
         nifti_swap_8bytes(1, &d0);        /* swap? */
         fprintf(stderr,"%ld\n", d0);
      }

      return -1;        /* bad, naughty d0 */
   }

   /* dim[0] == 0 should not happen, but could, so try hdrsize */
   if( hsize == sizeof(nifti_2_header) ) return 0;

   nifti_swap_4bytes(1, &hsize);     /* swap? */
   if( hsize == sizeof(nifti_2_header) ) return 1;

   if( g_opts.debug > 1 ){
      fprintf(stderr,"** NIFTI: bad swapped hsize = %d, unswapped = ", hsize);
      nifti_swap_4bytes(1, &hsize);        /* swap? */
      fprintf(stderr,"%d\n", hsize);
   }

   return -2;     /* bad, naughty hsize */
}


nifti_image *nifti2_image_read( const char *hname , int read_data )
{
   struct nifti_2_header  nhdr ;
   nifti_image           *nim ;
   znzFile                fp ;
   int                    rv, ii , filesize, remaining;
   char                   fname[] = { "nifti_image_read" };
   char                  *hfile=NULL;

   if( g_opts.debug > 1 ){
      fprintf(stderr,"-d image_read from '%s', read_data = %d",hname,read_data);
#ifdef HAVE_ZLIB
      fprintf(stderr,", HAVE_ZLIB = 1\n");
#else
      fprintf(stderr,", HAVE_ZLIB = 0\n");
#endif
   }

   /**- determine filename to use for header */
   hfile = nifti_findhdrname(hname);
   if( hfile == NULL ){
      if(g_opts.debug > 0)
         LNI_FERR(fname,"failed to find header file for", hname);
      return NULL;  /* check return */
   } else if( g_opts.debug > 1 )
      fprintf(stderr,"-d %s: found header filename '%s'\n",fname,hfile);

   if( nifti_is_gzfile(hfile) ) filesize = -1;  /* unknown */
   else                         filesize = nifti_get_filesize(hfile);

   fp = znzopen(hfile, "rb", nifti_is_gzfile(hfile));
   if( znz_isnull(fp) ){
      if( g_opts.debug > 0 ) LNI_FERR(fname,"failed to open header file",hfile);
      free(hfile);
      return NULL;
   }

   rv = has_ascii_header( fp );
   if( rv < 0 ){
      if( g_opts.debug > 0 ) LNI_FERR(fname,"short header read",hfile);
      znzclose( fp );
      free(hfile);
      return NULL;
   }
   else if ( rv == 1 )  /* process special file type */
   {
     /*
      return nifti2_read_ascii_image( fp, hfile, filesize, read_data );
      */
      znzclose(fp) ;
      free(hfile);
      return NULL;
   }

   /* else, just process normally */

   /**- read binary header */

   ii = (int)znzread( &nhdr , 1 , sizeof(nhdr) , fp ) ;       /* read the thing */

   /* keep file open so we can check for exts. after nifti_convert_nhdr2nim() */

   if( ii < (int) sizeof(nhdr) ){
      if( g_opts.debug > 0 ){
         LNI_FERR(fname,"bad binary header read for file", hfile);
         fprintf(stderr,"  - read %d of %d bytes\n",ii, (int)sizeof(nhdr));
      }
      znzclose(fp) ;
      free(hfile);
      return NULL;
   }

   /* create output image struct and set it up */

   /**- convert all nhdr fields to nifti_image fields */
   nim = nifti2_convert_nhdr2nim(nhdr,hfile);

   if( nim == NULL ){
      znzclose( fp ) ;                                   /* close the file */
      if( g_opts.debug > 0 )
         LNI_FERR(fname,"cannot create nifti image from header",hfile);
      free(hfile); /* had to save this for debug message */
      return NULL;
   }

   if( g_opts.debug > 3 ){
      fprintf(stderr,"+d nifti_image_read(), have nifti image:\n");
      if( g_opts.debug > 2 ) nifti_image_infodump(nim);
   }

   /**- check for extensions (any errors here means no extensions) */
   if( NIFTI_ONEFILE(nhdr) ) remaining = nim->iname_offset - sizeof(nhdr);
   else                      remaining = filesize - sizeof(nhdr);

   (void)nifti2_read_extensions(nim, fp, remaining);

   znzclose( fp ) ;                                      /* close the file */
   free(hfile);

   /**- read the data if desired, then bug out */
   if( read_data ){
      if( nifti_image_load( nim ) < 0 ){
         nifti_image_free(nim);          /* take ball, go home. */
         return NULL;
      }
   }
   else nim->data = NULL ;

   return nim ;
}


int is_nifti2_file( const char *hname )
{
   struct nifti_2_header nhdr ;
   znzFile fp ;
   int ii ;
   char *tmpname;

   /* bad input name? */

   if( !nifti_validfilename(hname) ) return -1 ;

   /* open file */

   tmpname = nifti_findhdrname(hname);
   if( tmpname == NULL ){
      if( g_opts.debug > 0 )
         fprintf(stderr,"** no header file found for '%s'\n",hname);
      return -1;
   }
   fp = znzopen( tmpname , "rb" , nifti_is_gzfile(tmpname) ) ;
   free(tmpname);
   if (znz_isnull(fp))                      return -1 ;  /* bad open? */

   /* read header, close file */

   ii = (int)znzread( &nhdr , 1 , sizeof(nhdr) , fp ) ;
   znzclose( fp ) ;
   if( ii < (int) sizeof(nhdr) )               return -1 ;  /* bad read? */

   /* check for NIFTI-ness */

   if( NIFTI_VERSION(nhdr) != 0 ){
     return ( NIFTI_ONEFILE(nhdr) ) ? 1 : 2 ;
   }

   /* check for ANALYZE-ness (sizeof_hdr field == 348) */

   ii = nhdr.sizeof_hdr ;
   if( ii == (int)sizeof(nhdr) ) return 0 ;  /* matches */

   /* try byte-swapping header */

   swap_4(ii) ;
   if( ii == (int)sizeof(nhdr) ) return 0 ;  /* matches */

   return -1 ;                          /* not good */
}


static int nifti2_read_extensions( nifti_image *nim, znzFile fp, int remain )
{
   nifti1_extender    extdr;      /* defines extension existence  */
   nifti1_extension   extn;       /* single extension to process  */
   nifti1_extension * Elist;      /* list of processed extensions */
   int                posn, count;

   if( !nim || znz_isnull(fp) ) {
      if( g_opts.debug > 0 )
         fprintf(stderr,"** nifti2_read_extensions: bad inputs (%p,%p)\n",
                 (void *)nim, (void *)fp);
      return -1;
   }

   posn = znztell(fp);

   if( (posn != sizeof(nifti_2_header)) &&
       (nim->nifti_type != NIFTI_FTYPE_ASCII) )
      fprintf(stderr,"** WARNING: posn not header size (%d, %d)\n",
              posn, (int)sizeof(nifti_1_header));

   if( g_opts.debug > 2 )
      fprintf(stderr,"-d nre: posn = %d, offset = %d, type = %d, remain = %d\n",
              posn, nim->iname_offset, nim->nifti_type, remain);

   if( remain < 16 ){
      if( g_opts.debug > 2 ){
         if( g_opts.skip_blank_ext )
            fprintf(stderr,"-d no extender in '%s' is okay, as "
                           "skip_blank_ext is set\n",nim->fname);
         else
            fprintf(stderr,"-d remain=%d, no space for extensions\n",remain);
      }
      return 0;
   }

   count = (int)znzread( extdr.extension, 1, 4, fp ); /* get extender */

   if( count < 4 ){
      if( g_opts.debug > 1 )
         fprintf(stderr,"-d file '%s' is too short for an extender\n",
                 nim->fname);
      return 0;
   }

   if( extdr.extension[0] != 1 ){
      if( g_opts.debug > 2 )
         fprintf(stderr,"-d extender[0] (%d) shows no extensions for '%s'\n",
                 extdr.extension[0], nim->fname);
      return 0;
   }

   remain -= 4;
   if( g_opts.debug > 2 )
      fprintf(stderr,"-d found valid 4-byte extender, remain = %d\n", remain);

   /* so we expect extensions, but have no idea of how many there may be */

   count = 0;
   Elist = NULL;
   while (nifti_read_next_extension(&extn, nim, remain, fp) > 0)
   {
      if( nifti_add_exten_to_list(&extn, &Elist, count+1) < 0 ){
         if( g_opts.debug > 0 )
            fprintf(stderr,"** failed adding ext %d to list\n", count);
         return -1;
      }

      /* we have a new extension */
      if( g_opts.debug > 1 ){
         fprintf(stderr,"+d found extension #%d, code = 0x%x, size = %d\n",
                 count, extn.ecode, extn.esize);
         if( extn.ecode == NIFTI_ECODE_AFNI && g_opts.debug > 2 ) /* ~XML */
            fprintf(stderr,"   AFNI extension: %.*s\n",
                    extn.esize-8,extn.edata);
         else if( extn.ecode == NIFTI_ECODE_COMMENT && g_opts.debug > 2 )
            fprintf(stderr,"   COMMENT extension: %.*s\n",        /* TEXT */
                    extn.esize-8,extn.edata);
      }
      remain -= extn.esize;
      count++;
   }

   if( g_opts.debug > 2 ) fprintf(stderr,"+d found %d extension(s)\n", count);

   nim->num_ext = count;
   nim->ext_list = Elist;

   return count;
}


znzFile nifti2_image_write_hdr_img( nifti_image *nim , int write_data ,
                                          const char* opts )
{
  return nifti2_image_write_hdr_img2(nim,write_data,opts,NULL,NULL);
}


#undef  ERREX
#define ERREX(msg)                                                \
 do{ fprintf(stderr,"** ERROR: nifti2_image_write_hdr_img: %s\n",(msg)) ;  \
     return fp ; } while(0)

znzFile nifti2_image_write_hdr_img2(nifti_image *nim, int write_opts,
               const char * opts, znzFile imgfile, const nifti_brick_list * NBL)
{
   struct nifti_2_header nhdr ;
   znzFile               fp=NULL;
   size_t                ss ;
   int                   write_data, leave_open;
   char                  func[] = { "nifti2_image_write_hdr_img2" };

   write_data = write_opts & 1;  /* just separate the bits now */
   leave_open = write_opts & 2;

   if( ! nim                              ) ERREX("NULL input") ;
   if( ! nifti_validfilename(nim->fname)  ) ERREX("bad fname input") ;
   if( write_data && ! nim->data && ! NBL ) ERREX("no image data") ;

   if( write_data && NBL && ! nifti_NBL_matches_nim(nim, NBL) )
      ERREX("NBL does not match nim");

   nifti2_set_iname_offset(nim);

   if( g_opts.debug > 1 ){
      fprintf(stderr,"-d writing nifti file '%s'...\n", nim->fname);
      if( g_opts.debug > 2 )
         fprintf(stderr,"-d nifti type %d, offset %d\n",
                 nim->nifti_type, nim->iname_offset);
   }

#if 0
   if( nim->nifti_type == NIFTI_FTYPE_ASCII )   /* non-standard case */
      return nifti_write_ascii_image(nim,NBL,opts,write_data,leave_open);
#endif

   nhdr = nifti2_convert_nim2nhdr(nim);    /* create the nifti2_header struct */

   /* if writing to 2 files, make sure iname is set and different from fname */
   if( nim->nifti_type != NIFTI_FTYPE_NIFTI1_1 ){
       if( nim->iname && strcmp(nim->iname,nim->fname) == 0 ){
         free(nim->iname) ; nim->iname = NULL ;
       }
       if( nim->iname == NULL ){ /* then make a new one */
         nim->iname = nifti_makeimgname(nim->fname,nim->nifti_type,0,0);
         if( nim->iname == NULL ) return NULL;
       }
   }

   /* if we have an imgfile and will write the header there, use it */
   if( ! znz_isnull(imgfile) && nim->nifti_type == NIFTI_FTYPE_NIFTI1_1 ){
      if( g_opts.debug > 2 ) fprintf(stderr,"+d using passed file for hdr\n");
      fp = imgfile;
   }
   else {
      if( g_opts.debug > 2 )
         fprintf(stderr,"+d opening output file %s [%s]\n",nim->fname,opts);
      fp = znzopen( nim->fname , opts , nifti_is_gzfile(nim->fname) ) ;
      if( znz_isnull(fp) ){
         LNI_FERR(func,"cannot open output file",nim->fname);
         return fp;
      }
   }

   /* write the header and extensions */

   ss = znzwrite(&nhdr , 1 , sizeof(nhdr) , fp); /* write header */
   if( ss < sizeof(nhdr) ){
      LNI_FERR(func,"bad header write to output file",nim->fname);
      znzclose(fp); return fp;
   }

   /* partial file exists, and errors have been printed, so ignore return */
   if( nim->nifti_type != NIFTI_FTYPE_ANALYZE )
      (void)nifti_write_extensions(fp,nim);

   /* if the header is all we want, we are done */
   if( ! write_data && ! leave_open ){
      if( g_opts.debug > 2 ) fprintf(stderr,"-d header is all we want: done\n");
      znzclose(fp); return(fp);
   }

   if( nim->nifti_type != NIFTI_FTYPE_NIFTI1_1 ){ /* get a new file pointer */
      znzclose(fp);         /* first, close header file */
      if( ! znz_isnull(imgfile) ){
         if(g_opts.debug > 2) fprintf(stderr,"+d using passed file for img\n");
         fp = imgfile;
      }
      else {
         if( g_opts.debug > 2 )
            fprintf(stderr,"+d opening img file '%s'\n", nim->iname);
         fp = znzopen( nim->iname , opts , nifti_is_gzfile(nim->iname) ) ;
         if( znz_isnull(fp) ) ERREX("cannot open image file") ;
      }
   }

   znzseek(fp, nim->iname_offset, SEEK_SET);  /* in any case, seek to offset */

   if( write_data ) nifti_write_all_data(fp,nim,NBL);
   if( ! leave_open ) znzclose(fp);

   return fp;
}


void nifti2_set_iname_offset(nifti_image *nim)
{
   int offset;

   switch( nim->nifti_type ){

     default:  /* writing into 2 files */
       /* we only write files with 0 offset in the 2 file format */
       nim->iname_offset = 0 ;
     break ;

     /* NIFTI-1 single binary file - always update */
     case NIFTI_FTYPE_NIFTI1_1:
       offset = nifti_extension_size(nim)+sizeof(struct nifti_2_header)+4;
       /* be sure offset is aligned to a 16 byte boundary */
       if ( ( offset % 16 ) != 0 )  offset = ((offset + 0xf) & ~0xf);
       if( nim->iname_offset != offset ){
          if( g_opts.debug > 1 )
             fprintf(stderr,"+d changing offset from %d to %d\n",
                  nim->iname_offset, offset);
          nim->iname_offset = offset;
       }
     break ;

     /* non-standard case: NIFTI-1 ASCII header + binary data (single file) */
     case NIFTI_FTYPE_ASCII:
       nim->iname_offset = -1 ;             /* compute offset from filesize */
     break ;
   }
}


