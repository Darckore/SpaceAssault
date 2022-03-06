#include "gfx/gfx.h"
#include "gfx/window.h"
#include "core/win_includes.h"

namespace assault::graphics
{
  // Special members

  gfx::~gfx() noexcept
  { }

  gfx::gfx(window& wnd) :
    m_wnd{ wnd }
  { }

  // Public members

  void gfx::begin_frame() noexcept
  {

  }
  void gfx::draw() noexcept
  {

  }

}