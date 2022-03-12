#pragma once
#include "core/err.hpp"

namespace assault::graphics
{
  class window
  {
  public:
    class wnd_error : public err::error
    {
    public:
      static constexpr auto prefix = "window"sv;

    public:
      using error::error;
    };

  public:
    using str_type      = std::string;
    using size_type     = std::int32_t;
    using proc_result   = std::intptr_t;
    using handle_type   = void*;

    struct dimensions
    {
      bool operator==(const dimensions&) const noexcept = default;

      size_type width;
      size_type height;
    };

  private:
    struct msg_wrapper;
    friend struct wnd_helper;

  public:
    CLASS_SPECIALS_NONE(window);

    ~window() noexcept;

    window(str_type title, str_type name) noexcept;

    explicit operator bool() const noexcept;

  private:
    proc_result window_proc(msg_wrapper msg) noexcept;

  public:
    template <typename Handle>
    auto handle() const noexcept
    {
      return reinterpret_cast<Handle>(m_handle);
    }
    dimensions  size() const noexcept;

  private:
    void init() noexcept;

  private:
    str_type m_title;
    str_type m_name;
    handle_type m_handle{};
    dimensions m_size{};
  };
}