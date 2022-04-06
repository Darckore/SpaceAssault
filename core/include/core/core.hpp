#pragma once
#include "gfx/window.hpp"
#include "gfx/gfx.hpp"

namespace engine
{
  class core
  {
  public:
    friend class base_game;
    using graphics_type = graphics::gfx;
    using game_type     = base_game;

    static constexpr auto fps = time_type{ 60 };
    static constexpr auto framerate = utils::inv(fps);

  private:
    static time_type clamp_time(time_type dt) noexcept;

  public:
    CLASS_SPECIALS_NONE(core);

    explicit core(game_type& game) noexcept;

    explicit operator bool() const noexcept;

  private:
    void run() noexcept;
    void loop() noexcept;
    void shutdown() noexcept;

    graphics_type& gfx() noexcept;

  private:
    graphics_type m_gfx;
    game_type& m_game;
  };
}