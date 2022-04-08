#pragma once
#include "core/core.hpp"
#include "game/component_store.hpp"

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
    using scene_type      = world::scene;
    using component_store = world::component_store;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(base_game);

    virtual ~base_game() noexcept;

    base_game() noexcept;

  public:
    virtual void request_quit(const world::scene& caller) noexcept = 0;

  protected:
    virtual bool before_run() noexcept = 0;
    virtual void on_update(time_type dt) noexcept = 0;
    virtual void on_render() noexcept = 0;

  private:
    bool init() noexcept;
    void update(time_type dt) noexcept;
    void render() noexcept;

  protected:
    void run() noexcept;
    void quit() noexcept;

    scene_type& scene() noexcept;

    bool switch_scene(scene_type& s) noexcept;

    component_store& components() noexcept;

  private:
    scene_type* m_curScene{};

    component_store m_components;
  };
}