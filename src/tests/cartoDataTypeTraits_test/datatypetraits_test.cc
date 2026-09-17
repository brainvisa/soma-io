#include <iostream>
#include <cartobase/exception/assert.h>
#include <cartobase/type/types.h>
#include <cartobase/type/datatypetraits.h>
#include <cartobase/type/datatypeinfo.h>
#include <cartobase/type/string_conversion.h>

template <class VoxelType>
inline void display() {
  std::cout << "--------------------------------------------------" << std::endl
            << "Data type: " << carto::DataTypeCode<VoxelType>::name() << std::endl
            << "Is multichannel: " << carto::toString(carto::DataTypeTraits<VoxelType>::is_multichannel) << std::endl
            << "Channel count: " << carto::toString(carto::DataTypeTraits<VoxelType>::channelcount) << std::endl
            << "Info samples: " << carto::toString(DataTypeInfo<VoxelType>::samples()) << std::endl
            << "Info depth: " << carto::toString(DataTypeInfo<VoxelType>::depth()) << std::endl
            << "Info channel type: " << carto::toString(DataTypeInfo<VoxelType>::channelType()) << std::endl;
}

int main()
{
  // Single channel types
  ASSERT(!carto::DataTypeTraits<uint8_t>::is_multichannel);
  display<uint8_t>();

  ASSERT(!carto::DataTypeTraits<int8_t>::is_multichannel);
  display<int8_t>();

  ASSERT(!carto::DataTypeTraits<uint16_t>::is_multichannel);
  display<uint16_t>();
  
  ASSERT(!carto::DataTypeTraits<int16_t>::is_multichannel);
  display<int16_t>();
  
  ASSERT(!carto::DataTypeTraits<uint32_t>::is_multichannel);
  display<uint32_t>();
  
  ASSERT(!carto::DataTypeTraits<int32_t>::is_multichannel);
  display<int32_t>();
  
  ASSERT(!carto::DataTypeTraits<unsigned long>::is_multichannel);
  display<unsigned long>();
  
  ASSERT(!carto::DataTypeTraits<long>::is_multichannel);
  display<long>();
  
  ASSERT(!carto::DataTypeTraits<unsigned long long>::is_multichannel);
  display<unsigned long long>();
  
  ASSERT(!carto::DataTypeTraits<long long>::is_multichannel);
  display<long long>();
  
  ASSERT(!carto::DataTypeTraits<float>::is_multichannel);
  display<float>();
  
  ASSERT(!carto::DataTypeTraits<double>::is_multichannel);
  display<double>();
  
  // Multi channel types
  ASSERT(carto::DataTypeTraits<carto::VoxelRGB>::is_multichannel);
  display<carto::VoxelRGB>();
  
  ASSERT(carto::DataTypeTraits<carto::VoxelRGBA>::is_multichannel);
  display<carto::VoxelRGBA>();
  
  ASSERT(carto::DataTypeTraits<carto::VoxelHSV>::is_multichannel);
  display<carto::VoxelHSV>();
  
}
