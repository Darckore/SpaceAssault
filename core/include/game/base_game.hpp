#pragma once
#include "core/core.hpp"

namespace engine
{
  namespace world
  {
    class scene;
  }

  class base_game
  {
  public:
    friend class core;

  public:
    using time_type     = core::time_type;
    using graphics_type = core::graphics_type;
    using scene_type    = world::scene;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(base_game);

    base_game() noexcept;

  public:
    virtual void request_quit(const world::scene& caller) noexcept = 0;

  protected:
    virtual bool before_run() noexcept = 0;
    virtual void update(time_type dt) noexcept = 0;
    virtual void render() noexcept = 0;

  protected:
    void run() noexcept;
    void quit() noexcept;

    graphics_type& gfx() noexcept;
    scene_type& scene() noexcept;

    bool switch_scene(scene_type& s) noexcept;

  private:
    core m_engine;
    scene_type* m_curScene{};
  };
}