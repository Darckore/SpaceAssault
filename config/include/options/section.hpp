#pragma once
#include "options/option.hpp"

namespace engine::config
{
  class section
  {
  public:
    using opt_type  = option;
    using name_type = opt_type::name_type;

  public:
    CLASS_SPECIALS_NODEFAULT_NOCOPY(section);

    explicit section(name_type name) noexcept;

    section(name_type name, section* parent) noexcept;

  private:
    section* m_parent{};
    name_type m_name;
  };
}