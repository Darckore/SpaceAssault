#include "core/core.h"
#include "core/win_includes.h"

#include "gfx/window.h"
#include "gfx/gfx.h"

namespace assault
{
  // Special members

  core::core()
  {
    run();
  }

  // Private members

  void core::run()
  {
    using time_type = float;
    using clock_type = utils::clock<time_type>;

    constexpr auto framerate = 1.0f / 60.0f;

    auto mainWnd = create_window();
    auto gfx     = init_graphics(mainWnd);

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
        gfx.begin_frame();
        clock();
      }

      gfx.draw();
    }
  }

  // Private members

  core::window_type core::create_window()
  {
  #ifdef NDEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
  #endif

    return { "Space Assault", "SA_MAIN", 1280, 720 };
  }
  core::graphics_type core::init_graphics(window_type& wnd)
  {
    return { wnd };
  }
}