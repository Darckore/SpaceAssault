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

  gfx::ratio_type gfx::aspect() const noexcept
  {
    return m_aspect;
  }
  gfx::size_type gfx::width() const noexcept
  {
    return m_size.width;
  }
  gfx::size_type gfx::height() const noexcept
  {
    return m_size.height;
  }

  void gfx::bind_scaling(scale_type scale) noexcept
  {
    m_scale = width() / scale;
  }
  void gfx::set_origin(vertex_type origin) noexcept
  {
    m_origin = origin;
  }

  // stupid test code
  void gfx::draw(const vertex_type& v1, const vertex_type& v2) noexcept
  {
    const auto cv1 = world_to_viewport(v1);
    const auto cv2 = world_to_viewport(v2);
    m_renderer->line(cv1, cv2);
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

  gfx::vertex_type gfx::world_to_viewport(const vertex_type& v) const noexcept
  {
    auto vView = (v - m_origin) * m_scale;
    return vView;
  }
}