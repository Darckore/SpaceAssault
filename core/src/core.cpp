#include "core/core.hpp"
#include "core/win_includes.hpp"
#include "game/base_game.hpp"

namespace assault
{
  // Special members

  core::core(game_type& game) :
    m_wnd{ create_window() },
    m_gfx{ init_graphics(m_wnd) },
    m_game{ game }
  {
  }

  // Private members

  void core::run()
  {
    using time_type = float;
    using clock_type = utils::clock<time_type>;

    constexpr auto framerate = 1.0f / 60.0f;

    MSG msg{};
    clock_type clock;
    
    while (msg.message != WM_QUIT)
    {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }

      if (const auto dt = clock.peek();
                     dt >= framerate)
      {
        m_gfx.begin_frame();
        clock();
      }

      m_gfx.draw();
    }
  }
  void core::shutdown()
  {
    PostQuitMessage(0);
  }

  core::window_type core::create_window()
  {
  #ifdef NDEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
  #endif

    return { "Space Assault", "SA_MAIN" };
  }
  core::graphics_type core::init_graphics(window_type& wnd)
  {
    return { wnd };
  }
}