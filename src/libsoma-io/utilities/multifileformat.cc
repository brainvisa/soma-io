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

#include <cartobase/object/object_d.h>
#include <cartobase/stream/fileutil.h>
#include <cartobase/stream/directory.h>
#include <cartobase/stream/sstream.h>
#include <soma-io/utilities/multifileformat.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>

//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "MULTIFILEFORMAT" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace carto;
using namespace soma;
using namespace std;

const std::string MultiFileFormat::FILENAME_SLICE_REGEX = "^(.*[^0-9])([0-9]+)$";
const std::string MultiFileFormat::FILENAME_TIME_SLICE_REGEX = "^(.*)t([0-9]+)_s([0-9]+)$";
const std::string MultiFileFormat::FILENAME_TIME_REGEX = "^(.*)t([0-9]+)$";

MultiFileFormatInfo::MultiFileFormatInfo() 
  : slicemin(0),
    slicemax(0),
    timemin(0),
    timemax(0),
    initialized(false){
}

MultiFileFormat::MultiFileFormat() {
}


MultiFileFormat::~MultiFileFormat() {
}

MultiFileFormatInfo MultiFileFormat::info( const std::string & filename ) {
  
  string  name = FileUtil::basename( FileUtil::removeExtension( filename ) ),
          ext = FileUtil::extension( filename ),
          dirname = FileUtil::dirname( filename );
  MultiFileFormatInfo info;
  
  info.directory = dirname;
  info.slicemin = info.slicemax = info.timemin = info.timemax = 0;
  
  // Regular expression to find slice and time in filename
  regex_t regst;
  regcomp( &regst, MultiFileFormat::FILENAME_TIME_SLICE_REGEX.c_str(), REG_EXTENDED | REG_ICASE );
  
  // Regular expression to find time number at the end of filename
  regex_t regt;
  regcomp( &regt, MultiFileFormat::FILENAME_TIME_REGEX.c_str(), REG_EXTENDED | REG_ICASE );
  
  // Regular expression to find number at the end of filename
  regex_t regs;
  regcomp( &regs, MultiFileFormat::FILENAME_SLICE_REGEX.c_str(), REG_EXTENDED | REG_ICASE );
  regmatch_t  rmatch[4];

  // Try to find time and slice in filename
  localMsg("regex tested file name: " + name);
  if( !regexec( &regst, name.c_str(), 4, rmatch, 0 ) )
  {
    localMsg("regex slice time ok for file name: " + name);
    unsigned  lens = rmatch[2].rm_eo - rmatch[2].rm_so;
    unsigned  lent = rmatch[3].rm_eo - rmatch[3].rm_so;
    char    num1[10], num2[10];
    sprintf( num1, "%d", lens );
    sprintf( num2, "%d", lent );
    info.pattern = string( "^" ) + name.substr( 0, rmatch[1].rm_eo + 1 ) 
                   + "([0-9]+)_s([0-9]+)\\." + ext + "$";
    info.format = name.substr( 0, rmatch[1].rm_eo + 1 ) + "%0" 
                  + num1 + "u_s%0" + num2 + "u." + ext;
    info.type = MultiFileFormatInfo::SliceTime;
  }
  // Try to find time in filename
  else if( !regexec( &regt, name.c_str(), 3, rmatch, 0 ) )
  {
    localMsg("regex time ok for file name: " + name);
    unsigned  lent = rmatch[2].rm_eo - rmatch[2].rm_so;
    char    num[10];
    sprintf( num, "%u", lent );
    info.pattern = string( "^" ) + name.substr( 0, rmatch[1].rm_eo ) 
                   + "t([0-9]+)\\." + ext + "$";
    info.format = name.substr( 0, rmatch[1].rm_eo ) + "t%0" 
                  + num + "u." + ext;
    info.type = MultiFileFormatInfo::Time;
  }
  // Try to find slice in filename
  else if( !regexec( &regs, name.c_str(), 3, rmatch, 0 ) )
  {
    localMsg("regex slice ok for file name: " + name);
    unsigned  lens = rmatch[2].rm_eo - rmatch[2].rm_so;
    char    num[10];
    sprintf( num, "%u", lens );
    info.pattern = string( "^" ) + name.substr( 0, rmatch[1].rm_eo ) 
                   + "([0-9]+)\\." + ext + "$";
    info.format = name.substr( 0, rmatch[1].rm_eo ) + "%0" 
                  + num + "u." + ext;
    info.type = MultiFileFormatInfo::Slice;
  }
  else
  {
    localMsg("regex no slice or time : " + name);
    info.pattern = name + "." +ext;
    info.format = name + "." +ext;
    info.type = MultiFileFormatInfo::Single;
  }
  
  regfree( &regst );
  regfree( &regt );
  regfree( &regs );
  
  if (info.type != MultiFileFormatInfo::Single)
  {
    // Scan directory for matching files
    Directory dir( dirname );
    set<string> files = dir.files();
    set<string>::iterator is, es = files.end();

    std::string num;
    unsigned  n;
    bool  hast = false, hass = false;
    regcomp( &regst, info.pattern.c_str(), REG_EXTENDED | REG_ICASE );
    
    for( is = files.begin(); is != es; ++is )
    {
      if( !regexec( &regst, is->c_str(), 3, rmatch, 0 ) )
      {
        switch( info.type )
        {
          case MultiFileFormatInfo::SliceTime:
            num = is->substr( rmatch[1].rm_so, rmatch[1].rm_eo );
            sscanf( num.c_str(), "%u", &n );
            if( n < info.timemin || !hast )
              info.timemin = n;
            if( n > info.timemax || !hast )
              info.timemax = n;
            if( !hast )
              hast = true;
              
            num = is->substr( rmatch[2].rm_so, rmatch[2].rm_eo );
            sscanf( num.c_str(), "%u", &n );
            if( n < info.slicemin || !hass )
              info.slicemin = n;
            if( n > info.slicemax || !hass )
              info.slicemax = n;
            if( !hass )
              hass = true;
            
            break;
            
          case MultiFileFormatInfo::Time:
            num = is->substr( rmatch[1].rm_so, rmatch[1].rm_eo );
            sscanf( num.c_str(), "%u", &n );
            if( n < info.timemin || !hast )
              info.timemin = n;
            if( n > info.timemax || !hast )
              info.timemax = n;
            if( !hast )
              hast = true;
            
            break;

          default:
            num = is->substr( rmatch[1].rm_so, rmatch[1].rm_eo );
            sscanf( num.c_str(), "%u", &n );
            //cout << "num: " << n << endl;
            if( n < info.slicemin || !hass )
              info.slicemin = n;
            if( n > info.slicemax || !hass )
              info.slicemax = n;
            if( !hass )
              hass = true;
            
            break;
        }
      }
    }

    regfree( &regst );
  }
  
  info.initialized = true;
  return info;
}

void MultiFileFormat::updateDimensions( const MultiFileFormatInfo & info,
                                        vector<int> & dims ) {
  if (dims.size() < 4)
    dims.resize(4, 1);
  
  switch( info.type )
  {
    case MultiFileFormatInfo::Slice:
      dims[2] = info.slicemax - info.slicemin + 1;
      break;

    case MultiFileFormatInfo::Time:
      dims[3] = info.timemax - info.timemin + 1;
      break;
      
    case MultiFileFormatInfo::SliceTime:
      dims[2] = info.slicemax - info.slicemin + 1;
      dims[3] = info.timemax - info.timemin + 1;
      break;

    case MultiFileFormatInfo::Single:
      break;
  }
}

vector<string> MultiFileFormat::filenames( const MultiFileFormatInfo & info )
{
//   vector<int32_t> dims;
//   MultiFileFormat::updateDimensions(info, dims);

//   std::cout << "info.slicemin :" << carto::toString(info.slicemin) << std::endl
//             << "info.slicemax :" << carto::toString(info.slicemax) << std::endl
//             << "info.timemin :" << carto::toString(info.timemin) << std::endl
//             << "info.timemax :" << carto::toString(info.timemax) << std::endl;

  return MultiFileFormat::filenames( info, 
                                     info.slicemin, info.slicemax,
                                     info.timemin, info.timemax );
}

vector<string> MultiFileFormat::filenames( const MultiFileFormatInfo & info,
                                           uint32_t slicemin, uint32_t slicemax,
                                           uint32_t timemin, uint32_t timemax )
{

  uint32_t dimz = slicemax - slicemin + 1;
  uint32_t dimt = timemax - timemin + 1;
  
  vector<string> filenames;
  filenames.reserve( dimz * dimt );
  
  for( uint32_t t = 0; t < dimt; ++t )
    for( uint32_t z = 0; z < dimz; ++z )
      filenames.push_back( filename(info, z + slicemin, t  + timemin) );
  
  return filenames;
}

string MultiFileFormat::filename( const MultiFileFormatInfo & info, 
                                  uint32_t slice, 
                                  uint32_t time ) {
  string filename;
//   std::cout << "pattern: " << info.pattern << std::endl
//             << "format: " << info.format << std::endl
//             << "type: " << carto::toString(info.type) << std::endl
//             << "time: " << carto::toString(time) << std::endl
//             << "slice: " << carto::toString(slice) << std::endl
//             << "minslice: " << carto::toString(info.slicemin) << std::endl
//             << "mintime: " << carto::toString(info.timemin) << std::endl;
  char *fname;
  switch( info.type )
  {
    case MultiFileFormatInfo::Single:
      filename = info.pattern;
      break;
      
    case MultiFileFormatInfo::Slice:
      fname = new char[ info.format.length() + 1 ];
      sprintf( fname, info.format.c_str(), slice );
      filename = string(fname);
      delete[] fname;
      break;
      
    case MultiFileFormatInfo::SliceTime:
      fname = new char[ info.format.length() + 1 ];
      sprintf( fname, info.format.c_str(), time, slice );
      filename = string(fname);
      delete[] fname;
      break;
      
    case MultiFileFormatInfo::Time:
      fname = new char[ info.format.length() + 1 ];
      sprintf( fname, info.format.c_str(), time );
      filename = string(fname);
      delete[] fname;
      break;
      
    default:
      throw invalid_argument( "MultiFileFormatInfo type is not valid." );
  }
  
  return info.directory + FileUtil::separator() + filename;
}

INSTANTIATE_GENERIC_OBJECT_TYPE( MultiFileFormatInfo )

#undef localMsg
