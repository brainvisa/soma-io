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

//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/formatdictionary.h>
//--- system -----------------------------------------------------------------
#include <string>
#include <map>
#include <set>
#include <vector>
//----------------------------------------------------------------------------

using namespace soma;
using namespace std;

void IOObjectTypesDictionary::registerReadType( const string & datatype, 
                                                FormatInfo info )
{
  readTypes()[ datatype ] = info;
}


void IOObjectTypesDictionary::registerWriteType( const string & datatype, 
                                                 FormatInfo info )
{
  writeTypes()[ datatype ] = info;
}


map<string, IOObjectTypesDictionary::FormatInfo> & 
IOObjectTypesDictionary::readTypes()
{
  static map<string, FormatInfo>	t;
  return t;
}


map<string, IOObjectTypesDictionary::FormatInfo> & 
IOObjectTypesDictionary::writeTypes()
{
  static map<string, FormatInfo>	t;
  return t;
}


bool IOObjectTypesDictionary::hasReadType( const string & datatype )
{
  map<string, IOObjectTypesDictionary::FormatInfo> & t 
    = readTypes();
  map<string, IOObjectTypesDictionary::FormatInfo>::iterator 
    i = t.find( datatype );
  return( i != t.end() );
}


bool IOObjectTypesDictionary::hasWriteType( const string & datatype )
{
  map<string, IOObjectTypesDictionary::FormatInfo> & t 
    = writeTypes();
  map<string, IOObjectTypesDictionary::FormatInfo>::iterator 
    i = t.find( datatype );
  return( i != t.end() );
}

