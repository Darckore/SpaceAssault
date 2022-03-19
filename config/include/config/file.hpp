#pragma once

namespace engine::config
{
  class cfg_file
  {
  public:
    using name_type   = std::string_view;
    using line_type   = std::string_view;
    using in_type     = std::ifstream;
    using out_type    = std::ofstream;
    using buf_type    = std::string;
    using iterator    = buf_type::const_iterator;

  public:
    CLASS_SPECIALS_NODEFAULT_NOCOPY(cfg_file);

    explicit cfg_file(name_type fname) noexcept;

    explicit operator bool() const noexcept;

  public:
    void rewind() noexcept;
    void discard() noexcept;

    line_type line() noexcept;

  private:
    void read() noexcept;

  private:
    name_type m_name;
    buf_type  m_buf;
    iterator  m_cur;
  };
}