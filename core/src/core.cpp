#include "core/core.hpp"
#include "core/sys_registry.hpp"
#include "game/base_game.hpp"
#include "core/logger/logger.hpp"

namespace engine
{
  using graphics::window;

  // Statics

  void core::startup() noexcept
  {
    if (auto&& inst = instance())
    {
      inst->run();
    }

    logger::error("Engine startup failed");
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
    if (!systems::init_system<window>())
    {
      logger::error("Unable to create window");
      return;
    }

    loop();
  }
  void core::loop() noexcept
  {
    while (systems::window().update())
    {
      
    }
  }
  void core::quit() noexcept
  {
    systems::shutdown_system<window>();
  }
}