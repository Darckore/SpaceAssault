#include "game/base_game.hpp"
#include "game/scene.hpp"

namespace engine
{
  // Special members

  base_game::base_game() noexcept :
    m_engine{ *this }
  {
  }

  // Private members

  bool base_game::init() noexcept
  {
    if (!before_run())
    {
      // todo: error
      return false;
    }

    update(time_type{});
    return true;
  }
  void base_game::update(time_type dt) noexcept
  {
    scene().update(dt);
    on_update(dt);
  }
  void base_game::render() noexcept
  {
    scene().render();
    on_render();
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