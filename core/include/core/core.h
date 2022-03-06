#pragma once
#include "gfx/window.h"

namespace assault
{
  class core
  {
  public:
    CLASS_SPECIALS_NONE_CUSTOM(core);

    core();

  private:
    void run();

  private:
    graphics::window m_main_wnd;
  };
}