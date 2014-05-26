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
