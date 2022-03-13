#include "gfx/camera.hpp"

namespace engine::graphics
{
  namespace detail
  {
    auto norm(auto val, auto vMin) noexcept
    {
      return val >= vMin ? val : vMin;
    }
  }

  // Special members

  camera::camera(gfx_type& g, dist_type fs) noexcept :
    m_gfx{ &g },
    m_fieldSize{ fs }
  {
  }

  // Public members

  void camera::update() noexcept
  {
    using detail::norm;
    const auto gfxAspect = gfx().aspect();
    if (gfxAspect == m_aspect)
      return;

    m_aspect = gfxAspect;
    const auto scale = norm(m_fieldSize * m_aspect, m_fieldSize);
    gfx().bind_scaling(scale);
  }

  // Private members

  camera::gfx_type& camera::gfx() noexcept
  {
    return *m_gfx;
  }
}