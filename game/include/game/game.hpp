#pragma once
#include "game/base_game.hpp"

namespace assault::game
{
  class game final : public engine::base_game
  {
  public:
    using base_type  = engine::base_game;
    using level_type = engine::world::scene;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(game);

    ~game() noexcept;

    game() noexcept;

  private:
    virtual bool before_run() noexcept override;
    virtual void update(time_type dt) noexcept override;
    virtual void render() noexcept override;

  private:
    level_type* m_curScene{};
  };
}