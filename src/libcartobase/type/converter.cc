#include <cartobase/type/converter_d.h>

namespace carto
{

  RescalerInfo::RescalerInfo()
    : usevtypelimits(false),
      vmin( std::numeric_limits<double>::quiet_NaN() ),
      vmax( std::numeric_limits<double>::quiet_NaN() ),
      omin( std::numeric_limits<double>::quiet_NaN() ),
      omax( std::numeric_limits<double>::quiet_NaN() )
  {
  }

  bool RescalerInfo::explicitRescale() const
  {
    return usevtypelimits
      || !std::isnan( vmin )
      || !std::isnan( vmax )
      || !std::isnan( omin )
      || !std::isnan( omax );
  }

  template class DefaultedRescalerInfo<int8_t, int8_t>;
  template class DefaultedRescalerInfo<int8_t, uint8_t>;
  template class DefaultedRescalerInfo<int8_t, int16_t>;
  template class DefaultedRescalerInfo<int8_t, uint16_t>;
  template class DefaultedRescalerInfo<int8_t, int32_t>;
  template class DefaultedRescalerInfo<int8_t, uint32_t>;
  template class DefaultedRescalerInfo<int8_t, int64_t>;
  template class DefaultedRescalerInfo<int8_t, uint64_t>;
  template class DefaultedRescalerInfo<int8_t, float>;
  template class DefaultedRescalerInfo<int8_t, double>;

  template class DefaultedRescalerInfo<uint8_t, int8_t>;
  template class DefaultedRescalerInfo<uint8_t, uint8_t>;
  template class DefaultedRescalerInfo<uint8_t, int16_t>;
  template class DefaultedRescalerInfo<uint8_t, uint16_t>;
  template class DefaultedRescalerInfo<uint8_t, int32_t>;
  template class DefaultedRescalerInfo<uint8_t, uint32_t>;
  template class DefaultedRescalerInfo<uint8_t, int64_t>;
  template class DefaultedRescalerInfo<uint8_t, uint64_t>;
  template class DefaultedRescalerInfo<uint8_t, float>;
  template class DefaultedRescalerInfo<uint8_t, double>;

  template class DefaultedRescalerInfo<int16_t, int8_t>;
  template class DefaultedRescalerInfo<int16_t, uint8_t>;
  template class DefaultedRescalerInfo<int16_t, int16_t>;
  template class DefaultedRescalerInfo<int16_t, uint16_t>;
  template class DefaultedRescalerInfo<int16_t, int32_t>;
  template class DefaultedRescalerInfo<int16_t, uint32_t>;
  template class DefaultedRescalerInfo<int16_t, int64_t>;
  template class DefaultedRescalerInfo<int16_t, uint64_t>;
  template class DefaultedRescalerInfo<int16_t, float>;
  template class DefaultedRescalerInfo<int16_t, double>;

  template class DefaultedRescalerInfo<uint16_t, int8_t>;
  template class DefaultedRescalerInfo<uint16_t, uint8_t>;
  template class DefaultedRescalerInfo<uint16_t, int16_t>;
  template class DefaultedRescalerInfo<uint16_t, uint16_t>;
  template class DefaultedRescalerInfo<uint16_t, int32_t>;
  template class DefaultedRescalerInfo<uint16_t, uint32_t>;
  template class DefaultedRescalerInfo<uint16_t, int64_t>;
  template class DefaultedRescalerInfo<uint16_t, uint64_t>;
  template class DefaultedRescalerInfo<uint16_t, float>;
  template class DefaultedRescalerInfo<uint16_t, double>;

  template class DefaultedRescalerInfo<int32_t, int8_t>;
  template class DefaultedRescalerInfo<int32_t, uint8_t>;
  template class DefaultedRescalerInfo<int32_t, int16_t>;
  template class DefaultedRescalerInfo<int32_t, uint16_t>;
  template class DefaultedRescalerInfo<int32_t, int32_t>;
  template class DefaultedRescalerInfo<int32_t, uint32_t>;
  template class DefaultedRescalerInfo<int32_t, int64_t>;
  template class DefaultedRescalerInfo<int32_t, uint64_t>;
  template class DefaultedRescalerInfo<int32_t, float>;
  template class DefaultedRescalerInfo<int32_t, double>;

  template class DefaultedRescalerInfo<uint32_t, int8_t>;
  template class DefaultedRescalerInfo<uint32_t, uint8_t>;
  template class DefaultedRescalerInfo<uint32_t, int16_t>;
  template class DefaultedRescalerInfo<uint32_t, uint16_t>;
  template class DefaultedRescalerInfo<uint32_t, int32_t>;
  template class DefaultedRescalerInfo<uint32_t, uint32_t>;
  template class DefaultedRescalerInfo<uint32_t, int64_t>;
  template class DefaultedRescalerInfo<uint32_t, uint64_t>;
  template class DefaultedRescalerInfo<uint32_t, float>;
  template class DefaultedRescalerInfo<uint32_t, double>;

  template class DefaultedRescalerInfo<int64_t, int8_t>;
  template class DefaultedRescalerInfo<int64_t, uint8_t>;
  template class DefaultedRescalerInfo<int64_t, int16_t>;
  template class DefaultedRescalerInfo<int64_t, uint16_t>;
  template class DefaultedRescalerInfo<int64_t, int32_t>;
  template class DefaultedRescalerInfo<int64_t, uint32_t>;
  template class DefaultedRescalerInfo<int64_t, int64_t>;
  template class DefaultedRescalerInfo<int64_t, uint64_t>;
  template class DefaultedRescalerInfo<int64_t, float>;
  template class DefaultedRescalerInfo<int64_t, double>;

  template class DefaultedRescalerInfo<uint64_t, int8_t>;
  template class DefaultedRescalerInfo<uint64_t, uint8_t>;
  template class DefaultedRescalerInfo<uint64_t, int16_t>;
  template class DefaultedRescalerInfo<uint64_t, uint16_t>;
  template class DefaultedRescalerInfo<uint64_t, int32_t>;
  template class DefaultedRescalerInfo<uint64_t, uint32_t>;
  template class DefaultedRescalerInfo<uint64_t, int64_t>;
  template class DefaultedRescalerInfo<uint64_t, uint64_t>;
  template class DefaultedRescalerInfo<uint64_t, float>;
  template class DefaultedRescalerInfo<uint64_t, double>;

  template class DefaultedRescalerInfo<float, int8_t>;
  template class DefaultedRescalerInfo<float, uint8_t>;
  template class DefaultedRescalerInfo<float, int16_t>;
  template class DefaultedRescalerInfo<float, uint16_t>;
  template class DefaultedRescalerInfo<float, int32_t>;
  template class DefaultedRescalerInfo<float, uint32_t>;
  template class DefaultedRescalerInfo<float, int64_t>;
  template class DefaultedRescalerInfo<float, uint64_t>;
  template class DefaultedRescalerInfo<float, float>;
  template class DefaultedRescalerInfo<float, double>;

  template class DefaultedRescalerInfo<double, int8_t>;
  template class DefaultedRescalerInfo<double, uint8_t>;
  template class DefaultedRescalerInfo<double, int16_t>;
  template class DefaultedRescalerInfo<double, uint16_t>;
  template class DefaultedRescalerInfo<double, int32_t>;
  template class DefaultedRescalerInfo<double, uint32_t>;
  template class DefaultedRescalerInfo<double, int64_t>;
  template class DefaultedRescalerInfo<double, uint64_t>;
  template class DefaultedRescalerInfo<double, float>;
  template class DefaultedRescalerInfo<double, double>;

  template class DefaultedRescalerInfo<float, char>; //for command AimsDynamicImageSegmentation of shfjprocessing library
  template class DefaultedRescalerInfo<char, float>; //for command AimsDynamicImageSegmentation of shfjprocessing library

  // These three specializations seem to be needed in order to compile
  // the AimsFileConvert command.
  template class DefaultedRescalerInfo<char, short>;

#ifdef CARTO_LONG_IS_DISTINCT
  template class DefaultedRescalerInfo<unsigned long, short>;
  template class DefaultedRescalerInfo<long, short>;
#endif

}
