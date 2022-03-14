#pragma once
#include "core/core.hpp"

namespace engine
{
  namespace world
  {
    class scene;
  }

  class i_game
  {
  public:
    using time_type     = core::time_type;
    using graphics_type = core::graphics_type;

    virtual void request_quit(const world::scene& caller) noexcept = 0;
  };
}