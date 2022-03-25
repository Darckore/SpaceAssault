#include "assets/sprite.hpp"
using engine::graphics::sprite;

namespace engine_tests
{
  TEST(sprites, t_png)
  {
    constexpr auto fname = "data/test_set/img/sample.png"sv;

    sprite s{ fname };
    ASSERT_TRUE(s);

    const auto w = s.width();
    const auto h = s.height();
    EXPECT_EQ(w, 4u);
    EXPECT_EQ(h, 1u);
  }

  TEST(sprites, t_bmp)
  {
    constexpr auto fname = "data/test_set/img/sample.png"sv;

    sprite s{ fname, sprite::bmp };
    ASSERT_TRUE(s);

    const auto w = s.width();
    const auto h = s.height();
    EXPECT_EQ(w, 4u);
    EXPECT_EQ(h, 1u);
  }

}