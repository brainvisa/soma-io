
#ifndef NIFTI2_IO_H
#define NIFTI2_IO_H

#include "nifti1_io.h"
#include "nifti2.h"

/*=================*/
#ifdef  __cplusplus
extern "C" {
#endif
/*=================*/

nifti_image *nifti2_image_read    ( const char *hname , int read_data ) ;

void  swap_nifti2_header ( struct nifti_2_header *h , int is_nifti ) ;
struct nifti_2_header   nifti2_convert_nim2nhdr(const nifti_image* nim);
nifti_2_header * nifti2_read_header(const char *hname, int *swapped, int check);
nifti_image    * nifti2_convert_nhdr2nim(struct nifti_2_header nhdr,
                                        const char * fname);

int    nifti2_hdr_looks_good (const nifti_2_header * hdr);
int    is_nifti2_file      (const char *hname);

znzFile nifti2_image_write_hdr_img(nifti_image *nim, int write_data,
                                  const char* opts);
znzFile nifti2_image_write_hdr_img2( nifti_image *nim , int write_opts ,
               const char* opts, znzFile imgfile, const nifti_brick_list * NBL);
void   nifti2_set_iname_offset      (nifti_image *nim);


/*=================*/
#ifdef  __cplusplus
}
#endif
/*=================*/

#endif

