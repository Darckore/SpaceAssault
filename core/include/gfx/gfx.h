#pragma once
#include "gfx/window.h"
#include "core/exception.h"

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
    void release_renderer();

  private:
    window& m_wnd;
    renderer& m_render;
    viewport_size m_viewPort{};
  };
}