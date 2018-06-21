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


#define DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION_SINGLE_CHANNEL( T, IS_SCALAR, LONG, HAS_BOOL_CONVERSION ) \
template <> \
struct DataTypeTraits< T > \
{ \
  public: \
    typedef T ChannelType; \
    typedef LONG LongType; \
    \
    static const bool is_scalar = IS_SCALAR; \
    static const bool is_multichannel = false; \
    static const bool has_bool_conversion = HAS_BOOL_CONVERSION; \
    static const unsigned int channelcount = 1; \
}; \

#define DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION_MULTI_CHANNEL( T, IS_SCALAR, LONG, HAS_BOOL_CONVERSION ) \
template <> \
struct DataTypeTraits< T > \
{ \
  public: \
    typedef T::ChannelType ChannelType; \
    typedef LONG LongType; \
    \
    static const bool is_scalar = false; \
    static const bool is_multichannel = true; \
    static const bool has_bool_conversion = HAS_BOOL_CONVERSION; \
    static const unsigned int channelcount = T::channelcount; \
}; \

#define DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( T, IS_SCALAR, IS_MULTICHANNEL, LONG, HAS_BOOL_CONVERSION ) \
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION_##IS_MULTICHANNEL(T, IS_SCALAR, LONG, HAS_BOOL_CONVERSION) \

#define DATA_TYPE_TRAITS_INSTANCIATE_SPECIALIZATION( T ) \
  const bool carto::DataTypeTraits< T >::is_scalar; \
  const bool carto::DataTypeTraits< T >::is_multichannel; \
  const bool carto::DataTypeTraits< T >::has_bool_conversion; \
  const unsigned int carto::DataTypeTraits< T >::channelcount; \

#define DATA_TYPE_TRAITS_INSTANCIATE_TEMPLATE_SPECIALIZATION( T ) \
  template<> const bool carto::DataTypeTraits< T >::is_scalar; \
  template<> const bool carto::DataTypeTraits< T >::is_multichannel; \
  template<> const bool carto::DataTypeTraits< T >::has_bool_conversion; \
  template<> const unsigned int carto::DataTypeTraits< T >::channelcount; \


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
      typedef DataType LongType;

      static const bool is_scalar = false;
      static const bool is_multichannel = false;
      static const bool has_bool_conversion = false;
      static const unsigned int channelcount = 0;
  };

  /// DataTypeTraits base types specialization
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( bool, true, SINGLE_CHANNEL, uintmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( uint8_t, true, SINGLE_CHANNEL, uintmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( int8_t, true, SINGLE_CHANNEL, intmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( uint16_t, true, SINGLE_CHANNEL, uintmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( int16_t, true, SINGLE_CHANNEL, intmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( uint32_t, true, SINGLE_CHANNEL, uintmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( int32_t, true, SINGLE_CHANNEL, intmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( unsigned long, true, SINGLE_CHANNEL, uintmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( long, true, SINGLE_CHANNEL, intmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( unsigned long long, true, SINGLE_CHANNEL, uintmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( long long, true, SINGLE_CHANNEL, intmax_t, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( float, true, SINGLE_CHANNEL, double, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( double, true, SINGLE_CHANNEL, double, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( cfloat, false, SINGLE_CHANNEL, cdouble, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( cdouble, false, SINGLE_CHANNEL, cdouble, true )

  /// DataTypeTraits carto::VoxelValue specialization
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( carto::VoxelRGB, false, MULTI_CHANNEL, carto::VoxelRGB, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( carto::VoxelRGBA, false, MULTI_CHANNEL, carto::VoxelRGBA, true )
  DATA_TYPE_TRAITS_DECLARE_SPECIALIZATION( carto::VoxelHSV, false, MULTI_CHANNEL, carto::VoxelHSV, true )

}


#endif
