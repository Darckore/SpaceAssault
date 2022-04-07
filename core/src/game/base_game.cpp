#include "game/base_game.hpp"
#include "game/scene.hpp"

namespace engine
{
  // Special members

  base_game::~base_game() noexcept
  {
    core::shutdown();
  }

  base_game::base_game() noexcept
  {
    if (!core::create(*this))
    {
      // todo: error
    }
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
    core::startup();
  }
  void base_game::quit() noexcept
  {
    core::shutdown();
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