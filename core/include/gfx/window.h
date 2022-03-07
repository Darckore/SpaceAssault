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

    struct dimensions
    {
      size_type width;
      size_type height;
    };

  private:
    struct msg_wrapper;
    friend struct wnd_helper;

  public:
    CLASS_SPECIALS_NONE(window);

    ~window() noexcept;

    window(str_type title, str_type name, size_type width, size_type height);

  private:
    proc_result window_proc(msg_wrapper msg) noexcept;

  public:
    handle_type handle() const noexcept;
    dimensions  size() const noexcept;

  private:
    void init();

  private:
    str_type m_title;
    str_type m_name;
    handle_type m_handle{};
    dimensions m_size{};
  };
}