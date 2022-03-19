#include "game/game.hpp"
#include "world/level.hpp"

namespace assault::game
{
  // stupid test code
  static level& make_lvl(auto& owner, auto& g) noexcept
  {
    static level lvl{ owner, g };
    return lvl;
  }

  // Special members

  game::~game() noexcept = default;

  game::game() noexcept = default;

  // Public members

  void game::request_quit(const scene_type& caller) noexcept
  {
    utils::unused(caller);
    quit();
  }

  // Private members

  bool game::before_run() noexcept
  {
    // stupid test code
    auto&& lvl = make_lvl(*this, gfx());
    if (!lvl.load())
    {
      return false;
    }

    return switch_scene(lvl);
  }
  void game::update(time_type dt) noexcept
  {
    scene().update(dt);
  }
  void game::render() noexcept
  {
    scene().render();
  }
}