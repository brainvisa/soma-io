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
#include <soma-io/image/mincimagewriter_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <time.h>
//----------------------------------------------------------------------------

using namespace carto;
using namespace std;

namespace soma
{

  namespace minc_io
  {

    // Reads an AIMS attribute and writes it as a MINC attribute.
    // A MINC attribute is always associated to a MINC variable.
    // If the MINC variable does not exist, it is created. A MINC variable is always associated to a parent variable which must exist and be given as a parameter.
    //
    // Parameters:
    // sx : The AIMS syntax defining the types of the AIMS attributes.
    // mincid : MINC file id.
    // minc_var : Name of the MINC variable.
    // minc_var_type : Type of the MINC variable.
    // minc_var_parent : Name of the parent MINC variable.
    // minc_att : Name of the MINC attribute.
    // aism_att_name : Name of the AIMS attribute.
    //
    // Return value:
    //  0 : success
    // -1 : this AIMS attribute does not exist
    // -2 : MINC parent variable does not exist
    // -3 : wrong attribute type
    int _writeMincAttribute( Object hdr,
                             Syntax &sx, int mincid,
                             const std::string & minc_var,
                             nc_type minc_var_type,
                             const std::string & minc_var_parent,
                             const std::string & minc_att,
                             const std::string & aims_att_name )
    {

      //std::cout << sx[aims_att_name].type << "\n";

      if( !hdr->hasProperty( aims_att_name ) )
      {
        return(-1);
      }

      int varid = ncvarid( mincid, minc_var.c_str() );

      //If the variable does not exist, create it
      if( varid == -1 )
      {
        varid = ncvardef( mincid, minc_var.c_str(), minc_var_type, 0, NULL );
        ncattput( mincid, varid, MIparent, NC_CHAR,
                  minc_var_parent.length() + 1,
                  minc_var_parent.c_str() );

        int parent_varid = ncvarid( mincid, minc_var_parent.c_str() );

        if( parent_varid==-1 )
        {
          return(-2);
        }

        nc_type child_att_datatype;
        int child_att_length;
        int child_attid = ncattinq( mincid, parent_varid, MIchildren,
                                    &child_att_datatype, &child_att_length );

        string children="";
        //std::cout << "Child att : " << child_attid <<"\n";
        //If the children attribute already exist, we need to read its current value to preserve it.
        if( child_attid!=-1 )
        {
          char *cdata;
          cdata = (char*) malloc( (child_att_length + 1) * sizeof(char) );
          miattgetstr( mincid, parent_varid, const_cast<char*>( MIchildren ),
                       child_att_length + 1, cdata );
          children = cdata;
          children = children + "\n";
          free( cdata );
        }
        children = children + minc_var;
        //std::cout << "Children : " <<children <<"\n";
        ncattput( mincid, parent_varid, MIchildren, NC_CHAR,
                  children.length()+1, children.c_str() );
      }

      if( sx[aims_att_name].type == "string" )
      {

        std::string aims_value;

        if( hdr->getProperty( aims_att_name, aims_value ) == false )
        {
          return(-1);
        }
        ncattput( mincid, varid, minc_att.c_str(), NC_CHAR,
                  aims_value.length()+1, aims_value.c_str() );
      }

      else if( sx[aims_att_name].type == "float" )
      {
        float aims_value;

        if( hdr->getProperty( aims_att_name, aims_value ) == false )
        {
          return(-1);
        }
        miattputdbl( mincid, varid, (char*) minc_att.c_str(),
                     (double) aims_value );

      }

      return 0;
    }

    // Read the  AIMS attribute "MINC_history" and store it in the MINC history.
    // In the future, an history attribute might be added to AIMS and then it would be good to use it also to store the MINC history.
    //
    // Parameters:
    // mincid : MINC file id.
    //
    // Return value:
    //  0 : success
    int _writeMincHistory( Object hdr, int mincid, const string & name )
    {
      int varid = NC_GLOBAL;

      std::string history="";

      try
      {
        history = hdr->getProperty( "MINC_history" )->getString();
      }
      catch( ... )
      {
      }

      time_t tod;
      tod = time( NULL );
      char *char_time = ctime( &tod );

      char_time[ strlen( char_time ) - 1 ] = '\0';

      history = history + char_time + ">>> AIMS write MINC file ("
        + name + ")\n";
      miattputstr( mincid, varid, const_cast<char*>( "history" ),
                   (char*) history.c_str() );
      return 0;
    }

  }

  template class MincImageWriter<int8_t>;
  template class MincImageWriter<int16_t>;
  template class MincImageWriter<int32_t>;
  template class MincImageWriter<int64_t>;
  template class MincImageWriter<uint8_t>;
  template class MincImageWriter<uint16_t>;
  template class MincImageWriter<uint32_t>;
  template class MincImageWriter<uint64_t>;
  template class MincImageWriter<float>;
  template class MincImageWriter<double>;

} // namespace soma
