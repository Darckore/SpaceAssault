#include "game/base_game.hpp"
#include "game/scene.hpp"

namespace engine
{
  // Special members

  base_game::base_game() noexcept :
    m_engine{ *this }
  {
  }

  // Public members

  void base_game::run() noexcept
  {
    m_engine.run();
  }
  void base_game::quit() noexcept
  {
    m_engine.shutdown();
  }

  // Protected members

  base_game::graphics_type& base_game::gfx() noexcept
  {
    return m_engine.gfx();
  }
  base_game::scene_type& base_game::scene() noexcept
  {
    return *m_curScene;
  }

  void base_game::switch_scene(scene_type& s) noexcept
  {
    m_curScene = &s;
  }
}