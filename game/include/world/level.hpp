#pragma once
#include "game/scene.hpp"

namespace assault::game
{
  class level : public engine::world::scene
  {
  public:
    using base_type = engine::world::scene;

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