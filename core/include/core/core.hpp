#pragma once
#include "gfx/window.hpp"
#include "gfx/gfx.hpp"

namespace engine
{
  class core
  {
  public:
    friend class base_game;

    using window_type   = graphics::window;
    using graphics_type = graphics::gfx;
    using game_type = base_game;

  public:
    CLASS_SPECIALS_NONE(core);

    explicit core(game_type& game) noexcept;

    explicit operator bool() const noexcept;

  private:
    void run() noexcept;
    void shutdown() noexcept;

  private:
    window_type create_window() noexcept;
    graphics_type init_graphics(window_type& wnd) noexcept;

  private:
    window_type m_wnd;
    graphics_type m_gfx;
    game_type& m_game;
  };
}