#pragma once

namespace assault
{
  namespace graphics
  {
    class window;
    class gfx;
  }

  class core
  {
  public:
    using window_type   = graphics::window;
    using graphics_type = graphics::gfx;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(core);

    core();

  private:
    void run();

  private:
    window_type create_window();
    graphics_type init_graphics(window_type& wnd);
  };
}