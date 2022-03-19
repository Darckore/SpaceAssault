#pragma once
#include "config/file.hpp"
#include "options/section.hpp"

namespace engine::config
{
  class cfg
  {
  public:
    using file_type  = cfg_file;
    using file_name  = file_type::name_type;
    using line_type  = file_type::line_type;
    using name_type  = section::name_type;

    using sections = std::unordered_map<name_type, section>;

  public:
    CLASS_SPECIALS_NODEFAULT_NOCOPY(cfg);

    explicit cfg(file_name fname) noexcept;

    explicit operator bool() const noexcept;

  private:
    void read() noexcept;
    bool parse() noexcept;

  private:
    file_type m_file;
    sections  m_sections;
  };
}