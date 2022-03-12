#pragma once
#include "gfx/window.hpp"
#include "core/exception.hpp"

namespace assault::graphics
{
  class window;
  class renderer;

  class gfx
  {
  public:
    class gfx_error : public err::exception
    {
    public:
      using exception::exception;
    };

  public:
    using viewport_size = window::dimensions;
    using size_type     = window::size_type;
    using ratio_type    = utils::ratio<size_type>;

  public:
    CLASS_SPECIALS_NONE(gfx);

    ~gfx() noexcept;

    gfx(window& wnd);

  public:
    void begin_frame() noexcept;
    void draw() noexcept;
    void setup() noexcept;

  private:
    renderer& get_renderer();
    void release_renderer() noexcept;

    ratio_type calc_aspect_ratio() const noexcept;

  private:
    window& m_wnd;
    renderer& m_renderer;
    viewport_size m_size{};
    ratio_type m_aspect{};
  };
}