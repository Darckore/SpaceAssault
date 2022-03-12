#pragma once
#include "game/base_game.hpp"

namespace assault::game
{
  class game final : public engine::base_game
  {
  public:
    CLASS_SPECIALS_NONE_CUSTOM(game);

    game() = default;

  private:
    virtual bool before_run() noexcept override;
    virtual void update(time_type dt) noexcept override;
    virtual void render() noexcept override;
  };
}