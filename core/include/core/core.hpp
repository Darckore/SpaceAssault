#pragma once
#include "gfx/window.hpp"
#include "gfx/gfx.hpp"

namespace engine
{
  class core
  {
  public:
    friend class base_game;

    using time_type = float;
    using clock_type = utils::clock<time_type>;
    using window_type   = graphics::window;
    using graphics_type = graphics::gfx;
    using game_type = base_game;

    static constexpr auto fps = time_type{ 60 };
    static constexpr auto framerate = utils::inv(fps);

  private:
    static bool past_frame(time_type dt) noexcept;
    static time_type clamp_time(time_type dt) noexcept;

  public:
    CLASS_SPECIALS_NONE(core);

    explicit core(game_type& game) noexcept;

    explicit operator bool() const noexcept;

  private:
    void run() noexcept;
    void loop() noexcept;
    void shutdown() noexcept;

    window_type create_window() noexcept;
    graphics_type init_graphics(window_type& wnd) noexcept;

  private:
    window_type m_wnd;
    graphics_type m_gfx;
    game_type& m_game;
  };
}