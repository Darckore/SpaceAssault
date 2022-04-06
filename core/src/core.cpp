#include "core/core.hpp"
#include "core/win_includes.hpp"
#include "game/base_game.hpp"

namespace engine
{
  // Statics

  time_type core::clamp_time(time_type dt) noexcept
  {
    return std::min(dt, framerate);
  }

  // Special members

  core::core(game_type& game) noexcept :
    m_game{ game }
  {
  }

  core::operator bool() const noexcept
  {
    return static_cast<bool>(m_gfx);
  }

  // Private members

  void core::run() noexcept
  {
    if (!*this)
    {
      return;
    }

    if (!m_game.init())
    {
      return;
    }

    loop();
  }
  void core::loop() noexcept
  {
    MSG msg{};
    clock_type clock;

    while (msg.message != WM_QUIT)
    {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }

      if (!m_gfx)
      {
        // todo: error
        shutdown();
      }

      m_gfx.begin_frame();

      m_game.update(clamp_time(clock()));
      m_game.render();
      
      if (m_gfx)
      {
        m_gfx.draw();
      }
    }
  }
  void core::shutdown() noexcept
  {
    PostQuitMessage(0);
  }

  core::graphics_type& core::gfx() noexcept
  {
    return m_gfx;
  }
}