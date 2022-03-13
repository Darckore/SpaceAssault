#pragma once
#include "game/base_game.hpp"

namespace engine::world
{
  class scene
  {
  public:
    using owner_type = i_game;
    using gfx_type   = owner_type::graphics_type;
    using time_type  = owner_type::time_type;

  public:
    CLASS_SPECIALS_NONE(scene);

    scene(owner_type& owner, gfx_type& g) noexcept;

  public:
    virtual void update(time_type dt) noexcept = 0;
    virtual void render() noexcept = 0;

  public:
    gfx_type& gfx() noexcept;
    owner_type& game() noexcept;

  private:
    owner_type& m_owner;
    gfx_type& m_gfx;
  };
}