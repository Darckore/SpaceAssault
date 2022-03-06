#include "core/core.h"
#include "core/win_includes.h"

namespace assault
{
  // Special members

  core::core() :
    m_main_wnd{ "nekis", "kekpuk", 1280, 720 }
  {
    run();
  }

  // Private members

  void core::run()
  {
  #ifdef NDEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
  #endif

    MSG msg{};
    while (msg.message != WM_QUIT)
    {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
}