#pragma once
#include "gfx/window.hpp"
#include "gfx/gfx.hpp"

namespace engine
{
  class core
  {
  public:
    friend class base_game;
    using graphics_type = graphics::gfx;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(core);

    core() noexcept;

    explicit operator bool() const noexcept;

  private:
    void run() noexcept;
    void loop() noexcept;
    void shutdown() noexcept;

  private:

  };
}