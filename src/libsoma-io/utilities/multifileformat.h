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
    int32_t sel_slice;
    int32_t sel_time;
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
      
      /** Update MultiFileFormatInfo from .minf header info:

          Checks that the header has a list of files and that the input
          filename is in it, otherwise fallbacks to single file mode.
      */
      static void updateInfo( MultiFileFormatInfo & info,
                              carto::Object header, bool force_vol=false );

      /// Update dimensions using a MultiFileFormatInfo
      static void updateDimensions( const MultiFileFormatInfo & info,
                                    std::vector<int> & dims );
                                    
      /// Get formatted file name using a MultiFileFormatInfo
      static std::string filename( const MultiFileFormatInfo & info, 
                                   int32_t slice = -1,
                                   int32_t time = -1 );
                   
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

