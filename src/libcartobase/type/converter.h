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

#ifndef CARTOBASE_TYPE_CONVERTER_H
#define CARTOBASE_TYPE_CONVERTER_H

#include <cartobase/config/cartobase_config.h>
#include <cartobase/type/types.h>
#include <cartobase/type/limits.h>
#include <cartobase/type/string_conversion.h>

namespace carto
{
  /** Low level rescaler info used for rescaling

      Request linear rescaling from the input range [vmin, vmax] to the output
      range [omin, omax].  These ranges are initialized with Not-a-Number (NaN)
      values, which correspond to the following defaults:

      - NaN in the input range means to use as input range the range of values
        effectively used in the input of the rescaler; \e unless usevtypelimits
        is \c true, in which case the whole range values of accommodated by the
        input type is used.
      - NaN in the output range means to use the whole range of values
        accommodated by the output type.
   */
  class RescalerInfo
  {
  public:
    /// Construct object requesting default rescaling
    RescalerInfo();

    /// Test if any explicit rescaling parameters has been specified
    bool explicitRescale() const;

    /// Use limits of the input type instead of effective input min-max
    ///
    /// Has no effect if vmin and vmax are explicitly specified.
    bool    usevtypelimits;

    double  vmin;
    double  vmax;
    double  omin;
    double  omax;
  };

  /// Low-level raw converter (not rescaling)
  template<typename INP, typename OUTP>
  class RawConverter
  {
  public:
    void convert( const INP &in, OUTP & out ) const;
  };

  // Conversion with Void
  template <typename OUTP>
  class RawConverter<Void, OUTP>
  {
  public:
    void convert( const Void &, OUTP & out ) const
    {
      out = OUTP( 1 );
    }
  };


  template <typename INP>
  class RawConverter<INP, Void>
  {
  public:
    void convert( const INP &, Void & ) const
    {
    }
  };


  template<>
  class RawConverter<Void, Void>
  {
    public:
    void convert( const Void &, Void & ) const
    {
    }
  };

  template<typename INP, typename OUTP>
  class DefaultedRescalerInfo
  {
  public:
    DefaultedRescalerInfo();
    DefaultedRescalerInfo( const RescalerInfo & info );

    double getScale() const
    {
      return _scale;
    }
    OUTP getScaledValue( INP value ) const;

  private:
    double getscale() const;

    INP _defaultedvmin;
    INP _defaultedvmax;
    OUTP _defaultedomin;
    OUTP _defaultedomax;

    double _scale;
  };

  /// Low-level rescaling converter
  template<typename INP, typename OUTP>
  class Rescaler
  {
  public:
    Rescaler();
    Rescaler( const RescalerInfo & info );
    void convert( const INP &in, OUTP & out ) const;

  private:
    DefaultedRescalerInfo<INP, OUTP> _defaulted_info;
  };


  /** This converter is a bit higher-level than RawConverter and Rescaler,
      it switches to the one or the other depending on the bool template
      parameter: still not very user-friendly */
  template<typename INP, typename OUTP, bool MODE>
  class ConverterSwitch;

  /** This allocator is used by the top-level Converter to allocate a new
      object depending on the input: it is responsible for dimensioning
      the output object (ie image dimensions etc) */
  template<typename INP, typename OUTP>
  class ConverterAllocator
  {
  public:
    static OUTP* alloc( const INP &in );
  };


  /// Specialization of rescaling ConverterSwitch
  template<typename INP, typename OUTP>
  class ConverterSwitch<INP,OUTP,true> : public Rescaler<INP,OUTP>
  {
  public:
    ConverterSwitch( const RescalerInfo & info )
      : Rescaler<INP,OUTP>( info ) {}
    ConverterSwitch() : Rescaler<INP,OUTP>() {}
  };


  /** Specialization of non-rescaling ConverterSwitch

      \todo This is not completely satisfactory: this class is instantiated in
      cases where rescaling has been requested, but the rescaling parameters
      are ignored \b silently.  This may be acceptable if no explicit rescaling
      parameters are passed, but the constructor of this class should probably
      throw an exception if explicit rescaling has been requested
      (i.e. RescalerInfo::explicitRescale() returns \c true).
   */
  template<typename INP, typename OUTP>
  class ConverterSwitch<INP,OUTP,false> : public RawConverter<INP,OUTP>
  {
  public:
    ConverterSwitch( const RescalerInfo & )
      : RawConverter<INP,OUTP>() {}
    ConverterSwitch() : RawConverter<INP,OUTP>() {}
  };


  /** Mid-level converter.
      This converter automatically switches to raw or rescaling converter
      depending on the ability to use rescaling on the data types */
  template<typename INP, typename OUTP>
  class SmartConverter
    : public ConverterSwitch<INP,OUTP,
                             std::numeric_limits<INP>::is_specialized
  && std::numeric_limits<OUTP>::is_bounded >
  {
  public:
    SmartConverter( const RescalerInfo & info )
      : ConverterSwitch<INP,OUTP,
                        std::numeric_limits<INP>::is_specialized
                        && std::numeric_limits<OUTP>::is_bounded >( info ) {}
  };


  /// High-level converter, reimplementation of aims::Converter
  template <class INP,class OUTP>
  class Converter
  {
  public :
    Converter( bool rescale = false ) : _rescale( rescale ), _info() {}
    Converter( bool rescale, const RescalerInfo & info )
      : _rescale( rescale ), _info(info) {}
    virtual ~Converter() {}

    /** Converts an INP type data to an OUTP data.

        If rescale mode is on, the conversion uses the parameters provided as a
        RescalerInfo object.  If no such object is provided, it tries to use
        the whole dynamics of the OUTP type (int types).

        In any case, all quantification info will be lost, unless put as a
        scale factor in the header (used for AimsData).
    */
    OUTP* operator () ( const INP &in ) const;
    /** Converts INP into an existing OUTP object. The output object must be
        already allocated with correct size: its contents will be replaced,
        but no reallocation or resizing will take place.
        To actually trigger allocation, use the operator () instead.
    */
    virtual void convert( const INP &in, OUTP & out ) const;

  protected:
    bool _rescale;
    RescalerInfo _info;
  };


  /** ShallowConverter only differs from the "standard" Converter in the
      way that it may make shallow copies of objects if input and output types
      are identical. When INP and OUTP types are different, it is the same as
      Converter.
   */
  template <typename INP, typename OUTP>
  class ShallowConverter : public Converter<INP, OUTP>
  {
  public:
    ShallowConverter( bool rescale = false )
      : Converter<INP, OUTP>( rescale ) {}
    ShallowConverter( bool rescale, RescalerInfo & info )
      : Converter<INP, OUTP>( rescale, info ) {}
  };

  // implementation
  template<typename INP, typename OUTP>
  inline
  Rescaler<INP,OUTP>::Rescaler() : _defaulted_info()
  {
  }

  template<typename INP, typename OUTP>
  inline
  Rescaler<INP,OUTP>::Rescaler(const RescalerInfo & info) :
      _defaulted_info(info)
  {
  }

  template<typename INP, typename OUTP>
  inline
  void Rescaler<INP,OUTP>::convert( const INP &in, OUTP & out ) const
  {
    // TODO: this extremely sub-optimal...
    // (but hopefullty extremely rarely used)
    out = _defaulted_info.getScaledValue(in);
  }


#define CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( INP , OUTP ) \
template<> \
inline \
void RawConverter< INP , OUTP >::convert( const INP &in, OUTP & out ) const \
{ \
   out = static_cast<OUTP>( rint( in ) ); \
}
  // Specialization for integer conversions rawconverter methods
  // In the case of conversion to integer types, we must round values,
  // not only convert.
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( double, int16_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( float, int16_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( double, uint16_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( float, uint16_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( double, int32_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( float, int32_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( double, uint32_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( float, uint32_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( double, int64_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( float, int64_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( double, uint64_t )
  CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT( float, uint64_t )
#undef CARTO_SPECIALIZED_RAWCONVERTER_ROUNDED_CONVERT


  template<typename INP, typename OUTP>
  inline
  void RawConverter<INP,OUTP>::convert( const INP &in, OUTP & out ) const
  {
    out = static_cast<OUTP>( in );
  }

  template<class INP, class OUTP>
  inline OUTP* ConverterAllocator<INP,OUTP>::alloc( const INP & )
  {
    return new OUTP;
  }

  // Converter
  template<class INP, class OUTP>
  inline OUTP* Converter<INP,OUTP>::operator () ( const INP & in ) const
  {
    OUTP    *out = ConverterAllocator<INP,OUTP>::alloc( in );
    convert( in, *out );
    return out;
  }

  template<class INP, class OUTP>
  inline void
  Converter<INP,OUTP>::convert( const INP &in, OUTP & out ) const
  {
    if( _rescale )
      {
        SmartConverter<INP,OUTP> sc(this->_info);
        sc.convert( in, out );
      }
    else
      {
        RawConverter<INP,OUTP> rc;
        rc.convert( in, out );
      }
  }

#if __cplusplus >= 201103L
  template<typename T>
  inline T min_limit()
  {
    // in C++11 we have std::numeric_limits<T>::lowest()
    return std::numeric_limits<T>::lowest();
  }

#else
  namespace internal
  {
    // use a class to get partial specialization
    template <typename T, bool is_min=std::numeric_limits<T>::is_integer>
    class limits_complement
    {
    public:
      static T lowest()
      {
        return std::numeric_limits<T>::min();
      }
    };

    template <typename T>
    class limits_complement<T, false>
    {
    public:
      static T lowest()
      {
        /// Returns the negative minimum value, because for float types
        /// (double and float) the min value is the nearest value to 0
        return 0 - std::numeric_limits<T>::max();
      }
    };
  }

  template<typename T>
  inline T min_limit()
  {
    return internal::limits_complement<T>::lowest();
  }
#endif

  template<typename T>
  inline bool ismin_limit( T value ){
    return ( value == carto::min_limit<T>() );
  }

  template<typename T>
  inline bool ismax_limit( T value ){
    return ( value == std::numeric_limits<T>::max() );
  }

  template<typename TYPE, typename TEST>
  inline bool isvalidvalue( TYPE value ){
    /// Check that value is valid for the test type
    return ( static_cast<TYPE>(carto::min_limit<TEST>()) <= value )
      && ( value <= static_cast<TYPE>(std::numeric_limits<TEST>::max()) );
  }

  template<typename TYPE>
  inline TYPE getcheckedmin( double min ) {
    return ( carto::isvalidvalue<double, TYPE>( min ) ?
             static_cast<TYPE>( min ) :
             carto::min_limit<TYPE>() );
  }

  template<typename TYPE>
  inline TYPE getcheckedmax( double max ) {
    return ( carto::isvalidvalue<double, TYPE>( max ) ?
             static_cast<TYPE>( max ) :
             std::numeric_limits<TYPE>::max() );
  }

  /*
   * DefaultedRescalerInfo methods implementation
   */

  template<typename INP, typename OUTP>
  inline OUTP DefaultedRescalerInfo<INP,OUTP>::getScaledValue( INP value ) const
  {
    OUTP result;
    RawConverter<double, OUTP> doubleconverter;
    double scaledvalue;
    
    scaledvalue = ( value - this->_defaultedvmin ) * _scale
      + this->_defaultedomin;

    if ( scaledvalue < static_cast<double>(carto::min_limit<OUTP>())) {
      result = carto::min_limit<OUTP>();
    } else if ( scaledvalue > static_cast<double>(std::numeric_limits<OUTP>::max()) ) {
      result = std::numeric_limits<OUTP>::max();
    } else {
      doubleconverter.convert( scaledvalue, result );
    }

//     if (value != 0)
//         std::cout << "Scale value " << carto::toString(value) 
//                   << "(" << carto::DataTypeCode<INP>::name() << ")"
//                   << " to " << carto::toString(result) 
//                   << "(" << carto::DataTypeCode<OUTP>::name() << ")"
//                   << "scale is " << carto::toString(_scale) << ")"
//                   << std::endl;
    return result;
  }

  extern template class DefaultedRescalerInfo<int8_t, int8_t>;
  extern template class DefaultedRescalerInfo<int8_t, uint8_t>;
  extern template class DefaultedRescalerInfo<int8_t, int16_t>;
  extern template class DefaultedRescalerInfo<int8_t, uint16_t>;
  extern template class DefaultedRescalerInfo<int8_t, int32_t>;
  extern template class DefaultedRescalerInfo<int8_t, uint32_t>;
  extern template class DefaultedRescalerInfo<int8_t, float>;
  extern template class DefaultedRescalerInfo<int8_t, double>;

  extern template class DefaultedRescalerInfo<uint8_t, int8_t>;
  extern template class DefaultedRescalerInfo<uint8_t, uint8_t>;
  extern template class DefaultedRescalerInfo<uint8_t, int16_t>;
  extern template class DefaultedRescalerInfo<uint8_t, uint16_t>;
  extern template class DefaultedRescalerInfo<uint8_t, int32_t>;
  extern template class DefaultedRescalerInfo<uint8_t, uint32_t>;
  extern template class DefaultedRescalerInfo<uint8_t, float>;
  extern template class DefaultedRescalerInfo<uint8_t, double>;

  extern template class DefaultedRescalerInfo<int16_t, int8_t>;
  extern template class DefaultedRescalerInfo<int16_t, uint8_t>;
  extern template class DefaultedRescalerInfo<int16_t, int16_t>;
  extern template class DefaultedRescalerInfo<int16_t, uint16_t>;
  extern template class DefaultedRescalerInfo<int16_t, int32_t>;
  extern template class DefaultedRescalerInfo<int16_t, uint32_t>;
  extern template class DefaultedRescalerInfo<int16_t, float>;
  extern template class DefaultedRescalerInfo<int16_t, double>;

  extern template class DefaultedRescalerInfo<uint16_t, int8_t>;
  extern template class DefaultedRescalerInfo<uint16_t, uint8_t>;
  extern template class DefaultedRescalerInfo<uint16_t, int16_t>;
  extern template class DefaultedRescalerInfo<uint16_t, uint16_t>;
  extern template class DefaultedRescalerInfo<uint16_t, int32_t>;
  extern template class DefaultedRescalerInfo<uint16_t, uint32_t>;
  extern template class DefaultedRescalerInfo<uint16_t, float>;
  extern template class DefaultedRescalerInfo<uint16_t, double>;

  extern template class DefaultedRescalerInfo<int32_t, int8_t>;
  extern template class DefaultedRescalerInfo<int32_t, uint8_t>;
  extern template class DefaultedRescalerInfo<int32_t, int16_t>;
  extern template class DefaultedRescalerInfo<int32_t, uint16_t>;
  extern template class DefaultedRescalerInfo<int32_t, int32_t>;
  extern template class DefaultedRescalerInfo<int32_t, uint32_t>;
  extern template class DefaultedRescalerInfo<int32_t, float>;
  extern template class DefaultedRescalerInfo<int32_t, double>;

  extern template class DefaultedRescalerInfo<uint32_t, int8_t>;
  extern template class DefaultedRescalerInfo<uint32_t, uint8_t>;
  extern template class DefaultedRescalerInfo<uint32_t, int16_t>;
  extern template class DefaultedRescalerInfo<uint32_t, uint16_t>;
  extern template class DefaultedRescalerInfo<uint32_t, int32_t>;
  extern template class DefaultedRescalerInfo<uint32_t, uint32_t>;
  extern template class DefaultedRescalerInfo<uint32_t, float>;
  extern template class DefaultedRescalerInfo<uint32_t, double>;

  extern template class DefaultedRescalerInfo<float, int8_t>;
  extern template class DefaultedRescalerInfo<float, uint8_t>;
  extern template class DefaultedRescalerInfo<float, int16_t>;
  extern template class DefaultedRescalerInfo<float, uint16_t>;
  extern template class DefaultedRescalerInfo<float, int32_t>;
  extern template class DefaultedRescalerInfo<float, uint32_t>;
  extern template class DefaultedRescalerInfo<float, float>;
  extern template class DefaultedRescalerInfo<float, double>;

  extern template class DefaultedRescalerInfo<double, int8_t>;
  extern template class DefaultedRescalerInfo<double, uint8_t>;
  extern template class DefaultedRescalerInfo<double, int16_t>;
  extern template class DefaultedRescalerInfo<double, uint16_t>;
  extern template class DefaultedRescalerInfo<double, int32_t>;
  extern template class DefaultedRescalerInfo<double, uint32_t>;
  extern template class DefaultedRescalerInfo<double, float>;
  extern template class DefaultedRescalerInfo<double, double>;

  // These three specializations seem to be needed in order to compile
  // the AimsFileConvert command.
  extern template class DefaultedRescalerInfo<char, short>;
  extern template class DefaultedRescalerInfo<unsigned long, short>;
  extern template class DefaultedRescalerInfo<long, short>;
}

#endif
