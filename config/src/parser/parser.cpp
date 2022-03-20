#include "parser/parser.hpp"

namespace config
{
  // Special members

  parser::parser(cfg_file& file) noexcept :
    m_file{ file },
    m_res{ root_name },
    m_curRoot{ &*m_res }
  {
    if (!parse())
      m_res.reset();
  }

  parser::operator bool() const noexcept
  {
    return m_res.has_value();
  }

  // Public members

  section parser::get() noexcept
  {
    return std::move(*m_res);
  }

  // Private members

  bool parser::parse() noexcept
  {
    return true;
  }
}