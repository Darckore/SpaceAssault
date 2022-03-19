#include "config/conf.hpp"

namespace engine::config
{
  // Special members

  cfg::cfg(fname_type fname) noexcept :
    m_file{ fname }
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
    while (m_file)
    {
      auto l = m_file.line();
      utils::unused(l);
    }
  }

}