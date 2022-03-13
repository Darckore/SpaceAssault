#include "game/game.hpp"
#include "world/level.hpp"

namespace assault::game
{
  // stupid test code
  static level* make_lvl(auto& owner, auto& g) noexcept
  {
    static level lvl{ owner, g };
    return &lvl;
  }

  // Special members

  game::~game() noexcept = default;

  game::game() noexcept :
    m_curScene{ make_lvl(*this, gfx()) } // stupid test code
  {
  }

  // Private members

  bool game::before_run() noexcept
  {
    return true;
  }
  void game::update(time_type dt) noexcept
  {
    m_curScene->update(dt);
  }
  void game::render() noexcept
  {
    m_curScene->render();
  }
}