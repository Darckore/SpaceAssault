#include "game/game.hpp"

namespace assault::game
{
  // Special members

  game::~game() noexcept = default;

  game::game() noexcept
  {
    run();
  }

  // Public members

  void game::request_quit(const scene_type& caller) noexcept
  {
    utils::unused(caller);
    quit();
  }

  // Private members

  bool game::before_run() noexcept
  {
    return true;
  }
  void game::on_update(time_type dt) noexcept
  {
    utils::unused(dt);
  }
  void game::on_render() noexcept
  {
  }
}