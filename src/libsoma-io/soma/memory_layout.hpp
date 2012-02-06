#ifndef SOMA_MEMORY_LAYOUT
#define SOMA_MEMORY_LAYOUT

#include <vector>

namespace soma {

class MemoryLayout
{
public:
  inline MemoryLayout( size_t dimensions );
  inline void set_axis( int axis, size_t count, ptrdiff_t offset );
  
  inline size_t dimensions() const;
  inline size_t count( int axis ) const;
  inline ptrdiff_t offset( int axit ) const;
  
private:
  std::vector< std::pair< int, size_t > > _layout;
};


inline MemoryLayout::MemoryLayout( size_t dimensions )
{
  _layout.resize( dimensions );
}


inline void MemoryLayout::set_axis( int axis, size_t count, ptrdiff_t offset )
{
  _layout[ axis ].first = count;
  _layout[ axis ].second = offset;
}


inline size_t MemoryLayout::dimensions() const
{
  return _layout.size();
}


inline size_t MemoryLayout::count( int axis ) const
{
  return _layout[ axis ].first;
}


inline ptrdiff_t MemoryLayout::offset( int axis ) const
{
  return _layout[ axis ].second;
}


} // namespace soma

#endif // SOMA_MEMORY_LAYOUT
