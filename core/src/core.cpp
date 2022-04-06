#include "core/core.hpp"
#include "core/win_includes.hpp"
#include "game/base_game.hpp"

namespace engine
{
  // Special members

  core::core() noexcept
  {
  }

  core::operator bool() const noexcept
  {
    return true;
  }

  // Private members

  void core::run() noexcept
  {
    if (!*this)
    {
      return;
    }

    // init game

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
      // main loop
    }
  }
  void core::shutdown() noexcept
  {
    PostQuitMessage(0);
  }
}