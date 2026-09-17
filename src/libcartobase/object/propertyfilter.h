#ifndef CARTOBASE_OBJECT_PROPERTYFILTER_H
#define CARTOBASE_OBJECT_PROPERTYFILTER_H


#include <cartobase/smart/rcobject.h>
#include <cartobase/object/object.h>
#include <cartobase/sigc++/sigc++.h>
#include <string>
#include <set>
#include <map>


namespace carto
{


class PropertyFilter : public RCObject
{

  public:

    typedef ::sigc::signal1< void, const PropertyFilter& > Signal;
    typedef ::sigc::slot1< void, const PropertyFilter& > Slot;

    PropertyFilter( const std::string& name,
                    const std::set< std::string >& propertyNames );
    PropertyFilter( const std::string& name,
                    const std::string& propertyName );
    virtual ~PropertyFilter();


    const std::string& getName() const;
    Signal& getSignal();

    bool hasProperty( const std::string& propertyName ) const;
    bool hasOldValue( const std::string& propertyName ) const;

    void setOldValue( const std::string& propertyName,
                      const Object& oldValue );
    Object getOldValue( const std::string& propertyName ) const;

    void emitsignal();


  protected:

    std::string _name;
    std::set< std::string > _propertyNames;
    std::map< std::string, Object > _oldValues;
    Signal _signal;

};


}


#endif
