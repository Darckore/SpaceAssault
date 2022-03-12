#include "gfx/gfx.hpp"
#include "gfx/renderer.hpp"

namespace engine::graphics
{
  // Special members

  gfx::~gfx() noexcept
  {
    release_renderer();
  }

  gfx::gfx(window& wnd) noexcept :
    m_wnd{ wnd },
    m_renderer{ get_renderer() },
    m_size{ wnd.size() },
    m_aspect{ calc_aspect_ratio() }
  { }

  gfx::operator bool() const noexcept
  {
    return m_wnd && m_renderer;
  }

  // Public members

  // stupid test code
  void gfx::draw(const utils::vecd2& v1, const utils::vecd2& v2) noexcept
  {
    m_renderer->line(v1, v2);
  }

  // Private members

  void gfx::begin_frame() noexcept
  {
    setup();
    m_renderer->init_drawing();
  }
  void gfx::draw() noexcept
  {
    m_renderer->end_drawing();
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
    m_renderer->reset();
  }

  renderer* gfx::get_renderer() noexcept
  {
    if (!m_wnd)
      return {};

    return &renderer::get(this, m_wnd);
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