#pragma once
#include "core/core.hpp"

namespace engine
{
  class base_game
  {
  public:
    using time_type     = core::time_type;
    using graphics_type = core::graphics_type;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(base_game);

    base_game() noexcept;

  public:
    void run() noexcept;

    virtual bool before_run() noexcept = 0;
    virtual void update(time_type dt) noexcept = 0;
    virtual void render() noexcept = 0;

  protected:
    graphics_type& gfx() noexcept;

    void quit() noexcept;

  private:
    core m_engine;
  };
}