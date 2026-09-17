

//===========================================================================//
//                                                                           //
// WARNING : All code here is under construction. Do not use it.             //
//                                                                           //
//===========================================================================//


#include <cartobase/algorithm/algorithm.h>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

namespace carto {

  //-----------------//
 //  BaseParameter  //
//-----------------//

//-----------------------------------------------------------------------------
BaseParameter::~BaseParameter() {}


  //-------------------------//
 //  _registeredAlgorithms  //
//-------------------------//

// I think there should be a class for algorithm registration 
// (class Namespace ?). But for this example code, I will just
// use STL classes.
//-----------------------------------------------------------------------------
map< string, vector< Algorithm *> >  _registeredAlgorithms;


  //-------------//
 //  Algorithm  //
//-------------//

//-----------------------------------------------------------------------------
Algorithm::Algorithm( const string &name ) : _name( name )
{
  cerr << "register algorithm: " << name << endl;
  _registeredAlgorithms[ _name ].push_back( this );
}



  //-------------------//
 //  AlgorithmCaller  //
//-------------------//

//-----------------------------------------------------------------------------
AlgorithmCaller::AlgorithmCaller( const string &algorithmName ) :
  _name( algorithmName ),
  _namedParameters( Object::value( PropertySet() ) )
{
}


//-----------------------------------------------------------------------------
template <>
AlgorithmCaller &AlgorithmCaller::
operator <<( const  AlgorithmCaller::LaunchExecution & )
{
  // TODO: Find the appropriate algorithm to launch according to :
  //       - named parameters,
  //       - unnamed parameters.
  //       - registered "converter" algorithms
  //       - registered "reader" or "writer" algorithms
  //
  return *this;
}


//-----------------------------------------------------------------------------
const AlgorithmCaller::LaunchExecution execute = 
           AlgorithmCaller::LaunchExecution();



} // namespace carto
