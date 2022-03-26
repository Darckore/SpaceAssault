#include "assets/sprite.hpp"
using engine::graphics::sprite;

namespace engine_tests
{
  namespace detail
  {
    void test_sprite_sample(const sprite& s)
    {
      ASSERT_TRUE(s);

      const auto w = s.width();
      const auto h = s.height();
      EXPECT_EQ(w, 4u);
      EXPECT_EQ(h, 1u);

      using utils::colour;
      namespace colours = utils::colours;
      constexpr auto c0 = colours::red;
      constexpr auto c1 = colours::lime;
      constexpr auto c2 = colours::blue;
      constexpr auto c3 = colour{ 128, 64, 32 };

      auto quads = s.colours();
      auto end = quads.end();
      
      auto it = quads.begin();
      
      ASSERT_TRUE(it != end);
      EXPECT_TRUE((*(it++)) == c0);
      
      ASSERT_TRUE(it != end);
      EXPECT_TRUE((*(it++)) == c1);

      ASSERT_TRUE(it != end);
      EXPECT_TRUE((*(it++)) == c2);

      ASSERT_TRUE(it != end);
      EXPECT_TRUE((*(it++)) == c3);

      ASSERT_TRUE(it == end);
    }
  }

  TEST(sprites, t_png)
  {
    constexpr auto fname = "data/test_set/img/sample.png"sv;
    sprite s{ fname };
    detail::test_sprite_sample(s);
  }

  TEST(sprites, t_bmp)
  {
    constexpr auto fname = "data/test_set/img/sample.png"sv;
    sprite s{ fname, sprite::bmp };
    detail::test_sprite_sample(s);
  }

}