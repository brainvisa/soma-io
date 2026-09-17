//--- plugin -----------------------------------------------------------------
#include <soma-io/image/mincimagereader_d.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//----------------------------------------------------------------------------

using namespace carto;

namespace soma
{

  template class MincImageReader<int8_t>;
  template class MincImageReader<int16_t>;
  template class MincImageReader<int32_t>;
  template class MincImageReader<int64_t>;
  template class MincImageReader<uint8_t>;
  template class MincImageReader<uint16_t>;
  template class MincImageReader<uint32_t>;
  template class MincImageReader<uint64_t>;
  template class MincImageReader<float>;
  template class MincImageReader<double>;

#ifdef MINC_MGH_SUPPORT
  template class FreeSurferMincImageReader<int8_t>;
  template class FreeSurferMincImageReader<int16_t>;
  template class FreeSurferMincImageReader<int32_t>;
  template class FreeSurferMincImageReader<int64_t>;
  template class FreeSurferMincImageReader<uint8_t>;
  template class FreeSurferMincImageReader<uint16_t>;
  template class FreeSurferMincImageReader<uint32_t>;
  template class FreeSurferMincImageReader<uint64_t>;
  template class FreeSurferMincImageReader<float>;
  template class FreeSurferMincImageReader<double>;
#endif

} // namespace soma
