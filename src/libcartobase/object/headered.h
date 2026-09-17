#ifndef CARTOBASE_OBJECT_HEADERED_H
#define CARTOBASE_OBJECT_HEADERED_H


#include <cartobase/pattern/observable.h>
#include <cartobase/pattern/observer.h>
#include <cartobase/object/property.h>
#include <cartobase/object/propertyfilter.h>
#include <cartobase/type/typetraits.h>
#include <map>
#include <set>


namespace carto
{


class Headered : virtual public RCObject, public Observable, public Observer
{

  public:

    Headered();
    Headered( const Headered& other );
    virtual ~Headered();

    Headered& operator = ( const Headered& other );

    const PropertySet& header() const;
    PropertySet& header();

    void addPropertyFilter( const rc_ptr< PropertyFilter >& propertyFilter );
    bool hasPropertyFilter( const std::string& propertyFilterName ) const;
    bool connect( const std::string& propertyFilterName,
                  const PropertyFilter::Slot& slot );
    bool disconnect( const std::string& propertyFilterName,
                     const PropertyFilter::Slot& slot );

    std::set< rc_ptr< PropertyFilter > >
      getPropertyFilters( const std::string& propertyName ) const;

    void delay();
    void flush();
    void blockSignals( bool );
    bool signalsBlocked() const;

    virtual void initialize();

    /// retrieve or generate an UUID - this method intentionally is non-const
    std::string uuid();
    /** take the UUID from another header and set it to this.
        \return false if the source had no UUID
    */
    bool copyUuid( const PropertySet & );
    bool copyUuid( const Headered & );

  private:

    void slotPropertyChanged( const Object& sender, 
                              const std::string& propertyName,
                              const Object& oldValue );

    PropertySet _propertySet;
    bool _delayed;
    bool _blocked;
    std::set< rc_ptr< PropertyFilter > > _propertyFilters;
    std::set< rc_ptr< PropertyFilter > > _delayedEmissions;

};


  template <typename T> inline
  carto::Object getObjectHeader( Headered & h )
  {
    return Object::reference( h.header() );
  }

}


#endif
