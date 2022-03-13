#include "game/scene.hpp"
#include "gfx/gfx.hpp"

namespace engine::world
{
  // Special members

  scene::scene(owner_type& owner, camera_type cam) noexcept :
    m_owner{ owner },
    m_cam{ cam }
  {
  }

  // Public members

  scene::camera_type& scene::camera() noexcept
  {
    return m_cam;
  }
  scene::owner_type& scene::game() noexcept
  {
    return m_owner;
  }
}