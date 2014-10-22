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

#ifndef SOMAIO_NIFTILIB_DEFINES_H
#define SOMAIO_NIFTILIB_DEFINES_H

/* These macros are used to replace all niftilib (nifti-1) symbols by a
   prefixed version (somanifti_*).

   niftilib_defines.h should always be included before nifti1_io.h or nifti1.h
   It is already included in nifti2_io.h, which also includes nifti1 headers,
   so normally including ony nifti2_io.h is enough for all cases.

   Why ?

   In somanifti, the niftilib is builtin, the source files are compiled within
   somanifti. But in some cases another version of niftilib may be loaded in
   memory: for instance via a python module nifti, which links against
   niftilib.
   In such a case, the same symbols (functions, structures, and global
   variables) are duplicated with the same names. Then calls to niftilib
   functions may call the somaio lib, or the other one, whitout control, more
   or less at random, and may produce strange results because:
   - both niftilibs may differ, either because they are not the same version,
     or anyway because we made a few fixes in our copy
   - some global static variables (nifti_global_options g_opts for instance)
     get duplicated, may me modified independently and do not contain the
     same value. It was actually this weird behaviour that made us aware of
     the problem of duplicated lib in some cases.
   To overcome this problem, we try here to change all symbol names, so that
   they do not risk being duplicated with the same name, and there are no
   ambiguities any longer.

   TODO: znzlib symbols have not been changed yet.
*/

#define nifti_1_header somanifti_nifti_1_header
#define nifti1_extender somanifti_nifti1_extender
#define nifti1_extension somanifti_nifti1_extension
#define mat44 somanifti_mat44
#define mat33 somanifti_mat33
#define analyze_75_orient_code somanifti_analyze_75_orient_code
#define _analyze75_orient_code _somanifti_analyze75_orient_code
#define nifti_image somanifti_nifti_image
#define nifti_brick_list somanifti_nifti_brick_list
#define nifti_analyze75 somanifti_nifti_analyze75
#define nifti_datatype_string somanifti_nifti_datatype_string
#define nifti_units_string somanifti_nifti_units_string
#define nifti_intent_string somanifti_nifti_intent_string
#define nifti_xform_string somanifti_nifti_xform_string
#define nifti_slice_string somanifti_nifti_slice_string
#define nifti_orientation_string somanifti_nifti_orientation_string
#define nifti_is_inttype somanifti_nifti_is_inttype
#define nifti_mat44_inverse somanifti_nifti_mat44_inverse
#define nifti_mat33_inverse somanifti_nifti_mat33_inverse
#define nifti_mat33_polar somanifti_nifti_mat33_polar
#define nifti_mat33_rownorm somanifti_nifti_mat33_rownorm
#define nifti_mat33_colnorm somanifti_nifti_mat33_colnorm
#define nifti_mat33_determ somanifti_nifti_mat33_determ
#define nifti_mat33_mul somanifti_nifti_mat33_mul
#define nifti_swap_2bytes somanifti_nifti_swap_2bytes
#define nifti_swap_4bytes somanifti_nifti_swap_4bytes
#define nifti_swap_8bytes somanifti_nifti_swap_8bytes
#define nifti_swap_16bytes somanifti_nifti_swap_16bytes
#define nifti_swap_Nbytes somanifti_nifti_swap_Nbytes
#define nifti_datatype_is_valid somanifti_nifti_datatype_is_valid
#define nifti_datatype_from_string somanifti_nifti_datatype_from_string
#define nifti_datatype_to_string somanifti_nifti_datatype_to_string
#define nifti_get_filesize somanifti_nifti_get_filesize
#define swap_nifti_header somanifti_swap_nifti_header
#define old_swap_nifti_header somanifti_old_swap_nifti_header
#define nifti_swap_as_analyze somanifti_nifti_swap_as_analyze
#define nifti_image_read_bricks somanifti_nifti_image_read_bricks
#define nifti_image_load_bricks somanifti_nifti_image_load_bricks
#define nifti_free_NBL somanifti_nifti_free_NBL
#define nifti_image_read somanifti_nifti_image_read
#define nifti_image_load somanifti_nifti_image_load
#define nifti_image_unload somanifti_nifti_image_unload
#define nifti_image_free somanifti_nifti_image_free
#define nifti_read_collapsed_image somanifti_nifti_read_collapsed_image
#define nifti_read_subregion_image somanifti_nifti_read_subregion_image
#define nifti_image_write somanifti_nifti_image_write
#define nifti_image_write_bricks somanifti_nifti_image_write_bricks
#define nifti_image_infodump somanifti_nifti_image_infodump
#define nifti_disp_lib_hist somanifti_nifti_disp_lib_hist
#define nifti_disp_lib_version somanifti_nifti_disp_lib_version
#define nifti_disp_matrix_orient somanifti_nifti_disp_matrix_orient
#define nifti_disp_type_list somanifti_nifti_disp_type_list
#define nifti_image_to_ascii somanifti_nifti_image_to_ascii
#define nifti_image_from_ascii somanifti_nifti_image_from_ascii
#define nifti_get_volsize somanifti_nifti_get_volsize
#define nifti_set_filenames somanifti_nifti_set_filenames
#define nifti_makehdrname somanifti_nifti_makehdrname
#define nifti_makeimgname somanifti_nifti_makeimgname
#define is_nifti_file somanifti_is_nifti_file
#define nifti_find_file_extension somanifti_nifti_find_file_extension
#define nifti_is_complete_filename somanifti_nifti_is_complete_filename
#define nifti_validfilename somanifti_nifti_validfilename
#define disp_nifti_1_header somanifti_disp_nifti_1_header
#define nifti_set_debug_level somanifti_nifti_set_debug_level
#define nifti_set_skip_blank_ext somanifti_nifti_set_skip_blank_ext
#define nifti_set_allow_upper_fext somanifti_nifti_set_allow_upper_fext
#define valid_nifti_brick_list somanifti_valid_nifti_brick_list
#define nifti_image_open somanifti_nifti_image_open
#define nifti_image_write_hdr_img somanifti_nifti_image_write_hdr_img
#define nifti_image_write_hdr_img2 somanifti_nifti_image_write_hdr_img2
#define nifti_read_buffer somanifti_nifti_read_buffer
#define nifti_write_all_data somanifti_nifti_write_all_data
#define nifti_write_buffer somanifti_nifti_write_buffer
#define nifti_read_ascii_image somanifti_nifti_read_ascii_image
#define nifti_write_ascii_image somanifti_nifti_write_ascii_image
#define nifti_datatype_sizes somanifti_nifti_datatype_sizes
#define nifti_mat44_to_quatern somanifti_nifti_mat44_to_quatern
#define nifti_quatern_to_mat44 somanifti_nifti_quatern_to_mat44
#define nifti_make_orthog_mat44 somanifti_nifti_make_orthog_mat44
#define nifti_short_order somanifti_nifti_short_order
#define nifti_mat44_to_orientation somanifti_nifti_mat44_to_orientation
#define nifti_findhdrname somanifti_nifti_findhdrname
#define nifti_findimgname somanifti_nifti_findimgnam
#define nifti_is_gzfile somanifti_nifti_is_gzfile
#define nifti_makebasename somanifti_nifti_makebasename
#define nifti_convert_nim2nhdr somanifti_nifti_convert_nim2nhdr
#define nifti_make_new_header somanifti_nifti_make_new_header
#define nifti_read_header somanifti_nifti_read_header
#define nifti_copy_nim_info somanifti_nifti_copy_nim_info
#define nifti_make_new_nim somanifti_nifti_make_new_nim
#define nifti_simple_init_nim somanifti_nifti_simple_init_nim
#define nifti_convert_nhdr2nim somanifti_nifti_convert_nhdr2nim
#define nifti_hdr_looks_good somanifti_nifti_hdr_looks_good
#define nifti_is_valid_datatype somanifti_nifti_is_valid_datatype
#define nifti_is_valid_ecode somanifti_nifti_is_valid_ecode
#define nifti_nim_is_valid somanifti_nifti_nim_is_valid
#define nifti_nim_has_valid_dims somanifti_nifti_nim_has_valid_dims
#define is_valid_nifti_type somanifti_is_valid_nifti_type
#define nifti_test_datatype_sizes somanifti_nifti_test_datatype_sizes
#define nifti_type_and_names_match somanifti_nifti_type_and_names_match
#define nifti_update_dims_from_array somanifti_nifti_update_dims_from_array
#define nifti_set_iname_offset somanifti_nifti_set_iname_offset
#define nifti_set_type_from_names somanifti_nifti_set_type_from_names
#define nifti_add_extension somanifti_nifti_add_extension
#define nifti_compiled_with_zlib somanifti_nifti_compiled_with_zlib
#define nifti_copy_extensions somanifti_nifti_copy_extensions
#define nifti_free_extensions somanifti_nifti_free_extensions
#define nifti_get_intlist somanifti_nifti_get_intlist
#define nifti_strdup somanifti_nifti_strdup
#define valid_nifti_extensions somanifti_valid_nifti_extensions
#define nifti_global_options somanifti_nifti_global_options
#define nifti_type_ele somanifti_nifti_type_ele

#endif

