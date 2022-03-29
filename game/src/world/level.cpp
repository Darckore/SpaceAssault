#include "world/level.hpp"
#include "config/conf.hpp"
#include "assets/sprite.hpp"

// stupid test code
namespace test_thingy
{
  using assault::game::level;

  struct thing
  {
    static constexpr utils::vecd2 pos{};
    static constexpr utils::vecd2 dir = utils::vecd2::axis_norm<1>();
    static double angle;
    static utils::vecd2 heading;

    static auto& get_sprite() noexcept
    {
      using engine::graphics::sprite;
      static sprite s{ "data/assets/thing.png", sprite::bmp };
      return s;
    }
    static void step(level::camera_type& cam, float dt) noexcept
    {
      utils::unused(cam);
      if (angle >= 360.0)
        angle = 0.0;

      angle += 45.0 * dt;
      heading = dir.get_rotated(utils::deg_to_rad(angle));
    }
    static void draw(level::camera_type& cam) noexcept
    {
      auto&& ss = get_sprite();
      cam.gfx().draw(ss, pos, heading);
    }
  };

  double thing::angle = 0.0;
  utils::vecd2 thing::heading = dir;

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