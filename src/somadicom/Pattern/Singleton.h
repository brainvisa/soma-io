#ifndef _Singleton_h_
#define _Singleton_h_


namespace soma
{


template < class T >
class Singleton
{

  public:

    static T& getInstance()
    { 

      static T instance;
      return instance;

    }

};


}


#endif
