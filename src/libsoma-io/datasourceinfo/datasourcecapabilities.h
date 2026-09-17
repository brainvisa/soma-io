#ifndef SOMAIO_DATASOURCEINFO_DATASOURCECAPABILITIES_H
#define SOMAIO_DATASOURCEINFO_DATASOURCECAPABILITIES_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;

  /// Reading/Writing Capabilities of a FormatReader
  ///
  /// This object is constructed by the FormatChecker after reading the 
  /// header. It allows one to give information about the possible ways to 
  /// read a volume ( partial reading ... ).
  ///
  /// \remarks Since capabilities may be file-dependant (and not just format-
  /// dependent, think of compressed data), they need being stored in the
  /// class. The current implementation uses an integer of chosen size so 
  /// that each capability is written on one bit. If more capabilities must be
  /// added, increase the number of bytes of the integer.
  ///
  /// \see DataSourceInfo
  class DataSourceCapabilities
  {
    public:
      DataSourceCapabilities();
      DataSourceCapabilities( const DataSourceCapabilities & );
      virtual ~DataSourceCapabilities();

      //======================================================================
      //   A C C E S S O R S
      //======================================================================
      bool  allowsMemoryMapping() const;
      bool  isThreadSafe()        const;
      bool  isOrdered()           const;
      bool  canSeekVoxel()  const;
      bool  canSeekLine()   const;
      bool  canSeekSlice()  const;
      bool  canSeekVolume() const;
      bool  isRandomAccessEfficient() const;
      bool  canHandleStrides() const;

      carto::rc_ptr<DataSource>  mappableDataSource() const;

      //======================================================================
      //   M U T A T O R S
      //======================================================================
      void  setMemoryMapping  ( const bool & boo = true );
      void  setThreadSafe     ( const bool & boo = true );
      void  setOrdered        ( const bool & boo = true );
      void  setSeekVoxel      ( const bool & boo = true );
      void  setSeekLine       ( const bool & boo = true );
      void  setSeekSlice      ( const bool & boo = true );
      void  setSeekVolume     ( const bool & boo = true );
      void  setRandomAccessEfficient( const bool & boo = true );
      void  setHandleStrides  ( const bool & boo = true );

      void  setDataSource( const carto::rc_ptr<DataSource> ds );

      //======================================================================
      //   I N I T I A L I Z E D
      //======================================================================
      /// Did we initialize the capabilities ? default : false
      /// Since a DataSourceInfo can be either fully initialized because of a
      /// previous call to DataSourceInfoLoader or be only partially 
      /// initialized because we just know the header, it is important to 
      /// know if the value of this object matters or not.
      /// each time one of the mutators is called, setInit( true ) is also 
      /// called.
      bool  isInit() const;
      void  setInit( const bool & boo = true );
      /// Sets the whole capabilities to its default value (including
      /// isInit() to false )
      void  reset();

    protected:
      /// From LSB to MSB :
      ///  - capabilities_are_init
      ///  - allows_memory_mapping
      ///  - is_thread_safe
      ///  - is_ordered
      ///  - seek_voxel
      ///  - seek_line
      ///  - seek_slice
      ///  - seek_volume
      ///  - is_random_access_efficient
      ///  - handle_strides
      /// Default value is [00000000]
      unsigned int _capabilities;
      /// In a case where memory mapping is allowed, it is necessary to
      /// tell the AllocatorContext which DataSource must be mapped.
      carto::rc_ptr<DataSource>  _mds;
  };

}

#endif
