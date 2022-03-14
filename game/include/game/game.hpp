#pragma once
#include "game/base_game.hpp"

namespace assault::game
{
  class game final : public engine::base_game
  {
  public:
    using base_type  = engine::base_game;
    using scene_type = base_type::scene_type;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(game);

    ~game() noexcept;

    game() noexcept;

  public:
    void request_quit(const scene_type& caller) noexcept override;

  private:
    virtual bool before_run() noexcept override;
    virtual void update(time_type dt) noexcept override;
    virtual void render() noexcept override;
  };
}