#include "game/scene.hpp"
#include "gfx/gfx.hpp"

namespace engine::world
{
  // Special members

  scene::scene(owner_type& owner, gfx_type& g) noexcept :
    m_owner{ owner },
    m_gfx{ g }
  {
  }

  // Public members

  scene::gfx_type& scene::gfx() noexcept
  {
    return m_gfx;
  }
  scene::owner_type& scene::game() noexcept
  {
    return m_owner;
  }
}