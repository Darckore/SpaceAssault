#include "config/file.hpp"

namespace engine::config
{
  // Special members

  cfg_file::cfg_file(name_type fname) noexcept :
    m_name{ fname },
    m_cur{ m_buf.begin() }
  {
    read();
  }

  cfg_file::operator bool() const noexcept
  {
    return m_cur != m_buf.end();
  }

  // Public members

  void cfg_file::rewind() noexcept
  {
    m_cur = m_buf.begin();
  }

  cfg_file::line_type cfg_file::line() noexcept
  {
    constexpr auto eol = '\n';
    auto it = m_cur;
    for (; it != m_buf.end(); ++it)
    {
      if (*it == eol)
      {
        ++it;
        if (utils::trim(line_type{ m_cur, it }).empty())
        {
          m_cur = it;
          continue;
        }

        break;
      }
    }

    auto res = utils::rtrim(line_type{ m_cur, it });
    m_cur = it;
    return res;
  }

  // Private members

  void cfg_file::read() noexcept
  {
    m_buf.clear();
    in_type in(m_name.data());
    if (!in)
      return;

    in.seekg(0, std::ios::end);
    m_buf.reserve(in.tellg());
    in.seekg(0, std::ios::beg);
    
    using it = std::istreambuf_iterator<buf_type::value_type>;
    m_buf.assign(it{ in }, it{});
    m_cur = m_buf.begin();
  }

}