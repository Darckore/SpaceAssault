#include "game/game.hpp"

namespace assault::game
{
  // Special members

  // Private members

  bool game::before_run() noexcept
  {
    return true;
  }
  void game::update(time_type dt) noexcept
  {
    utils::unused(dt);
  }
  void game::render() noexcept
  {
  }
}