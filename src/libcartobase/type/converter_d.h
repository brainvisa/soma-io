#ifndef CARTOBASE_TYPE_CONVERTER_D_H
#define CARTOBASE_TYPE_CONVERTER_D_H

#include <cartobase/type/converter.h>

namespace carto
{

  template<typename INP, typename OUTP>
  DefaultedRescalerInfo<INP,OUTP>::DefaultedRescalerInfo()
  {
    // Initialize scale information
    _defaultedvmin = carto::getcheckedmin<INP>(
        std::numeric_limits<double>::quiet_NaN());
    _defaultedvmax = carto::getcheckedmax<INP>(
        std::numeric_limits<double>::quiet_NaN());
    _defaultedomin = carto::getcheckedmin<OUTP>(
        std::numeric_limits<double>::quiet_NaN());
    _defaultedomax = carto::getcheckedmax<OUTP>(
        std::numeric_limits<double>::quiet_NaN());

    _scale = this->getscale();
  }
    
  template<typename INP, typename OUTP>
  DefaultedRescalerInfo<INP,OUTP>::DefaultedRescalerInfo( const RescalerInfo & info )
  {
    // Initialize scale information
    _defaultedvmin = carto::getcheckedmin<INP>( info.vmin );
    _defaultedvmax = carto::getcheckedmax<INP>( info.vmax );
    _defaultedomin = carto::getcheckedmin<OUTP>( info.omin );
    _defaultedomax = carto::getcheckedmax<OUTP>( info.omax );

    _scale = this->getscale();
  }

  template<typename INP, typename OUTP>
  double DefaultedRescalerInfo<INP,OUTP>::getscale() const {
    double vdiff, odiff;
    vdiff = static_cast<double>(this->_defaultedvmax)
      - static_cast<double>(this->_defaultedvmin);
    vdiff = ( vdiff != 0 ? vdiff : 1 );
    odiff = static_cast<double>(this->_defaultedomax)
      - static_cast<double>(this->_defaultedomin);

    return odiff / vdiff ;
  }

}

#endif
