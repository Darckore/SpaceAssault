#pragma once

namespace assault::graphics
{
  class window
  {
  public:
    using str_type      = std::string;
    using size_type     = std::size_t;
    using proc_result   = std::intptr_t;
    using handle_type   = void*;

  private:
    struct msg_wrapper;

  public:
    CLASS_SPECIALS_NONE(window);

    ~window() noexcept;

    window(str_type title, str_type name, size_type width, size_type height);

  public:
    proc_result window_proc(msg_wrapper msg) noexcept;//(handle_type h, unsigned msg, uintptr_type wp, intptr_type lp) noexcept;

  private:
    bool init() noexcept;

  private:
    str_type m_title;
    str_type m_name;
    handle_type m_handle{};
    size_type m_width{};
    size_type m_height{};
  };
}