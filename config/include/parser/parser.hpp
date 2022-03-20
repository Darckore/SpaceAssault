#pragma once
#include "config/file.hpp"
#include "options/section.hpp"

namespace config
{
  class parser
  {
  public:
    static constexpr auto root_name = "~"sv;

  public:
    using value_type = section;
    using value_opt  = std::optional<value_type>;
    using line_type  = cfg_file::line_type;
    using size_type  = line_type::size_type;

  public:
    CLASS_SPECIALS_NONE(parser);

    explicit parser(cfg_file& file) noexcept;

    explicit operator bool() const noexcept;

  public:
    section get() noexcept;

  private:
    bool parse() noexcept;

  private:
    cfg_file& m_file;
    value_opt m_res;
    value_type* m_curRoot{};
  };
}