#include "config/conf.hpp"

namespace config
{
  // Special members

  cfg::cfg(file_name fname) noexcept :
    m_file{ fname },
    m_root{ "~" }
  {
    read();
  }

  cfg::operator bool() const noexcept
  {
    return static_cast<bool>(m_file);
  }

  // Private members

  void cfg::read() noexcept
  {
    if (!parse())
    {
      m_file.discard();
      return;
    }

    m_file.rewind();
  }

  bool cfg::parse() noexcept
  {
    return true;
  }
}