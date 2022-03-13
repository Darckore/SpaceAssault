#pragma once
#include "game/base_game.hpp"
#include "gfx/camera.hpp"

namespace engine::world
{
  class scene
  {
  public:
    using owner_type  = i_game;
    using gfx_type    = owner_type::graphics_type;
    using time_type   = owner_type::time_type;
    using camera_type = graphics::camera;

  public:
    CLASS_SPECIALS_NONE(scene);

    scene(owner_type& owner, camera_type cam) noexcept;

  public:
    virtual void update(time_type dt) noexcept = 0;
    virtual void render() noexcept = 0;

  public:
    camera_type& camera() noexcept;
    owner_type& game() noexcept;

  private:
    owner_type& m_owner;
    camera_type m_cam;
  };
}