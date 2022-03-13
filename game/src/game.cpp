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

  // Private members

  bool game::before_run() noexcept
  {
    // stupid test code
    switch_scene(make_lvl(*this, gfx()));
    return true;
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