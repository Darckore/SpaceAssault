#pragma once
#include "game/base_game.hpp"

namespace assault::game
{
  class game final : public engine::base_game
  {
  public:
    using time_type = engine::time_type;
    using base_type = engine::base_game;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(game);

    ~game() noexcept;

    game() noexcept;

  public:
    virtual void request_quit(const scene_type& caller) noexcept override;

  private:
    virtual bool before_run() noexcept override;
    virtual void on_update(time_type dt) noexcept override;
    virtual void on_render() noexcept override;
  };
}