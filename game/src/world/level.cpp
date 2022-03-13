#include "world/level.hpp"

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

  level::level(owner_type& owner, gfx_type& g) noexcept :
    base_type{ owner, g }
  {
  }

  // Public members

  void level::update(time_type dt) noexcept
  {
    // stupid test code
    test_thingy::line::step(dt);
  }
  void level::render() noexcept
  {
    // stupid test code
    using test_thingy::line;
    gfx().draw(line::from, line::to);
  }
}