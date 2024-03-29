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

#include <algorithm>
#include <cstdlib>
#include <cartobase/type/limits.h>
#include <soma-io/io/scaledcoding.h>
#include <cartobase/containers/nditerator.h>
#include <set>
#include <cmath>
#include <iostream>
#include <iterator>

using namespace soma;
using namespace carto;
using namespace std;

namespace
{

template <typename T>
bool canencode( const T * thing, float & slope,
                float & offset,
                const std::vector<long> & strides,
                const std::vector<int> & sizes,
                bool enableoffset, double *maxerr )
{
  using std::isnan;
  using std::isinf;

  const bool debug = false;

  int x, dx = sizes[0], dy = sizes[1],
    dz = sizes[2], dt = sizes[3];
  if( !thing || dx == 0 || dy == 0 || dz == 0 || dt == 0 )
  {
    slope = 0.;
    offset = 0.;
    if( maxerr )
      *maxerr = 0.;
    return false; // because we don't know the values yet.
  }
  T       val, vmin = thing[0], vmax = thing[0];
  typedef std::set<double> hset;
  hset    values;
  double  maxm = 0;
  double  off = 0, intv = 1;
  if( maxerr )
    *maxerr = 0;

  if( !enableoffset )
    values.insert( 0 ); // 0 must be a valid value if no offset is allowed
  const_line_NDIterator<T> it( thing, sizes, strides );
  for( ; !it.ended(); ++it )
  {
    const T* p = &*it;
    for( const T * pp = p + it.line_length(); p != pp; it.inc_line_ptr(p) )
    {
      val = *p;
      if( isnan( val ) || isinf( val ) )
        return false;
      if( val < vmin )
        vmin = val;
      if( val > vmax )
        vmax = val;
      values.insert( val );
      if( values.size() > 65536 )
        return false;
    }
  }

  if(debug) {
    clog << "Set of values: ";
    ostream_iterator<hset::value_type> out_it(clog, ", ");
    copy(values.begin(), values.end(), out_it);
  }

  if( !enableoffset )
  {
    if( vmin > 0 )
      vmin = 0;
    if( vmax < 0 )
      vmax = 0;
  }
  if( values.size() < 2 )
  {
    if( enableoffset )
    {
      slope = 1.;
      offset = *values.begin();
    }
    else
    {
      slope = *values.begin();
      offset = 0;
    }
    return true;
  }

  // find minimum interval
  std::set<double>::iterator  iv = values.begin(), ev = values.end();
  double      v, intv2;
  // interval list
  std::set<double>    intvl;
  intv = 0;
  off = *iv;
  for( ++iv; iv!=ev; ++iv )
  {
    v = *iv - off;
    if( intv == 0 || v < intv )
    {
      intv = v;
      intvl.insert( v );
    }
    if( ::fabs( ( v / intv ) - rint( v / intv ) ) > 1e-5 )
      intvl.insert( v );
    off = *iv;
  }


  iv = intvl.begin();
  ev = intvl.end();
  double span = double( vmax - vmin );

  while( iv != ev )
    for( iv=intvl.begin(); iv!=ev; ++iv )
    {
      intv2 = *iv;
      if( span / intv2 >= 65535.1 )
      {
        if( span / intv2 <= 65536.5 )
          intv2 = span / 65535.; // fix rounding error
        else
          // no hope
          return false;
      }
      v = ::fabs( intv2 - rint( intv2 / intv ) * intv );
      if( v > 0 ) //intv * 1e-3 )
      {
        if( span / v < 65536.5 )
        {
          if( span / v > 65535. )
            v = span / 65535.; // fix rounding error
          intv = v;     // use smaller interval
          break;
        }
      }
    }
  intv2 = rint( span / intv );
  if( intv2 > 65535. )
    intv2 = 65535.;
  intv = span / intv2;

  for( iv=intvl.begin(); iv!=ev; ++iv )
  {
    v = ::fabs( *iv / intv - rint( *iv / intv ) );
    if( v > maxm )
    {
      maxm = v;
      if( v > 1e-2 )
        return false;
    }
  }
  slope = float( intv );
  double nmax = (vmax - vmin) / slope;
  double doffset = std::min( 32768., floor(nmax/2) ) * double(slope) + vmin;
  // check float overflow (scale/offsets are in float32 in nifti)
  if( doffset < -numeric_limits<float>::max()
    || doffset > numeric_limits<float>::max() )
    // we could probably do better, trying not to use value -32767
    // but at least the volume will be correctly saved as float.
  {
    return false;
  }
  offset = float( doffset );
  if( maxerr )
    *maxerr = maxm;
  return true;
}

}

namespace soma
{
  using std::isnan;
  using std::isinf;

  template <>
  bool canEncodeAsScaledS16( const float * vol, float & slope,
                             float & offset,
                             const std::vector<long> & strides,
                             const std::vector<int> & sizes,
                             bool enableoffset,
                             double *maxerr )
  {
    return canencode( vol, slope, offset, strides, sizes, enableoffset,
                      maxerr );
  }


  template <>
  bool canEncodeAsScaledS16( const double * vol, float & slope,
                             float & offset,
                             const std::vector<long> & strides,
                             const std::vector<int> & sizes,
                             bool enableoffset,
                             double *maxerr )
  {
    return canencode( vol, slope, offset, strides, sizes, enableoffset,
                      maxerr );
  }


  template <typename INP, typename OUTP>
  ScaledEncodingInfo ScaledEncoding<INP, OUTP>::info(
    const INP * thing,
    const std::vector<long> & strides,
    const std::vector<int> & sizes )
  {
    ScaledEncodingInfo sei;
    double slope, offset, maxerr = 0;
    int y, z, x, f, dx = sizes[0], dy = sizes[1],
      dz = sizes[2], dt = sizes[3];
    long sx = strides[0], sy = strides[1], sz = strides[2], st = strides[3];
    INP val, vmin = thing[0], vmax = thing[0];

    double minlim = numeric_limits<OUTP>::min(),
      maxlim = numeric_limits<OUTP>::max();
    double dynsize = maxlim - minlim + 1;
    double minintv = 0, intv, intv2, v1, err;
    bool isexact = true;
    std::set<double> values;
    std::set<double> intvl;

    // Get values to be scaled
    for( f=0; f<dt; ++f )
      for( z=0; z<dz; ++z )
        for( y=0; y<dy; ++y )
          for( x=0; x<dx; ++x )
          {
            val = thing[ x * sx + y * sy + z * sz + f * st ];
            if( isnan( val ) || isinf( val ) )
            {
              maxerr = val;
            }
            else
            {
              if( val < vmin )
                vmin = val;
              if( val > vmax )
                vmax = val;

              if (isexact)
              {
                if (values.size() < dynsize)
                  values.insert( val );
                else
                  isexact = false;
              }
            }
          }

    // Initialize offset and slope
    if( values.size() < 2 )
    {
      if( values.size() == 0 )
      {
        slope = 1.;
        offset = 0;
      }
      else
      {
        slope = 1.;
        offset = *values.begin();
      }
    }
    else
    {
      offset = vmin;

      if (isexact)
      {
        // Find minimum interval of values and not dividable intervals
        std::set<double>::iterator  iv, ev = values.end();

        iv = values.begin();
        v1 = *iv;

        while( ++iv != ev )
        {
          intv = fabs(*iv - v1);
          if( minintv == 0 || intv < minintv )
          {
            minintv = intv;
            intvl.insert( intv );
          }
          else if( fabs( ( intv / minintv ) - rint( intv / minintv ) )
            > numeric_limits<double>::epsilon() )
          {
            intvl.insert( intv );
          }

          v1 = *iv;
        }

        // Subdivide intervals
        iv = intvl.begin();
        ev = intvl.end();

        while( iv != ev )
          for( iv = intvl.begin(); iv != ev; ++iv )
          {
            intv2 = *iv;
            if ( ( ( vmax - vmin ) / intv2) > maxlim )
            {
              // Scaling won't be exact but we can not store more intervals
              // so we stop the loop
              iv = ev;
              isexact = false;
              break;
            }

            // Subdivide the interval
            intv = fabs( intv2 - rint( intv2 / minintv ) * minintv );
            if( intv > 0 )
            {
              if ( ( ( vmax - vmin ) / intv ) < maxlim )
              {
                minintv = intv;     // use smaller interval
                break;
              }
            }
          }
      }

      // Process the slope
      intv = (vmax - vmin);
      if (isexact)
      {
        slope = intv / rint( intv / minintv );
      }
      else
      {
        // We get a simple approximation
        slope = intv / (maxlim - minlim);
      }
    }

    // Find the maximum error
    if ( maxerr == 0 )
    {
      for( f=0; f<dt; ++f )
        for( z=0; z<dz; ++z )
          for( y=0; y<dy; ++y )
            for( x=0; x<dx; ++x )
            {
              val = thing[ x * sx + y * sy + z * sz + f * st ];
              err = fabs( val / slope - rint( val / slope ) );
              if (err > maxerr)
                maxerr = err;
            }
    }

    sei.offset() = offset;
    sei.slope() = slope;
    sei.maxerr() = maxerr;

    return sei;
  }

  template <typename INP, typename OUTP>
  ScaledEncodingInfo ScaledEncoding<INP, OUTP>::rescale(
    const INP * in,
    const std::vector<long> & strides,
    const std::vector<int> & sizes,
    OUTP * out )
  {
    ScaledEncodingInfo sei;
    sei = ScaledEncoding<INP, OUTP>::info( in, strides, sizes );

    int y, z, x, f, dx = sizes[0], dy = sizes[1],
      dz = sizes[2], dt = sizes[3];
    long sx = strides[0], sy = strides[1], sz = strides[2], st = strides[3];
    long offset_p;
    double offset = sei.offset();
    double slope = sei.slope();
    OUTP omin = numeric_limits<OUTP>::min();

    for( f=0; f<dt; ++f )
      for( z=0; z<dz; ++z )
        for( y=0; y<dy; ++y )
          for( x=0; x<dx; ++x )
          {
            offset_p = x * sx + y * sy + z * sz + f * st;
            out[ offset_p ] = OUTP( ( in[ offset_p ] - offset ) / slope )
              + omin;
          }

    return sei;
  }


  // Instantiates ScaledEncoding class
  template class ScaledEncoding<float, int8_t>;
  template class ScaledEncoding<double, int8_t>;
  template class ScaledEncoding<float, uint8_t>;
  template class ScaledEncoding<double, uint8_t>;
  template class ScaledEncoding<float, int16_t>;
  template class ScaledEncoding<double, int16_t>;
  template class ScaledEncoding<float, uint16_t>;
  template class ScaledEncoding<double, uint16_t>;
  template class ScaledEncoding<float, int32_t>;
  template class ScaledEncoding<double, int32_t>;
  template class ScaledEncoding<float, uint32_t>;
  template class ScaledEncoding<double, uint32_t>;
}

