#include "options/section.hpp"

namespace engine::config
{
  // Special members
  section::section(name_type name) noexcept :
    section{ name, nullptr }
  { }

  section::section(name_type name, section* parent) noexcept :
    m_parent{ parent },
    m_name{ name }
  { }
}