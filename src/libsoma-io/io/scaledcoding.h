#ifndef SOMAIO_IO_SCALEDCODING_H
#define SOMAIO_IO_SCALEDCODING_H

#include <soma-io/config/soma_config.h>
#include <vector>

namespace soma
{

  /** Checks if a volume can be encoded as 16 bit signed ints with a scale
      factor and optionally an offset. If so, the slope and offset are
      determined and set.
  */
  template <typename T>
  bool canEncodeAsScaledS16( const T* vol, float & slope,
                             float & offset,
                             const std::vector<long> & strides,
                             const std::vector<int> & sizes,
                             bool enableoffset = true,
                             double *maxerr = 0 );

  // declare specializations that actually do something
  template <>
  bool canEncodeAsScaledS16( const float* vol, float & slope,
                             float & offset,
                             const std::vector<long> & strides,
                             const std::vector<int> & sizes,
                             bool enableoffset, double * );
  template <>
  bool canEncodeAsScaledS16( const double* vol, float & slope,
                             float & offset,
                             const std::vector<long> & strides,
                             const std::vector<int> & sizes,
                             bool enableoffset, double * );

  // default implementation always fails
  template <typename T>
  inline bool canEncodeAsScaledS16( const T*, float &,
                                    float &,
                                    const std::vector<long> &,
                                    const std::vector<int> &,
                                    bool, double * )
  {
    return false;
  }

  class ScaledEncodingInfo
  {
    public :
      ScaledEncodingInfo() : 
        _slope(1), _offset(0), _maxerr(0) {}

      virtual ~ScaledEncodingInfo() {}

      double & slope() { return _slope; }
      double & offset() { return _offset; }
      double & maxerr() { return _maxerr; }

    private :
      double _slope;
      double _offset;
      double _maxerr;
  };

  template <typename INP, typename OUTP>
  class ScaledEncoding
  {
    public :

      /** Get the slope, offset and maximum error to encode a volume of float
          or double using an integer type.
      */
      static ScaledEncodingInfo info( const INP * thing,
                                      const std::vector<long> & strides,
                                      const std::vector<int> & sizes );

      static ScaledEncodingInfo rescale( const INP * in,
                                         const std::vector<long> & strides,
                                         const std::vector<int> & sizes,
                                         OUTP * out );
  };
}

#endif



