#pragma once
#include "game/scene.hpp"

namespace assault::game
{
  class level : public engine::world::scene
  {
  public:
    using base_type   = engine::world::scene;
    using time_type   = base_type::time_type;
    using gfx_type    = base_type::gfx_type;
    using owner_type  = base_type::owner_type;
    using camera_type = base_type::camera_type;
    using cfg_type    = base_type::cfg_type;

  public:
    CLASS_SPECIALS_NONE(level);

    level(owner_type& owner, gfx_type& g) noexcept;

  public:
    virtual void update(time_type dt) noexcept override;
    virtual void render() noexcept override;

  private:
    virtual bool init() noexcept override;
  };
}