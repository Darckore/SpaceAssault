#include "game/game.hpp"

// stupid test code
namespace test_thingy
{
  struct line
  {
    static constexpr utils::vecd2 init{ 960.0, 270.0 };
    static utils::vecd2 from;
    static utils::vecd2 to;
    static double angle;

    static void step(float dt)
    {
      if (angle >= 360.0)
        angle = 0.0;

      angle += 45.0 * dt;
      to = init.get_rotated(utils::deg_to_rad(angle));
    }
  };

  utils::vecd2 line::from{ 960.0, 540.0 };
  utils::vecd2 line::to{ init };
  double line::angle = 0.0;
}

namespace assault::game
{
  // Special members

  // Private members

  bool game::before_run() noexcept
  {
    return true;
  }
  void game::update(time_type dt) noexcept
  {
    utils::unused(dt);

    // stupid test code
    test_thingy::line::step(dt);
  }
  void game::render() noexcept
  {
    // stupid test code
    using test_thingy::line;
    gfx().draw(line::from, line::to);
  }
}