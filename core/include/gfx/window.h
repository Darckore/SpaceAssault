#pragma once
#include "core/exception.h"

namespace assault::graphics
{
  class window
  {
  public:
    class wnd_error : public err::exception
    {
    public:
      using exception::exception;
    };

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
    proc_result window_proc(msg_wrapper msg) noexcept;
    handle_type handle() const noexcept;

  private:
    void init();

  private:
    str_type m_title;
    str_type m_name;
    handle_type m_handle{};
    size_type m_width{};
    size_type m_height{};
  };
}