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

#ifndef CARTOBASE_TYPE_DATATYPEINFO_H
#define CARTOBASE_TYPE_DATATYPEINFO_H

//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h>
#include <cartobase/type/voxelvalue.h>
#include <cartobase/type/datatypetraits.h>

/**
  *  \brief DataTypeInfo class is a hint to access data type information
  *
  *  This class allows a program to obtain information about the
  *  representation of a data type (number of channels, depth, ...).
  *
  *  \tparam DataType data type to get information about (uint8_t, VoxelRGB, VoxelRGBA, ...)
  */
template<class DataType>
class DataTypeInfo
{
  public :
    
    typedef carto::DataTypeTraits<DataType> DataTypeTraitsType;
  
  /**
   *  \brief Number of channels for the DataType
   */
    static inline const uint8_t samples();
    
  /**
   *  \brief Depth of channels for the DataType
   */
    static inline const uint8_t depth();
    
  /**
   *  \brief std::string that represents the channel type for the DataType
   */
    static inline const std::string channelType();
};


// Base type definitions
template<class DataType>
inline const uint8_t DataTypeInfo<DataType>::samples()
{ 
  return DataTypeTraitsType::channelcount;
}

template<class DataType>
inline const uint8_t DataTypeInfo<DataType>::depth()
{ 
  return sizeof(DataType) / DataTypeTraitsType::channelcount;
}

template<class DataType>
inline const std::string DataTypeInfo<DataType>::channelType()
{ 
  return carto::DataTypeCode<typename DataTypeTraitsType::ChannelType>::dataType();
}

#endif
