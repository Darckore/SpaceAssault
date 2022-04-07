#include "core/core.hpp"
#include "core/win_includes.hpp"
#include "game/base_game.hpp"

namespace engine
{
  // Statics

  void core::startup() noexcept
  {
    if (auto&& inst = instance())
    {
      inst->run();
    }

    // todo: error
  }

  // Special members

  core::~core() noexcept
  {
    quit();
  }

  core::core(game_type& game) noexcept :
    m_game{ game }
  {
  }

  // Private members

  void core::run() noexcept
  {
    // init game
  }
  void core::loop() noexcept
  {
    // main loop
  }
  void core::quit() noexcept
  {
    // exit
  }
}