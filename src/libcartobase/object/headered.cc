#include <cartobase/object/headered.h>
#include <cartobase/uuid/uuid.h>
#include <sigc++/slot.h>

using namespace carto;
using namespace std;


Headered::Headered()
  : RCObject(), 
    Observable(),
    Observer(),
    _delayed( false ), 
    _blocked( false )
{
}


Headered::Headered( const carto::Headered& other )
  : RCObject(), 
    Observable( other ),
    Observer( other ),
    _propertySet( other._propertySet ),
    _delayed( false )
{
  // avoid copying the UUID since a copy is generally a different data
  if( _propertySet.hasProperty( "uuid" ) )
    _propertySet.removeProperty( "uuid" );
}


Headered::~Headered()
{
}


Headered& 
Headered::operator = ( const Headered& other )
{

  _propertySet = other._propertySet;
  // avoid copying the UUID since a copy is generally a different data
  if( _propertySet.hasProperty( "uuid" ) )
    _propertySet.removeProperty( "uuid" );

  initialize();
  return *this;

}


const PropertySet& Headered::header() const
{

  return _propertySet;

}


PropertySet& Headered::header()
{

  return _propertySet;

}


void 
Headered::addPropertyFilter( const rc_ptr< PropertyFilter >& propertyFilter )
{

  if ( !hasPropertyFilter( propertyFilter->getName() ) )
  {

    _propertyFilters.insert( propertyFilter );

  }

}


bool 
Headered::hasPropertyFilter( const std::string& propertyFilterName ) const
{

  std::set< rc_ptr< PropertyFilter > >::const_iterator
    f = _propertyFilters.begin(),
    fe = _propertyFilters.end();

  while ( f != fe )
  {

    if ( ( *f )->getName() == propertyFilterName )
    {

      return true;

    }
    ++ f;

  }
  return false;

}


bool carto::Headered::connect( const std::string& propertyFilterName,
                               const PropertyFilter::Slot& slot )
{

  std::set< rc_ptr< PropertyFilter > >::const_iterator
    f = _propertyFilters.begin(),
    fe = _propertyFilters.end();

  while ( f != fe )
  {

    if ( ( *f )->getName() == propertyFilterName )
    {

      ( *f )->getSignal().connect( slot );
      return true;

    }
    ++ f;

  }
  return false;


}


bool carto::Headered::disconnect( const std::string& propertyFilterName,
                                  const PropertyFilter::Slot& slot )
{

  std::set< rc_ptr< PropertyFilter > >::const_iterator
    f = _propertyFilters.begin(),
    fe = _propertyFilters.end();

  while ( f != fe )
  {

    if ( ( *f )->getName() == propertyFilterName )
    {

      PropertyFilter::Signal::slot_list_type 
        sl = ( *f )->getSignal().sigcslots();
      PropertyFilter::Signal::iterator	is, es = sl.end();
      for( is=sl.begin();
           is!=es && static_cast<const sigc::slot_base*>(&(*is))!=static_cast<const sigc::slot_base*>(&slot);
           ++is ) ;
      if( is != es )
        {
          is->disconnect();
          return true;
        }

    }
    ++ f;

  }
  return false;

}


std::set< rc_ptr< PropertyFilter > >
Headered::getPropertyFilters( const std::string& propertyName ) const
{

  std::set< rc_ptr< PropertyFilter > > concernedPropertyFilters;

  std::set< rc_ptr< PropertyFilter > >::const_iterator
    f = _propertyFilters.begin(),
    fe = _propertyFilters.end();

  while ( f != fe )
  {

    if ( ( *f )->hasProperty( propertyName ) )
    {

      concernedPropertyFilters.insert( *f );

    }
    ++ f;

  }
  return concernedPropertyFilters;

}


void Headered::delay()
{

  _delayed = true;

}


void Headered::blockSignals( bool x )
{

  _blocked = x;

}


bool Headered::signalsBlocked() const
{

  return _blocked;

}


void Headered::flush(  )
{

  std::set< rc_ptr< PropertyFilter > >::const_iterator
    e = _delayedEmissions.begin(),
    ee = _delayedEmissions.end();
  while ( e != ee )
  {

    ( *e )->emitsignal();
    ++ e;

  }
  _delayedEmissions.clear();
  _delayed = false;

}


void Headered::initialize()
{

  _propertySet.getSignalPropertyChanged().connect( 
                   ::sigc::mem_fun( *this, &Headered::slotPropertyChanged ) );

}


void Headered::slotPropertyChanged( const Object &, 
                                    const std::string& propertyName,
                                    const Object& oldValue )
{
  if( _blocked )
    return; // when signals are blocked, just do nothing

  // finding all property filters pointing to propertyName
  std::set< rc_ptr< PropertyFilter > > concernedPropertyFilters =
    getPropertyFilters( propertyName );

  // adding concerned property filter to delayed emission set
  std::set< rc_ptr< PropertyFilter > >::const_iterator
    f = concernedPropertyFilters.begin(),
    fe = concernedPropertyFilters.end();

  while ( f != fe )
  {

    // setting old value for property only inside filters that still don't have
    // any old value
    if ( !( *f )->hasOldValue( propertyName ) )
    {

      ( *f )->setOldValue( propertyName, oldValue );

    }
    _delayedEmissions.insert( *f );
    ++ f;

  }

  // emitting when not delayed
  if ( !_delayed )
  {

    flush();

  }

}


string Headered::uuid()
{
  string u;
  if( !_propertySet.getProperty( "uuid", u ) )
  {
    UUID  uu;
    uu.generate();
    u = uu.toString();
    _propertySet.setProperty( "uuid", u );
  }
  return u;
}


bool Headered::copyUuid( const PropertySet & x )
{
  string u;
  if( !x.getProperty( "uuid", u ) )
    return false;
  _propertySet.setProperty( "uuid", u );
  return true;
}


bool Headered::copyUuid( const Headered & x )
{
  return copyUuid( x.header() );
}

