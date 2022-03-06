#pragma once

namespace assault::err
{
  class exception : public std::runtime_error
  {
  public:
    using location = std::source_location;

  public:
    exception(const std::string& msg, location at = location::current()) :
      exception{ msg.c_str(), at }
    { }
    exception(const char* msg, location at = location::current()) :
      runtime_error{ msg },
      m_loc{ at }
    { }

  public:
    const location& where() const noexcept
    {
      return m_loc;
    }

  private:
    location m_loc;
  };
}