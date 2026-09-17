
#ifndef CARTOBASE_STREAM_ATTACHABLE_CUIFSTREAM_H
#define CARTOBASE_STREAM_ATTACHABLE_CUIFSTREAM_H

#include <cartobase/config/cartobase_config.h>
#include <cartobase/stream/cuifstream.h>

namespace carto
{

  /**	Cette classe n'es là que parce que Dimitri ne voulait pas que je 
	touche à sa belle classe cuifstream. Il aurait suffi de remplacer 
	dans cuifstream la vartiable "en dur" _base par un pointeur, mais 
	ça ne lui convenait pas.
  */
  class CARTOBASE_API attachable_cuifstream : public cuifstream
  {
  public:
    attachable_cuifstream();
    attachable_cuifstream( const char *name );
    attachable_cuifstream( std::istream & s );

    virtual ~attachable_cuifstream();

    ///	attach to an existing (and open) stream
    virtual void attach( std::istream & s, int line_num = 1 );
    virtual void close();

    virtual bool operator!() const;
    virtual bool is_open() const;

  protected:
    virtual void init_buffers();
    virtual void destroy_buffers();

  private:
    std::istream	*_attached;
  };

}

#endif

