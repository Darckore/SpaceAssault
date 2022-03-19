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

  camera::camera(gfx_type& g) noexcept :
    m_gfx{ &g }
  {
  }

  camera::operator bool() const noexcept
  {
    return !utils::eq(m_fieldSize, dist_type{});
  }

  // Public members

  void camera::resize(dist_type fieldSize) noexcept
  {
    m_fieldSize = fieldSize;
  }

  void camera::update() noexcept
  {
    if (!*this)
    {
      // Report error
      return;
    }

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