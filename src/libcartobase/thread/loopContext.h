
#ifndef CARTOBASE_THREAD_LOOPCONTEXT_H
#define CARTOBASE_THREAD_LOOPCONTEXT_H

#include <cartobase/config/cartobase_config.h>

#ifndef CARTO_NO_THREAD

#include <cartobase/thread/exclusiveContext.h>


namespace carto
{

  class ThreadGauge;
  class ThreadCancel;


  class LoopContext : public ExclusiveContext
  {

  public:

    LoopContext( ThreadGauge* gauge = 0, ThreadCancel* cancel = 0,
                 bool singlethread = false );
    virtual ~LoopContext();

    virtual void doIt( int startIndex, int countIndex ) = 0;
    virtual void gaugeAdd( int value );
    virtual bool cancel();
    virtual void resetGauge();
    virtual void resetCancel();

  protected:

    ThreadGauge* _gauge;
    ThreadCancel* _cancel;
  };

}


#endif
#endif
