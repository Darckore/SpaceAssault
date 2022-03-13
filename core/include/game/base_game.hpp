#pragma once
#include "game/i_game.hpp"

namespace engine
{
  namespace world
  {
    class scene;
  }

  class base_game : public i_game
  {
  public:
    friend class core;

  public:
    using base_type     = i_game;
    using time_type     = base_type::time_type;
    using graphics_type = base_type::graphics_type;
    using scene_type    = world::scene;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(base_game);

    base_game() noexcept;

  public:
    void run() noexcept;
    void quit() noexcept;

  protected:
    virtual bool before_run() noexcept = 0;
    virtual void update(time_type dt) noexcept = 0;
    virtual void render() noexcept = 0;

  protected:
    graphics_type& gfx() noexcept;
    scene_type& scene() noexcept;

    void switch_scene(scene_type& s) noexcept;

  private:
    core m_engine;
    scene_type* m_curScene{};
  };
}