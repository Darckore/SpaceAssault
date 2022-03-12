#pragma once
#include "game/base_game.hpp"

namespace assault::game
{
  class game : public assault::base_game
  {
  public:
    CLASS_SPECIALS_NONE_CUSTOM(game);

    game() = default;
  };
}