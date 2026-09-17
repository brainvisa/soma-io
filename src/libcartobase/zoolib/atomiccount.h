
#ifndef CARTOBASE_ZOOLIB_ATOMICCOUNT_H
#define CARTOBASE_ZOOLIB_ATOMICCOUNT_H

#include "zconfigtest.h"
#if defined( ZCONFIG_Compiler ) && defined( ZCONFIG_OS ) \
  && defined( ZCONFIG_Processor )
#include "ZAtomic.h"

namespace boost
{
  namespace detail
  {

    /// fake boost atomic_count class using ZooLib atomic primitives
    class atomic_count
    {
    public:

      explicit atomic_count(long v)
      {
        ZAtomic_Set( &value_, v );
      }

      void operator++()
      {
        ZAtomic_Inc( &value_ );
      }

      long operator--()
      {
        return !ZAtomic_DecAndTest( &value_ );
      }

      operator long() const
      {
        return ZAtomic_Get( &value_ );
      }

      atomic_count & operator=(long v)
      {
        ZAtomic_Set( &value_, v );
      }

    private:

      atomic_count(atomic_count const &);
      atomic_count & operator=(atomic_count const &);

      mutable ZAtomic_t value_;
    };

  }
}

#define CARTO_RCPTR_USE_ZOOLIB

#endif
#endif

