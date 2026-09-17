#ifndef CARTOBASE_PATTERN_OBSERVER_H
#define CARTOBASE_PATTERN_OBSERVER_H


namespace carto
{


class Observer
{

  public:

    virtual ~Observer();

    virtual void initialize() = 0;

  protected:

    Observer();

};


}


#endif
