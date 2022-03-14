#include "world/level.hpp"

// stupid test code
namespace test_thingy
{
  struct line
  {
    static constexpr utils::vecd2 init{ 0.0, 20.0 };
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

  utils::vecd2 line::from{ 0.0, 0.0 };
  utils::vecd2 line::to{ init };
  double line::angle = 0.0;

  // temporaty, controls the level chunk that should fill in the viewport
  constexpr auto visibleSize = 40.0; // meters?
}

namespace assault::game
{
  // Special members

  level::level(owner_type& owner, gfx_type& g) noexcept :
    base_type{ owner, camera_type { g, test_thingy::visibleSize } }
  {
  }

  // Public members

  void level::update(time_type dt) noexcept
  {
    camera().update();

    // stupid test code
    test_thingy::line::step(dt);
  }
  void level::render() noexcept
  {
    // stupid test code
    using test_thingy::line;
    camera().gfx().draw(line::from, line::to);
  }

  bool level::init() noexcept
  {
    return true;
  }
}