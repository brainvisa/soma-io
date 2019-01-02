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
#include <soma-io/plugin/mincplugin.h>
#include <soma-io/checker/mincformatchecker.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
//--- system -----------------------------------------------------------------
#include <vector>
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

namespace soma
{
  namespace
  {
    bool initminc()
    {
      new MincPlugin;
      return true;
    }
    bool mincinit __attribute__((unused)) = initminc();
  }
}

MincPlugin::MincPlugin() : Plugin()
{
  vector<string>  exts(2);
  exts[0] = "mnc";
  exts[1] = "mnc.gz";
#ifdef MINC_MGH_SUPPORT
  vector<string> exts2(2);
  exts2[0] = "mgh";
  exts2[1] = "mgz";
#endif

  //////////////////////////////////////////////////////////////////////////
  ////                         C H E C K E R                            ////
  //////////////////////////////////////////////////////////////////////////

  DataSourceInfoLoader::registerFormat( "MINC", new MincFormatChecker,
                                        exts );
#ifdef MINC_MGH_SUPPORT
  DataSourceInfoLoader::registerFormat( "FREESURFER-MINC",
                                        new MincFormatChecker, exts2 );
#endif

  SyntaxSet &s = DataSourceInfoLoader::minfSyntax();
  Syntax	&sx = s[ "__generic__" ];

  sx[ "MINC_voxel_size" ] = Semantic( "float_vector" );

  sx[ "MINC_general_transform:inverse_flag" ] = Semantic( "int" );
  sx[ "MINC_general_transform:type" ] = Semantic( "string" );
  sx[ "MINC_general_transform:transform0" ] = Semantic( "float_vector" );
  sx[ "MINC_general_transform:transform1" ] = Semantic( "float_vector" );
  sx[ "MINC_general_transform:transform2" ] = Semantic( "float_vector" );
  sx[ "MINC_general_transform:transform3" ] = Semantic( "float_vector" );

  sx[ "MINC_space_type" ] = Semantic( "string" );

  sx[ "MINC_history" ] = Semantic( "string" );


  // AIMS "patient_id" is used to store MINC "patient:full_name" and not "patient:identification
  sx[ "MINC_patient:varid" ] = Semantic( "string" );
  sx[ "MINC_patient:vartype" ] = Semantic( "string" );
  sx[ "MINC_patient:version" ] = Semantic( "string" );
  //"patient:fullname" --> "patient_id"
  sx[ "MINC_patient:identification" ] = Semantic( "string" );
  sx[ "MINC_patient:birthdate" ] = Semantic( "string" );
  sx[ "MINC_patient:sex" ] = Semantic( "string" );
  sx[ "MINC_patient:weight" ] = Semantic( "float" );


  sx[ "MINC_study:varid" ] = Semantic( "string" );
  sx[ "MINC_study:vartype" ] = Semantic( "string" );
  sx[ "MINC_study:version" ] = Semantic( "string" );
  // Would like to use AIMS "start_time" but don't know how to make correspond this string and the int_vector
  sx[ "MINC_study:start_time" ] = Semantic( "string" );
  //"study:modality" --> "modality"
  sx[ "MINC_study:institution" ] = Semantic( "string" );
  sx[ "MINC_study:station_id" ] = Semantic( "string" );
  sx[ "MINC_study:procedure" ] = Semantic( "string" );
  //"study:study_id" --> "study_id"
  sx[ "MINC_study:acquisition_id" ] = Semantic( "string" );

  sx[ "MINC_acquisition:varid" ] = Semantic( "string" );
  sx[ "MINC_acquisition:vartype" ] = Semantic( "string" );
  sx[ "MINC_acquisition:version" ] = Semantic( "string" );
  sx[ "MINC_acquisition:scanning_sequence" ] = Semantic( "string" );
  //"acquisition:repetition_time" --> "tr"
  //"acquisition:echo_time" --> "te"
  sx[ "MINC_acquisition:inversion_time" ] = Semantic( "float" );
  //"acquisition:flip_angle" --> "flip_angle"
  sx[ "MINC_acquisition:num_averages" ] = Semantic( "float" );
  sx[ "MINC_acquisition:imaging_frequency" ] = Semantic( "float" );
  sx[ "MINC_acquisition:imaged_nucleus" ] = Semantic( "string" );
  sx[ "MINC_acquisition:comments" ] = Semantic( "string" );


  sx[ "MINC_image-min:units" ] = Semantic( "string" );
  sx[ "MINC_image-max:units" ] = Semantic( "string" );

  //From Siemens Sonata
  sx[ "MINC_patient:age" ] = Semantic( "string" );

  sx[ "MINC_study:start_date" ] = Semantic( "string" );
  sx[ "MINC_study:manufacturer" ] = Semantic( "string" );
  sx[ "MINC_study:model" ] = Semantic( "string" );
  sx[ "MINC_study:field_value" ] = Semantic( "float" );
  sx[ "MINC_study:software_version" ] = Semantic( "string" );
  sx[ "MINC_study:serial_no" ] = Semantic( "string" );
  sx[ "MINC_study:performing_physician" ] = Semantic( "string" );
  sx[ "MINC_study:operator" ] = Semantic( "string" );
  sx[ "MINC_study:calibration_date" ] = Semantic( "string" );

  sx[ "MINC_acquisition:acquisition_id" ] = Semantic( "string" );
  sx[ "MINC_acquisition:start_time" ] = Semantic( "string" );
  sx[ "MINC_acquisition:protocol_name" ] = Semantic( "string" );
  sx[ "MINC_acquisition:receive_coil" ] = Semantic( "string" );
  sx[ "MINC_acquisition:transmit_coil" ] = Semantic( "string" );
  sx[ "MINC_acquisition:echo_number" ] = Semantic( "float" );
  sx[ "MINC_acquisition:slice_thickness" ] = Semantic( "float" );
  sx[ "MINC_acquisition:num_slices" ] = Semantic( "float" );
  sx[ "MINC_acquisition:num_dyn_scans" ] = Semantic( "float" );
  sx[ "MINC_acquisition:window_center" ] = Semantic( "float" );
  sx[ "MINC_acquisition:window_width" ] = Semantic( "float" );
  sx[ "MINC_acquisition:num_phase_enc_steps" ] = Semantic( "float" );
  sx[ "MINC_acquisition:percent_sampling" ] = Semantic( "float" );
  sx[ "MINC_acquisition:percent_phase_fov" ] = Semantic( "float" );
  sx[ "MINC_acquisition:pixel_bandwidth" ] = Semantic( "float" );
  sx[ "MINC_acquisition:phase_enc_dir" ] = Semantic( "string" );
  sx[ "MINC_acquisition:SAR" ] = Semantic( "float" );
  sx[ "MINC_acquisition:mr_acq_type" ] = Semantic( "string" );
  sx[ "MINC_acquisition:image_type" ] = Semantic( "string" );
  sx[ "MINC_acquisition:MrProt_dump" ] = Semantic( "string" );

  sx[ "MINC_processing:transformation0-filename" ] = Semantic( "string" );
  sx[ "MINC_processing:transformation0-filedata" ] = Semantic( "string" );

  MincFormatChecker::mincMutex(); // force creating it now.
}


MincPlugin::~MincPlugin()
{
}


string MincPlugin::name() const
{
  return string("MINC SOMA-IO");
}


bool MincPlugin::noop()
{
  return true;
}

