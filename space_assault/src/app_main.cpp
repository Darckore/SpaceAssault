#include "game/game.hpp"
namespace sa = assault::game;
#include "lodepng/lodepng.h"

int main()
{
  sa::game game;
  game.run();
  return 0;
}