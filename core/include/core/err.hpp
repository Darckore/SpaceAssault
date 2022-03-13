#pragma once

namespace engine::err
{
  class error
  {
  public:
    using loc_type = std::source_location;
    using msg_type = std::string;
    using str_type = std::string_view;

  public:
    error(msg_type msg, loc_type at = loc_type::current()) noexcept :
      m_msg{ std::move(msg) },
      m_loc{ at }
    { }

  public:
    str_type what() const noexcept
    {
      return m_msg;
    }
    const loc_type& where() const noexcept
    {
      return m_loc;
    }

  private:
    msg_type m_msg;
    loc_type m_loc;
  };
}