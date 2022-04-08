#pragma once

namespace engine::graphics
{
  namespace detail
  {
    struct msg_wrapper;
    struct wnd_helper;
  }

  class window final
  {
  public:
    using size_type     = std::int32_t;
    using proc_result   = std::intptr_t;
    using handle_type   = std::uintptr_t;

    struct dimensions
    {
      bool operator==(const dimensions&) const noexcept = default;

      size_type width;
      size_type height;
    };

  private:
    using msg_wrapper = detail::msg_wrapper;
    friend struct detail::wnd_helper;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(window);

    ~window() noexcept;

    window() noexcept;

    explicit operator bool() const noexcept;

  private:
    proc_result window_proc(msg_wrapper msg) noexcept;

  public:
    handle_type handle() const noexcept;
    dimensions size() const noexcept;

  private:
    void init() noexcept;

  private:
    handle_type m_handle{};
    dimensions  m_size{};
  };
}