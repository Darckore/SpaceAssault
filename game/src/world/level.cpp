#include "world/level.hpp"
#include "config/conf.hpp"

// stupid test code
namespace test_thingy
{
  using assault::game::level;

  struct thing
  {
    static constexpr utils::vecd2 v1{ 0.0,    10.0 };
    static constexpr utils::vecd2 v2{ -10.0, -10.0 };
    static constexpr utils::vecd2 v3{ 10.0,  -10.0 };
    static double angle;

    static utils::vecd2 to1;
    static utils::vecd2 to2;
    static utils::vecd2 to3;

    static void step(level::camera_type& cam, float dt) noexcept
    {
      if (angle >= 360.0)
        angle = 0.0;

      angle += 45.0 * dt;

      const auto origin = cam.origin();
      to1 = v1.get_rotated(utils::deg_to_rad(angle)) - origin;
      to2 = v2.get_rotated(utils::deg_to_rad(angle)) - origin;
      to3 = v3.get_rotated(utils::deg_to_rad(angle)) - origin;
    }
    static void draw(level::camera_type& cam) noexcept
    {
      cam.gfx().draw(to1, to2);
      cam.gfx().draw(to2, to3);
      cam.gfx().draw(to3, to1);
    }
  };

  double thing::angle = 0.0;
  utils::vecd2 thing::to1;
  utils::vecd2 thing::to2;
  utils::vecd2 thing::to3;

}

namespace assault::game
{
  // Special members

  level::level(owner_type& owner, gfx_type& g) noexcept :
    base_type{ owner, camera_type { g } }
  {
  }

  // Public members

  void level::update(time_type dt) noexcept
  {
    auto&& cam = camera();
    if (!cam)
    {
      err_and_quit();
      return;
    }

    cam.update();

    // stupid test code
    test_thingy::thing::step(cam, dt);
  }
  void level::render() noexcept
  {
    // stupid test code
    using test_thingy::thing;
    thing::draw(camera());
  }

  bool level::init() noexcept
  {
    // stupid test code
    cfg_type cfg{ "data/levels/test_lvl.lvl" };
    auto lvl = cfg->get_section("level"sv);
    if (!lvl) return false;

    auto camOpt = lvl->get_option("camera"sv);
    if (!camOpt) return false;
    auto tOpt = camOpt->to<double, double, double>();
    if (!tOpt) return false;
    auto [posX, posY, visible] = *tOpt;

    auto&& cam = camera();

    cam.look_at({ posX, posY });
    cam.resize(visible);
    return true;
  }
}