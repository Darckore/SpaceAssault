#include "gfx/renderer.hpp"
#include "gfx/window.hpp"
#include "platform/windows/win_includes.hpp"
#include "assets/sprite.hpp"

#include "gfx/pipeline/d3d.hpp"

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

namespace engine::graphics
{
  // Special members

  renderer::~renderer() noexcept
  {
    CoUninitialize();
  }

  renderer::renderer(const window& wnd) noexcept :
    m_wnd{ wnd }
  {
    if (FAILED(CoInitialize(nullptr)))
    {
      // todo: error
      return;
    }

    init();
  }

  renderer::operator bool() const noexcept
  {
    return static_cast<bool>(m_pipeline);
  }

  // Public members

  void renderer::resize() noexcept
  {
    //auto size = detail::wnd_size(m_wnd);
    //m_target->Resize(size);
  }

  void renderer::init_drawing() noexcept
  {
    if (!m_pipeline || !m_pipeline->begin_frame())
    {
      // todo: error
      m_pipeline.reset(nullptr);
    }
  }
  void renderer::end_drawing() noexcept
  {
    if (!m_pipeline || !m_pipeline->end_frame())
    {
      // todo: error
      m_pipeline.reset(nullptr);
    }
  }

  // Draw stuff

  // stupid test code
  void renderer::image(const sprite& s, const point_type& pos, const point_type& dir) noexcept
  {
    utils::unused(s, pos, dir);
  }

  // Private members

  void renderer::init() noexcept
  {
    m_pipeline = std::make_unique<detail::pipeline>(m_wnd);
    if (!*m_pipeline)
    {
      // todo: error
      m_pipeline.reset(nullptr);
    }
  }

  renderer::pixel_type renderer::viewport_to_screen(const point_type& v) const noexcept
  {
    using coord_t = pixel_type::value_type;
    const auto [w, h] = m_wnd.size();
    const auto x = static_cast<coord_t>(v[0]) + w / 2;
    const auto y = h / 2 - static_cast<coord_t>(v[1]);
    return { x, y };
  }

}