#ifndef _HeaderProxy_h_
#define _HeaderProxy_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif

#include <string>
#include <vector>


namespace dcm
{


class HeaderProxy
{

  public:

    HeaderProxy();
    virtual ~HeaderProxy();

    virtual void addAttribute( const std::string& name, int32_t value ) = 0;
    virtual void addAttribute( const std::string& name, double value ) = 0;
    virtual void addAttribute( const std::string& name, 
                               const std::string& value ) = 0;

    virtual void addAttribute( const std::string& name, 
                               const std::vector< int32_t >& value ) = 0;
    virtual void addAttribute( const std::string& name, 
                               const std::vector< double >& value ) = 0;
    virtual void addAttribute( const std::string& name, 
                               const std::vector< std::string >& value ) = 0;

    virtual void addAttribute( 
                        const std::string& name, 
                        const std::vector< std::vector< double > >& value ) = 0;

    virtual bool hasAttribute( const std::string& name ) = 0;

};


}


#endif
