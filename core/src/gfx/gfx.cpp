#include "gfx/gfx.hpp"
#include "gfx/renderer.hpp"

namespace engine::graphics
{
  // Special members

  gfx::~gfx() noexcept = default;

  gfx::gfx() noexcept :
    m_wnd{ create_window() },
    m_renderer{ m_wnd },
    m_size{ m_wnd.size() },
    m_aspect{ calc_aspect_ratio() }
  { }

  gfx::operator bool() const noexcept
  {
    return static_cast<bool>(m_renderer);
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
  void gfx::draw(const sprite& s, const vertex_type& pos, const vertex_type& dir) noexcept
  {
    const auto p1 = world_to_viewport(pos);
    const auto d1 = world_to_viewport(dir).get_normalised();
    m_renderer.image(s, p1, dir);
  }

  // Private members

  window gfx::create_window() noexcept
  {
  #ifdef NDEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
  #endif

    return { "Todo", "GAME_MAIN" };
  }

  void gfx::begin_frame() noexcept
  {
    setup();
    m_renderer.init_drawing();
  }
  void gfx::draw() noexcept
  {
    m_renderer.end_drawing();
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
    m_renderer.resize();
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