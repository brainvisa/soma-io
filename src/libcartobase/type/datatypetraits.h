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

#ifndef CARTOBASE_TYPE_DATATYPETRAITS_H
#define CARTOBASE_TYPE_DATATYPETRAITS_H

//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/type/voxelvalue.h>
#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelhsv.h>

  
#define DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( T ) \
template <> \
struct DataTypeTraits< T > \
{ \
  public: \
    typedef T ChannelType; \
    \
    static const bool is_multichannel = false; \
    static const uint8_t channelcount = 1; \
}; \
  
#define DATA_TYPE_TRAITS_MULTI_CHANNEL_DECLARE_SPECIALIZATION( T ) \
template <> \
struct DataTypeTraits< T > \
{ \
  public: \
    typedef T::ChannelType ChannelType; \
    \
    static const bool is_multichannel = true; \
    static const uint8_t channelcount = T::channelcount; \
}; \

#define DATA_TYPE_TRAITS_INSTANCIATE_SPECIALIZATION( T ) \
  const uint8_t carto::DataTypeTraits< T >::channelcount; \
  const bool carto::DataTypeTraits< T >::is_multichannel; \
  

namespace carto {

  /**
   *  \brief Properties of data types.
   *
   *  This class allows a program to obtain information about the
   *  representation of a data type.
   *
   *  The \c static \c const members are usable as integral constant
   *  expressions.
   *
   * \tparam DataType to get information about.
   */

  template<typename DataType>
  struct DataTypeTraits
  {
    public:
      // Types definition
      typedef Void ChannelType;
      
      static const bool is_multichannel = false;
      static const uint8_t channelcount = 0;
  };

  /// DataTypeTraits base types specialization
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( uint8_t )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( int8_t )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( uint16_t )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( int16_t )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( uint32_t )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( int32_t )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( unsigned long )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( long )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( unsigned long long )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( long long )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( float )
  DATA_TYPE_TRAITS_SINGLE_CHANNEL_DECLARE_SPECIALIZATION( double )

  /// DataTypeTraits carto::VoxelValue specialization
  DATA_TYPE_TRAITS_MULTI_CHANNEL_DECLARE_SPECIALIZATION( carto::VoxelRGB )
  DATA_TYPE_TRAITS_MULTI_CHANNEL_DECLARE_SPECIALIZATION( carto::VoxelRGBA )
  DATA_TYPE_TRAITS_MULTI_CHANNEL_DECLARE_SPECIALIZATION( carto::VoxelHSV )

}

  
#endif
