#include <cartobase/config/cartobase_config.h>

/* The following code declares classes to read from and write to
 * file descriptore or file handles.
 *
 * See
 *      http://www.josuttis.com/cppcode
 * for details and the latest version.
 *
 * - open:
 *      - integrating BUFSIZ on some systems?
 *      - optimized reading of multiple characters
 *      - stream for reading AND writing
 *      - i18n
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * Version: Jul 28, 2002
 * History:
 *  Jul 28, 2002: bugfix memcpy() => memmove()
 *                fdinbuf::underflow(): cast for return statements
 *  Aug 05, 2001: first public version
 */
#ifndef BOOST_FDSTREAM_HPP
#define BOOST_FDSTREAM_HPP

#if !defined( __GNUC__ ) || __GNUC__-0 >= 3
#include <istream>
#include <ostream>
#include <streambuf>
#else
#include <iostream>
#include <streambuf.h>
#include <string>
#endif

// BEGIN namespace BOOST
namespace boost {


/************************************************************
 * fdostream
 * - a stream that writes on a file descriptor
 ************************************************************/


class fdoutbuf : public std::streambuf {
  protected:
    int fd;    // file descriptor
#if defined( __GNUC__ ) && __GNUC__-0 < 3
    typedef int int_type;
#endif
#ifdef _WIN32
    // on windows we can't write() on a socket fd (!)
    bool _sock;
#endif
  public:
    // constructor
  fdoutbuf (int _fd );

  protected:
    // write one character
    virtual int_type overflow (int_type c);
    // write multiple characters
    virtual
    std::streamsize xsputn (const char* s,
                            std::streamsize num);
};

class fdostream : public std::ostream {
  protected:
    fdoutbuf buf;
  public:
    fdostream (int fd) : std::ostream(0), buf(fd) {
        rdbuf(&buf);
    }
};


/************************************************************
 * fdistream
 * - a stream that reads on a file descriptor
 ************************************************************/

class fdinbuf : public std::streambuf {
  protected:
    int fd;    // file descriptor
#ifdef _WIN32
    // on windows we can't read() on a socket fd (!)
    bool _sock;
#endif
  protected:
    /* data buffer:
     * - at most, pbSize characters in putback area plus
     * - at most, bufSize characters in ordinary read buffer
     */
    static const int pbSize = 4;        // size of putback area
    static const int bufSize = 1024;    // size of the data buffer
    char buffer[bufSize+pbSize];        // data buffer
#if defined( __GNUC__ ) && __GNUC__-0 < 3
    typedef int int_type;
    typedef string::traits_type traits_type;
#endif

  public:
    /* constructor
     * - initialize file descriptor
     * - initialize empty data buffer
     * - no putback area
     * => force underflow()
     */
  fdinbuf (int _fd);

  protected:
    // insert new characters into the buffer
    virtual int_type underflow ();
};

class fdistream : public std::istream {
  protected:
    fdinbuf buf;
  public:
    fdistream (int fd) : std::istream(0), buf(fd) {
        rdbuf(&buf);
    }
};


} // END namespace boost

#endif /*BOOST_FDSTREAM_HPP*/
