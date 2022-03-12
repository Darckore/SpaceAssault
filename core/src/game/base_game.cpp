#include "game/base_game.hpp"

namespace assault
{
  // Special members

  base_game::base_game()
    : m_engine{ *this }
  {
  }

  // Public members

  void base_game::run()
  {
    m_engine.run();
  }

  // Protected members

  void base_game::quit()
  {
    m_engine.shutdown();
  }
}