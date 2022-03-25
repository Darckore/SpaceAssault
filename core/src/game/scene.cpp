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

  scene::operator bool() const noexcept
  {
    return m_status == status::ready;
  }

  // Public members

  bool scene::is_loading() const noexcept
  {
    return m_status == status::loading;
  }

  scene::camera_type& scene::camera() noexcept
  {
    return m_cam;
  }
  scene::owner_type& scene::game() noexcept
  {
    return m_owner;
  }

  bool scene::load() noexcept
  {
    loading();
    if (!init())
    {
      error();
      return false;
    }

    ok();
    return true;
  }

  // Protected members

  void scene::err_and_quit() noexcept
  {
    error();
    m_owner.request_quit(*this);
  }

  // Private members

  void scene::loading() noexcept
  {
    m_status = status::loading;
  }
  void scene::ok() noexcept
  {
    m_status = status::ready;
  }
  void scene::error() noexcept
  {
    m_status = status::error;
  }
}