#pragma once
#include "core/core.hpp"

namespace engine
{
  class base_game
  {
  public:
    CLASS_SPECIALS_NONE_CUSTOM(base_game);

    base_game();

  public:
    void run();

  protected:
    void quit();

  private:
    core m_engine;
  };
}