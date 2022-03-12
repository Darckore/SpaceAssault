#include "gfx/gfx.hpp"
#include "gfx/renderer.hpp"

namespace assault::graphics
{
  // Special members

  gfx::~gfx() noexcept
  {
    release_renderer();
  }

  gfx::gfx(window& wnd) :
    m_wnd{ wnd },
    m_render{ get_renderer() },
    m_size{ wnd.size() },
    m_aspect{ calc_aspect_ratio() }
  { }

  // Public members

  void gfx::begin_frame() noexcept
  {
    setup();
    m_render.init_drawing();
  }
  void gfx::draw() noexcept
  {
    m_render.end_drawing();
  }
  void gfx::setup() noexcept
  {
    auto wndSize = m_wnd.size();
    if (wndSize == m_size)
    {
      return;
    }

    m_size = wndSize;
    m_aspect = calc_aspect_ratio();
    m_render.reset();
  }

  // Private members

  renderer& gfx::get_renderer()
  {
    return renderer::get(this, m_wnd);
  }
  void gfx::release_renderer() noexcept
  {
    renderer::release(this);
  }

  gfx::ratio_type gfx::calc_aspect_ratio() const noexcept
  {
    return ratio_type{ m_size.width, m_size.height }.get_simplified();
  }

}