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

#ifndef SOMAIO_UTILITIES_MULTIFILEFORMAT_H
#define SOMAIO_UTILITIES_MULTIFILEFORMAT_H

//--- soma --------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//--- std --------------------------------------------------------------
#include <string>
#include <vector>
//----------------------------------------------------------------------------
namespace soma {

  struct MultiFileFormatInfo {
    
    MultiFileFormatInfo();
      
    
    enum Type { Single, Slice, SliceTime, Time };

    uint32_t slicemin, slicemax;
    uint32_t timemin, timemax;
    std::string directory;
    std::string pattern;
    std::string format;
    MultiFileFormatInfo::Type type;
    bool initialized;
  };

  class MultiFileFormat
  {
    public:
      
      MultiFileFormat();
      virtual ~MultiFileFormat();
      
      /// Get MultiFileFormatInfo to search files from a given filename
      static MultiFileFormatInfo info( const std::string & filename );
      
      /// Update dimensions using a MultiFileFormatInfo
      static void updateDimensions( const MultiFileFormatInfo & info,
                                    std::vector<int> & dims );
                                    
      /// Get formatted file name using a MultiFileFormatInfo
      static std::string filename( const MultiFileFormatInfo & info, 
                                   uint32_t slice, 
                                   uint32_t time = 1 );
                   
      /// Get the filenames associated to a MultiFileFormatInfo
      static std::vector<std::string> filenames( const MultiFileFormatInfo & info );
      static std::vector<std::string> filenames( const MultiFileFormatInfo & info,
                                                 uint32_t slicemin, uint32_t slicemax,
                                                 uint32_t timemin, uint32_t timemax );
                                          
      static const std::string FILENAME_SLICE_REGEX;
      static const std::string FILENAME_TIME_SLICE_REGEX;
      static const std::string FILENAME_TIME_REGEX;
  };
  
  DECLARE_GENERIC_OBJECT_TYPE( MultiFileFormatInfo )
}

#endif

