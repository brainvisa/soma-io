#ifndef CARTOBASE_ALGORITHM_ALGORITHM_H
#define CARTOBASE_ALGORITHM_ALGORITHM_H


//===========================================================================//
//                                                                           //
// WARNING : All code here is under construction. Do not use it.             //
//                                                                           //
//===========================================================================//


#include <cartobase/object/object.h>
#include <cartobase/object/property.h>

namespace carto {

  //-----------------//
 //  BaseParameter  //
//-----------------//

class BaseParameter
{
public:
  virtual ~BaseParameter();
};


  //----------------//
 //  Parameter<T>  //
//----------------//

template <typename T>
  class ParameterModifier;

template <typename T>
class Parameter : public BaseParameter
{
 public:
  inline Parameter() {}
  inline Parameter( T &ref, const std::string &name, 
                    const std::string &doc ) :
    _pValue( &ref ),
    _name( name ),
    _documentation( doc ) {}
  virtual ~Parameter();
  
 private:
  
  friend class ParameterModifier<T>;
  
  T *_pValue;
  std::string _name;
  std::string _documentation;
  bool _input;
  bool _output;
  std::vector< std::pair< std::string, T > > _choices;
  bool _optional;
};


  //-------------//
 //  Algorithm  //
//-------------//

class Algorithm
{
public:

  Algorithm( const std::string &name );


protected:

  template <typename T>
  ParameterModifier<T> inputParameter( T &ref, const std::string &name, 
                                       const std::string &documentation );
  template <typename T>
  ParameterModifier<T> outputParameter( T &ref, const std::string &name, 
                                        const std::string &documentation );

private:

  std::string _name;

  // It is not going to be a vector in the future, maybe a kind of ParameterSet
  std::vector< rc_ptr< BaseParameter > > _parameters;
};


  //------------------------//
 //  ParameterModifier<T>  //
//------------------------//

template <typename T>
class ParameterModifier
{
public:
  
  inline ParameterModifier( const ParameterModifier<T> &pm ) :
  _parameter( pm._parameter ) {}

  inline ParameterModifier( Parameter<T> &p ) :
  _parameter( p ) {}
  
  inline ParameterModifier<T> optional( bool o = true ) 
  { 
    _parameter._optional = o;
    return *this;
  }
  
  inline ParameterModifier<T> choice( const T &value )
  {
    return choices( "", value );
  }

  inline ParameterModifier<T> choice( const std::string &label,
                                       const T &value )
  { 
    _parameter._choices.push_back( std::pair<std::string, T>( label,
                                                              value ) );
    return *this; 
  }
  
 private:
  
  Parameter<T> &_parameter;
};


  //-------------------//
 //  AlgorithmCaller  //
//-------------------//

class AlgorithmCaller
{
public:

  AlgorithmCaller( const std::string &algorithmName );
  template <typename T>
  AlgorithmCaller &operator <<( const T & );

  class LaunchExecution {};

private:

  std::string _name;
  std::vector< carto::Object > _unnamedParameters;
  carto::Object _namedParameters;
};

extern const AlgorithmCaller::LaunchExecution execute;

typedef AlgorithmCaller algo;





  //----------------//
 //  Parameter<T>  //
//----------------//

//-----------------------------------------------------------------------------
template <typename T>
Parameter<T>::~Parameter() {}


  //-------------//
 //  Algorithm  //
//-------------//

//-----------------------------------------------------------------------------
template <typename T>
ParameterModifier<T>
Algorithm::inputParameter( T &ref, const std::string &name, 
                           const std::string &doc )
{
  _parameters.push_back( rc_ptr<BaseParameter>( new Parameter<T>( ref, name, doc ) ) );
  // TODO set _input = true;
  return ParameterModifier<T>( static_cast< Parameter<T> &>( *_parameters.rbegin()->get() ) );
}


//-----------------------------------------------------------------------------
template <typename T>
ParameterModifier<T>
Algorithm::outputParameter( T &ref, const std::string &name, 
                            const std::string &doc )
{
  _parameters.push_back( rc_ptr<BaseParameter>( new Parameter<T>( ref, name, doc ) ) );
  // TODO set _output = true;
  return ParameterModifier<T>( static_cast< Parameter<T> &>( *_parameters.rbegin()->get() ) );
}


  //-------------------//
 //  AlgorithmCaller  //
//-------------------//

template <typename T>
AlgorithmCaller &AlgorithmCaller::operator <<( const T &v )
{
  _unnamedParameters.push_back( Object::value( v ) );
  return *this;
}

template <>
AlgorithmCaller &AlgorithmCaller::
operator <<( const  AlgorithmCaller::LaunchExecution & );



} // namespace carto


#endif //ifndef CARTOBASE_ALGORITHM_ALGORITHM_H
