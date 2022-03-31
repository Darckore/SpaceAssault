#include "core/core.hpp"
#include "core/win_includes.hpp"
#include "game/base_game.hpp"

namespace engine
{
  // Statics

  bool core::past_frame(time_type dt) noexcept
  {
    return dt >= framerate;
  }
  core::time_type core::clamp_time(time_type dt) noexcept
  {
    return std::min(dt, framerate);
  }

  // Special members

  core::core(game_type& game) noexcept :
    m_wnd{ create_window() },
    m_gfx{ init_graphics(m_wnd) },
    m_game{ game }
  {
  }

  core::operator bool() const noexcept
  {
    return m_wnd && m_gfx;
  }

  // Private members

  void core::run() noexcept
  {
    if (!*this)
    {
      return;
    }

    if (!m_game.before_run())
    {
      return;
    }

    m_game.update(time_type{});
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

      m_gfx.begin_frame();

      m_game.update(clamp_time(clock()));
      m_game.render();
      
      m_gfx.draw();
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

  core::window_type core::create_window() noexcept
  {
  #ifdef NDEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
  #endif

    return { "Space Assault", "SA_MAIN" };
  }
  core::graphics_type core::init_graphics(window_type& wnd) noexcept
  {
    return { wnd };
  }
}