#ifndef CARTOBASE_SMART_WEAKOBJECT_H
#define CARTOBASE_SMART_WEAKOBJECT_H


#include <cartobase/config/cartobase_config.h>
#include <cartobase/smart/weakptr.h>
#include <map>


namespace carto
{

  template <typename T> class weak_shared_ptr;

class WeakObject
{

  typedef weak_ptr<WeakObject> WeakPtr;
  typedef weak_shared_ptr<WeakObject> WeakSharedPtr;

  public:

    template <typename T> void attachWeakPtr( weak_ptr<T>& ) const;
    template <typename T> void detachWeakPtr( weak_ptr<T>& ) const;
    template <typename T> void attachWeakPtr( weak_shared_ptr<T>& ) const;
    template <typename T> void detachWeakPtr( weak_shared_ptr<T>& ) const;
    virtual ~WeakObject();

  protected:

    WeakObject();
    WeakObject( const WeakObject& );
    WeakObject& operator=( const WeakObject& );

  private:

    void attachWeakPtr( WeakPtr* ) const;
    void detachWeakPtr( WeakPtr* ) const;
    void attachWeakPtr( WeakSharedPtr* ) const;
    void detachWeakPtr( WeakSharedPtr* ) const;
    void notifyObervers() const;

    mutable std::map<WeakPtr*, bool> observers;

};


template <typename T>
inline 
void WeakObject::attachWeakPtr( weak_ptr<T>& ptr ) const
{
  attachWeakPtr( reinterpret_cast<WeakPtr*>( &ptr ) );
}


template <typename T>
inline
void WeakObject::detachWeakPtr( weak_ptr<T>& ptr ) const
{
  detachWeakPtr( reinterpret_cast<WeakPtr*>( &ptr ) );
}


template <typename T>
inline
void WeakObject::attachWeakPtr( weak_shared_ptr<T>& ptr ) const
{
  attachWeakPtr( reinterpret_cast<WeakSharedPtr*>( &ptr ) );
}


template <typename T>
inline
void WeakObject::detachWeakPtr( weak_shared_ptr<T>& ptr ) const
{
  detachWeakPtr( reinterpret_cast<WeakSharedPtr*>( &ptr ) );
}


} // namespace carto


#endif
