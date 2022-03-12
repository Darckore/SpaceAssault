#pragma once
#include "gfx/window.hpp"
#include "core/err.hpp"

namespace engine::graphics
{
  class window;
  class renderer;

  class gfx
  {
  public:
    class gfx_error : public err::error
    {
    public:
      static constexpr auto prefix = "gfx"sv;

    public:
      using error::error;
    };

  public:
    using viewport_size = window::dimensions;
    using size_type     = window::size_type;
    using ratio_type    = utils::ratio<size_type>;

  public:
    CLASS_SPECIALS_NONE(gfx);

    ~gfx() noexcept;

    gfx(window& wnd) noexcept;

    explicit operator bool() const noexcept;

  public:
    void begin_frame() noexcept;
    void draw() noexcept;
    void setup() noexcept;

  private:
    renderer* get_renderer() noexcept;
    void release_renderer() noexcept;

    ratio_type calc_aspect_ratio() const noexcept;

  private:
    window& m_wnd;
    renderer* m_renderer;
    viewport_size m_size{};
    ratio_type m_aspect{};
  };
}