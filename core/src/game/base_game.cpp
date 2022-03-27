#include "game/base_game.hpp"
#include "game/scene.hpp"

namespace engine
{
  // Special members

  base_game::base_game() noexcept :
    m_engine{ *this }
  {
  }

  // Protected members

  void base_game::run() noexcept
  {
    m_engine.run();
  }
  void base_game::quit() noexcept
  {
    m_engine.shutdown();
  }

  base_game::graphics_type& base_game::gfx() noexcept
  {
    return m_engine.gfx();
  }
  base_game::scene_type& base_game::scene() noexcept
  {
    return *m_curScene;
  }

  bool base_game::switch_scene(scene_type& s) noexcept
  {
    if (!s)
    {
      // todo: Report error here
      return false;
    }

    m_curScene = &s;
    return true;
  }

  base_game::component_store& base_game::components() noexcept
  {
    return m_components;
  }
}