#include "game/base_game.hpp"

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

  // Protected members

  base_game::graphics_type& base_game::gfx() noexcept
  {
    return m_engine.gfx();
  }

  void base_game::quit() noexcept
  {
    m_engine.shutdown();
  }
}