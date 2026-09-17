#ifndef AIMS_NO_THREAD

#include <cartobase/thread/loopContext.h>
#include <cartobase/thread/threadGauge.h>
#include <cartobase/thread/threadCancel.h>


carto::LoopContext::LoopContext( carto::ThreadGauge* gauge,
                                 carto::ThreadCancel* cancel,
                                 bool singlethread )
  : carto::ExclusiveContext( singlethread ),
    _gauge( gauge ),
    _cancel( cancel )
{
}


carto::LoopContext::~LoopContext()
{
}


void carto::LoopContext::gaugeAdd( int value )
{

  if ( _gauge )
    _gauge->add( value );

}


bool carto::LoopContext::cancel()
{

  if ( _cancel )
    return _cancel->check();

  return false;

}


void carto::LoopContext::resetGauge()
{

  if ( _gauge )
    _gauge->reset();

}


void carto::LoopContext::resetCancel()
{

  if ( _cancel )
    _cancel->reset();

}

#endif
